# SPDX-License-Identifier: BSD-3-Clause

types_test = executable(
	'types_test', 'types.cc',
	dependencies: [ doctest, ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('Core Types', types_test, suite: [ 'core', 'types' ])

if fuzzing_tests.allowed()

endif
