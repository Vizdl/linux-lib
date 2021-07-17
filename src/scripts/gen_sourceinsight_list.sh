#!/bin/bash
ARCH=x86
MACH=x86_64
FILE_IN=$1
FILE_OUT=$2
# .c
SOURCE_LIST=""
# generated file list
FILE_LIST=""
# nest depth for function get_includes()
NEST_DTPTH=0
get_includes()
{
	local includes
	local file
	for file in $1
	do
		if [ ! -e ${file} ]; then
				continue
		fi
		if echo "${FILE_LIST}" | grep -E ${file} > /dev/null; then
				continue
		fi
		FILE_LIST="${FILE_LIST} ${file}"
		NEST_DTPTH=$((NEST_DTPTH+1))
		echo "<${NEST_DTPTH} : ${file}"
		includes=$(                                                                                \
				grep -E -H '^#include' ${file} |                                \
				sed -r \
						-e 's@^.*<(acpi/.*)>@include/\1@'                 \
						-e 's@^.*<(asm-generic/.*)>@include/\1@'\
						-e 's@^.*<(config/.*)>@include/\1@'         \
						-e 's@^.*<(crypto/.*)>@include/\1@'         \
						-e 's@^.*<(drm/.*)>@include/\1@'                 \
						-e 's@^.*<(generated/.*)>@include/\1@'         \
						-e 's@^.*<(keys/.*)>@include/\1@'                 \
						-e 's@^.*<(linux/.*)>@include/\1@'                 \
						-e 's@^.*<(math-emu/.*)>@include/\1@'         \
						-e 's@^.*<(media/.*)>@include/\1@'                 \
						-e 's@^.*<(misc/.*)>@include/\1@'                 \
						-e 's@^.*<(mtd/.*)>@include/\1@'                 \
						-e 's@^.*<(net/.*)>@include/\1@'                 \
						-e 's@^.*<(pcmcia/.*)>@include/\1@'         \
						-e 's@^.*<(rdma/.*)>@include/\1@'                 \
						-e 's@^.*<(rxrpc/.*)>@include/\1@'                 \
						-e 's@^.*<(scsi/.*)>@include/\1@'                 \
						-e 's@^.*<(sound/.*)>@include/\1@'                 \
						-e 's@^.*<(target/.*)>@include/\1@'         \
						-e 's@^.*<(trace/.*)>@include/\1@'                 \
						-e 's@^.*<(uapi/.*)>@include/\1@'                 \
						-e 's@^.*<(video/.*)>@include/\1@'                 \
						-e 's@^.*<(xen/.*)>@include/\1@'                 \
						-e "s@^.*<(asm/.*)>@arch/${ARCH}/include/\1 arch/${ARCH}/include/generated/\1@"        \
						-e "s@^.*<(mach/.*)>@arch/${ARCH}/mach-${MACH}/include/\1@"        \
						-e 's@(^.*/)[^/]+\.c.*\"(.*)\"@\1\2@'         \
						-e 's@/\*.*@@'                                                         \
						-e 's@^.*\#include.*$@@'                                  \
						-e 's@^@ @' |                                                        \
				sort |                                                                                 \
				uniq |                                                                                \
				tr -d '\n' |                                                                 \
				tr -d '\r'                                                                        \
		)
		if [ -n "${includes}" ]; then
				get_includes "${includes}"
		fi
		echo ">${NEST_DTPTH}) : ${file}"
		NEST_DTPTH=$((NEST_DTPTH-1))
	done
}
# get *.c from kernel build log
SOURCE_LIST=$(                                                \
        grep -E '^\s*CC' ${FILE_IN} |        \
        sed -r                                                         \
                -e 's/^\s*CC\s*/ /'                        \
                -e 's/\.o/\.c/'                        |        \
        tr -d '\n' |                                         \
        tr -d '\r'                                                \
)
get_includes "${SOURCE_LIST}"
FILE_LIST=$(echo "${FILE_LIST}" | sed -r -e 's/\s/\r\n/g' )
echo "${FILE_LIST}" > ${FILE_OUT}
