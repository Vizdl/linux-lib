.PHONY : read-config-high read-config-low

read-config-high :
	chmod 755 scripts -R && \
	find scripts -type f | xargs -I {} dos2unix {} && \
	sh scripts/build_config.sh "high"

read-config-low :
	chmod 755 scripts -R && \
	find scripts -type f | xargs -I {} dos2unix {} && \
	sh scripts/build_config.sh "low"

clean-read-config :
	rm -rf read-config