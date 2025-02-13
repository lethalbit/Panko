# SPDX-License-Identifier: BSD-3-Clause

shared_module(
	'%DISSECTOR_NAME%',
	files([ 'dissector.cc' ]),
	name_prefix: '',
	implicit_include_directories: false,
	include_directories: [
		root_inc, include_directories('.'),
	],
	cpp_args: [
		global_defs,
	],
	gnu_symbol_visibility: 'inlineshidden',
	link_with: [
		libpanko
	],
	install: true
	install_dir: panko_dissector_dir
)
