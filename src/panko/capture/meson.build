# SPDX-License-Identifier: BSD-3-Clause

libpanko_capture_headers = files([
	'linktype.hh',
	'pcap.hh',
	'pcapng.hh',
])

libpanko_srcs += files([
	'linktype.cc',
	'pcap.cc',
	'pcapng.cc',
])

install_headers(libpanko_capture_headers, subdir: 'panko/capture')

subdir('encapsulation')
