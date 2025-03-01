# SPDX-License-Identifier: BSD-3-Clause

libpanko_capture_headers = files([
	'file.hh',
	'linktype.hh',
	'packet.hh',
	'pcap.hh',
	'pcapng.hh',
])

libpanko_srcs += files([
	'file.cc',
	'linktype.cc',
	'packet.cc',
	'pcap.cc',
	'pcapng.cc',
])

install_headers(libpanko_capture_headers, subdir: 'panko/capture')

subdir('encapsulation')
