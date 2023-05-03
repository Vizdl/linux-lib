/* vi: set sw=4 ts=4: */
/*
 * bare bones sendmail
 *
 * Copyright (C) 2008 by Vladimir Dronnikov <dronnikov@gmail.com>
 *
 * Licensed under GPLv2, see file LICENSE in this tarball for details.
 */
#include "libbb.h"
#include "mail.h"

static int smtp_checkp(const char *fmt, const char *param, int code)
{
	char *answer;
	const char *msg = command(fmt, param);
	// read stdin
	// if the string has a form \d\d\d- -- read next string. E.g. EHLO response
	// parse first bytes to a number
	// if code = -1 then just return this number
	// if code != -1 then checks whether the number equals the code
	// if not equal -> die saying msg
	while ((answer = xmalloc_fgetline(stdin)) != NULL)
		if (strlen(answer) <= 3 || '-' != answer[3])
			break;
	if (answer) {
		int n = atoi(answer);
		if (timeout)
			alarm(0);
		free(answer);
		if (-1 == code || n == code)
			return n;
	}
	bb_error_msg_and_die("%s failed", msg);
}

static int smtp_check(const char *fmt, int code)
{
	return smtp_checkp(fmt, NULL, code);
}

// strip argument of bad chars
static char *sane_address(char *str)
{
	char *s = str;
	char *p = s;
	while (*s) {
		if (isalnum(*s) || '_' == *s || '-' == *s || '.' == *s || '@' == *s) {
			*p++ = *s;
		}
		s++;
	}
	*p = '\0';
	return str;
}

static void rcptto(const char *s)
{
	smtp_checkp("RCPT TO:<%s>", s, 250);
}

int sendmail_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int sendmail_main(int argc UNUSED_PARAM, char **argv)
{
	char *opt_connect = opt_connect;
	char *opt_from;
	char *s;
	llist_t *list = NULL;
	char *domain = sane_address(safe_getdomainname());
	int code;

	enum {
	//--- standard options
		OPT_t = 1 << 0,         // read message for recipients, append them to those on cmdline
		OPT_f = 1 << 1,         // sender address
		OPT_o = 1 << 2,         // various options. -oi IMPLIED! others are IGNORED!
	//--- BB specific options
		OPT_w = 1 << 3,         // network timeout
		OPT_H = 1 << 4,         // use external connection helper
		OPT_S = 1 << 5,         // specify connection string
		OPT_a = 1 << 6,         // authentication tokens
	};

	// init global variables
	INIT_G();

	// save initial stdin since body is piped!
	xdup2(STDIN_FILENO, 3);
	G.fp0 = fdopen(3, "r");

	// parse options
	// -f is required. -H and -S are mutually exclusive
	opt_complementary = "f:w+:H--S:S--H:a::";
	// N.B. since -H and -S are mutually exclusive they do not interfere in opt_connect
	// -a is for ssmtp (http://downloads.openwrt.org/people/nico/man/man8/ssmtp.8.html) compatibility,
	// it is still under development.
	opts = getopt32(argv, "tf:o:w:H:S:a::", &opt_from, NULL, &timeout, &opt_connect, &opt_connect, &list);
	//argc -= optind;
	argv += optind;

	// process -a[upm]<token> options
	if ((opts & OPT_a) && !list)
		bb_show_usage();
	while (list) {
		char *a = (char *) llist_pop(&list);
		if ('u' == a[0])
			G.user = xstrdup(a+1);
		if ('p' == a[0])
			G.pass = xstrdup(a+1);
		// N.B. we support only AUTH LOGIN so far
		//if ('m' == a[0])
		//	G.method = xstrdup(a+1);
	}
	// N.B. list == NULL here
	//bb_info_msg("OPT[%x] AU[%s], AP[%s], AM[%s], ARGV[%s]", opts, au, ap, am, *argv);

	// connect to server

	// connection helper ordered? ->
	if (opts & OPT_H) {
		const char *args[] = { "sh", "-c", opt_connect, NULL };
		// plug it in
		launch_helper(args);
	// vanilla connection
	} else {
		int fd;
		// host[:port] not explicitly specified? -> use $SMTPHOST
		// no $SMTPHOST ? -> use localhost
		if (!(opts & OPT_S)) {
			opt_connect = getenv("SMTPHOST");
			if (!opt_connect)
				opt_connect = (char *)"127.0.0.1";
		}
		// do connect
		fd = create_and_connect_stream_or_die(opt_connect, 25);
		// and make ourselves a simple IO filter
		xmove_fd(fd, STDIN_FILENO);
		xdup2(STDIN_FILENO, STDOUT_FILENO);
	}
	// N.B. from now we know nothing about network :)

	// wait for initial server OK
	// N.B. if we used openssl the initial 220 answer is already swallowed during openssl TLS init procedure
	// so we need to kick the server to see whether we are ok
	code = smtp_check("NOOP", -1);
	// 220 on plain connection, 250 on openssl-helped TLS session
	if (220 == code)
		smtp_check(NULL, 250); // reread the code to stay in sync
	else if (250 != code)
		bb_error_msg_and_die("INIT failed");

	// we should start with modern EHLO
	if (250 != smtp_checkp("EHLO %s", domain, -1)) {
		smtp_checkp("HELO %s", domain, 250);
	}
	if (ENABLE_FEATURE_CLEAN_UP)
		free(domain);

	// perform authentication
	if (opts & OPT_a) {
		smtp_check("AUTH LOGIN", 334);
		// we must read credentials unless they are given via -a[up] options
		if (!G.user || !G.pass)
			get_cred_or_die(4);
		encode_base64(NULL, G.user, NULL);
		smtp_check("", 334);
		encode_base64(NULL, G.pass, NULL);
		smtp_check("", 235);
	}

	// set sender
	// N.B. we have here a very loosely defined algotythm
	// since sendmail historically offers no means to specify secrets on cmdline.
	// 1) server can require no authentication ->
	//	we must just provide a (possibly fake) reply address.
	// 2) server can require AUTH ->
	//	we must provide valid username and password along with a (possibly fake) reply address.
	//	For the sake of security username and password are to be read either from console or from a secured file.
	//	Since reading from console may defeat usability, the solution is either to read from a predefined
	//	file descriptor (e.g. 4), or again from a secured file.

	// got no sender address? -> use system username as a resort
	// N.B. we marked -f as required option!
	//if (!G.user) {
	//	// N.B. IMHO getenv("USER") can be way easily spoofed!
	//	G.user = xuid2uname(getuid());
	//	opt_from = xasprintf("%s@%s", G.user, domain);
	//}
	//if (ENABLE_FEATURE_CLEAN_UP)
	//	free(domain);
	smtp_checkp("MAIL FROM:<%s>", opt_from, 250);

	// process message

	// read recipients from message and add them to those given on cmdline.
	// this means we scan stdin for To:, Cc:, Bcc: lines until an empty line
	// and then use the rest of stdin as message body
	code = 0; // set "analyze headers" mode
	while ((s = xmalloc_fgetline(G.fp0)) != NULL) {
		// put message lines doubling leading dots
		if (code) {
			// escape leading dots
			// N.B. this feature is implied even if no -i (-oi) switch given
			// N.B. we need to escape the leading dot regardless of
			// whether it is single or not character on the line
			if ('.' == s[0] /*&& '\0' == s[1] */)
				printf(".");
			// dump read line
			printf("%s\r\n", s);
			free(s);
			continue;
		}

		// analyze headers
		// To: or Cc: headers add recipients
		if (0 == strncasecmp("To: ", s, 4) || 0 == strncasecmp("Bcc: " + 1, s, 4)) {
			rcptto(sane_address(s+4));
//			goto addh;
			llist_add_to_end(&list, s);
		// Bcc: header adds blind copy (hidden) recipient
		} else if (0 == strncasecmp("Bcc: ", s, 5)) {
			rcptto(sane_address(s+5));
			free(s);
			// N.B. Bcc: vanishes from headers!
		// other headers go verbatim
		} else if (s[0]) {
// addh:
			llist_add_to_end(&list, s);
		// the empty line stops analyzing headers
		} else {
			free(s);
			// put recipients specified on cmdline
			while (*argv) {
				s = sane_address(*argv);
				rcptto(s);
				llist_add_to_end(&list, xasprintf("To: %s", s));
				argv++;
			}
			// enter "put message" mode
			smtp_check("DATA", 354);
			// dump the headers
			while (list) {
				printf("%s\r\n", (char *) llist_pop(&list));
			}
			printf("%s\r\n" + 2); // quirk for format string to be reused
			// stop analyzing headers
			code++;
		}
	}

	// finalize the message
	smtp_check(".", 250);
	// ... and say goodbye
	smtp_check("QUIT", 221);
	// cleanup
	if (ENABLE_FEATURE_CLEAN_UP)
		fclose(G.fp0);

	return EXIT_SUCCESS;
}
