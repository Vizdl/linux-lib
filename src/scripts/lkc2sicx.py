#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os
import re

RE_CONFIG = r'^CONFIG.*=.*'
RE_CONFIG_NOT_SET = r'^# (CONFIG\w*) (is not set)$'    # example: # CONFIG_LOCALVERSION_AUTO is not set
RE_NORMAL_CONFIG = r'(^CONFIG\w*)=([ym]{1}).*'    # examples: CONFIG_PROC_KCORE=y  CONFIG_NF_NAT_IPV4=m
RE_ABNORMAL_CONFIG = r'(^CONFIG\w*)=(.*)'    # examples: CONFIG_DEFAULT_TCP_CONG="cubic"  CONFIG_SPLIT_PTLOCK_CPUS=4

pattern_config = re.compile(RE_CONFIG)
pattern_config_not_set = re.compile(RE_CONFIG_NOT_SET)
pattern_normal_config = re.compile(RE_NORMAL_CONFIG)
pattern_abnormal_config = re.compile(RE_ABNORMAL_CONFIG)

def get_args():
	arg_parser = argparse.ArgumentParser()
	arg_parser.add_argument('-s', '--src', default='.config', help='linux kernel config file path, default: .config', dest='src_path')
	arg_parser.add_argument('-d', '--dest', required=True, help='source insight conditional parsing xml file path', dest='dest_path')
	
	args = arg_parser.parse_args()
	
	return (args.src_path, args.dest_path + '.conditions.xml')

def parse_config_line(line):
	if pattern_config.match(line):
		normal_match = pattern_normal_config.match(line)
		if normal_match:
			return normal_match.groups()
		else:
			abnormal_match = pattern_abnormal_config.match(line)
			return abnormal_match.groups()
	else:
		not_set_match = pattern_config_not_set.match(line)
		if not_set_match:
			return not_set_match.groups()
		
	return None

def parse_kernel_config(config_path):
	if not os.path.exists(config_path):
		return None
	
	kernel_configs = []
	config_file = None
	try:
		config_file = open(config_path)
		content = config_file.readlines()
		
		for raw_line in content:
			line = raw_line.rstrip()
			config_item = parse_config_line(line)
			if config_item:
				kernel_configs.append(config_item)
	finally:
		if config_file:
			config_file.close()
	return kernel_configs

def convert_kc2sic(kernel_configs):
	siconfigs = []
	for kconfig in kernel_configs:
		config = ()
		if kconfig[1] == 'is not set':
			config = (kconfig[0], '')
		elif kconfig[1] == 'y' or kconfig[1] == 'm':
			config = (kconfig[0], '1')
		elif kconfig[1][0] == '"':    # abnormal config, string type, ="cubic"
		    config = (kconfig[0], kconfig[1][1:-1])
		else:
			config = kconfig
		
		siconfigs.append(config)
	return siconfigs

def save_configs(si_configs, dest_path):
	begin_text = '''<?xml version="1.0" encoding="utf-8"?>
<SourceInsightParseConditions
	AppVer="4.00.0084"
	AppVerMinReader="4.00.0019"
	>
	<ParseConditions>
		<Defines>
'''
	end_text = '''		</Defines>
	</ParseConditions>
</SourceInsightParseConditions>
'''
	config_format = '''			<define id="%s" value="%s" />
'''
	with open(dest_path, 'w') as config_file:
		config_file.write(begin_text)
		for config in si_configs:
			config_file.write(config_format % config)
		config_file.write(end_text)
	return True
	
if __name__ == '__main__':
	src_path, dest_path = get_args()
	
	kernel_configs = parse_kernel_config(src_path)
	if not kernel_configs:
		print 'Error: kernel config file "%s" is empty, please check the content.' % (src_path)
		exit()
		
	si_configs = convert_kc2sic(kernel_configs)
	
	save_status = save_configs(si_configs, dest_path)
	if not save_status:
		print 'Error: save source insight config file "%s" failed.' % (dest_path)
		exit()
	
	print 'successful convert linux kernel config "%s" to source insight config "%s".' % (src_path, dest_path) 
