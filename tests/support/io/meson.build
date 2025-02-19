# SPDX-License-Identifier: BSD-3-Clause

raw_file_test = executable(
	'raw_file_test', 'raw_file.cc',
	dependencies: [ doctest ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('Raw File I/O', raw_file_test, suite: [ 'support', 'io' ])

if fuzzing_tests.allowed()
	raw_file_fuzz = executable(
		'raw_file_fuzz', 'raw_file-fuzz.cc',
		dependencies: [ ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-raw_file', command: raw_file_fuzz)
endif

subdir('compressed')
