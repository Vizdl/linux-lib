.PHONY : read-config

read-config :
	chmod 755 scripts -R && \
	find scripts -type f | xargs -I {} dos2unix {} && \
	sh scripts/build_config.sh
clean-read-config :
	rm -rf read-config