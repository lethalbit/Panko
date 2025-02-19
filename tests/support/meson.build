# SPDX-License-Identifier: BSD-3-Clause

file_test = executable(
	'file_test', [
		'file.cc',
		'@0@/src/panko/support/file.cc'.format(meson.project_source_root()),
	],
	dependencies: [
		doctest, bzip2, zlib, lz4, liblzma, zstd
	],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('File Detection I/O', file_test, suite: [ 'support', 'io' ])

sys_test = executable(
	'sys_test', [
		'sys.cc',
		'@0@/src/panko/support/sys.cc'.format(meson.project_source_root()),
	],
	dependencies: [ doctest, ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('System Support', sys_test, suite: [ 'support', 'sys' ])

leb128_test = executable(
	'leb128_test', 'leb128.cc',
	dependencies: [ doctest, ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('LEB128 Encoding / Decoding', leb128_test, suite: [ 'support', 'leb128' ])

if fuzzing_tests.allowed()
	file_fuzz = executable(
		'file_fuzz', [
			'file-fuzz.cc',
			'@0@/src/panko/support/file.cc'.format(meson.project_source_root()),
		],
		dependencies: [ ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-file', command: file_fuzz)

	sys_fuzz = executable(
		'sys_fuzz', [
			'sys-fuzz.cc',
			'@0@/src/panko/support/sys.cc'.format(meson.project_source_root()),
		],
		dependencies: [ ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-sys', command: sys_fuzz)
endif

subdir('io')
