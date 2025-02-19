# SPDX-License-Identifier: BSD-3-Clause

subdir('io')

libpanko_support_headers = files([
	'file.hh',
	'leb128.hh',
	'paths.hh',
	'so_loader.hh',
	'sys.hh',
])

libpanko_srcs += files([
	'file.cc',
	'paths.cc',
	'so_loader.cc',
	'sys.cc',
])


install_headers(libpanko_support_headers, subdir: 'panko/support')
