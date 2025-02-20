# SPDX-License-Identifier: BSD-3-Clause

libpanko_core_headers = files([
	'bitfield.hh',
	'bytearray.hh',
	'errcodes.hh',
	'integers.hh',
	'mmap.hh',
	'strutils.hh',
	'types.hh',
	'units.hh',
])

libpanko_srcs += files([
	'bytearray.cc',
	'errcodes.cc',
	'strutils.cc',
])

install_headers(libpanko_core_headers, subdir: 'panko/core')
