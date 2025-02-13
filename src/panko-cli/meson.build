# SPDX-License-Identifier: BSD-3-Clause

panko_cli_srcs = files([
	'main.cc',
])

panko_cli = executable(
	'panko-cli',
	panko_cli_srcs,
	implicit_include_directories: false,
	include_directories: [
		root_inc, include_directories('.'),
	],
	cpp_args: [
		global_defs,
	],
	gnu_symbol_visibility: 'inlineshidden',
	dependencies: [
		threads, spdlog
	],
	link_with: [
		libpanko,
	],
	install: true,
)
