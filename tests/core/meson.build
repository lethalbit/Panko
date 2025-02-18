# SPDX-License-Identifier: BSD-3-Clause

integers_test = executable(
	'integers_test', 'integers.cc',
	dependencies: [ doctest, ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('Strange-width integers', integers_test, suite: [ 'core', 'integers' ])

types_test = executable(
	'types_test', 'types.cc',
	dependencies: [ doctest, ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('Core Types', types_test, suite: [ 'core', 'types' ])

strutils_test = executable(
	'strutils_test', [
		'strutils.cc',
		'@0@/src/panko/core/strutils.cc'.format(meson.project_source_root()),
	],
	dependencies: [ doctest, ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('String utilities', strutils_test, suite: [ 'core', 'strutils' ])

if fuzzing_tests.allowed()

endif
