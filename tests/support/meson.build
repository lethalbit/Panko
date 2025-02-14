# SPDX-License-Identifier: BSD-3-Clause

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

raw_file_test = executable(
	'raw_file_test', 'raw_file.cc',
	dependencies: [ doctest ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('Raw File I/O', raw_file_test, suite: [ 'support', 'io' ])

bz2_file_test = executable(
	'bz2_file_test', 'bz2_file.cc',
	dependencies: [ doctest, bzip2 ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('bz2 File I/O', bz2_file_test, suite: [ 'support', 'io', 'compressed' ])

gzip_file_test = executable(
	'gzip_file_test', 'gzip_file.cc',
	dependencies: [ doctest, zlib ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('gzip File I/O', gzip_file_test, suite: [ 'support', 'io', 'compressed' ])

lz4_file_test = executable(
	'lz4_file_test', 'lz4_file.cc',
	dependencies: [ doctest, lz4 ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('lz4 File I/O', lz4_file_test, suite: [ 'support', 'io', 'compressed' ])

lzma_file_test = executable(
	'lzma_file_test', 'lzma_file.cc',
	dependencies: [ doctest, liblzma ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('LZMA File I/O', lzma_file_test, suite: [ 'support', 'io', 'compressed' ])

xz_file_test = executable(
	'xz_file_test', 'xz_file.cc',
	dependencies: [ doctest, liblzma ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('XZ File I/O', xz_file_test, suite: [ 'support', 'io', 'compressed' ])

zstd_file_test = executable(
	'zstd_file_test', 'zstd_file.cc',
	dependencies: [ doctest, zstd ],
	include_directories: [ root_inc ],
	cpp_args: test_cxx_args,
	link_args: test_link_args,
	override_options: test_overrides,
)
test('zstd File I/O', xz_file_test, suite: [ 'support', 'io', 'compressed' ])

if fuzzing_tests
	sys_fuzz = executable(
		'sys_fuzz', [
			'sys-fuzz.cc',
			'@0@/src/panko/support/sys.cc'.format(meson.project_source_root()),
		],
		dependencies: [ doctest, ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-sys', command: sys_fuzz)

	file_fuzz = executable(
		'file_fuzz', [
			'file-fuzz.cc',
			'@0@/src/panko/support/file.cc'.format(meson.project_source_root()),
		],
		dependencies: [ doctest, ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-file', command: file_fuzz)

	raw_file_fuzz = executable(
		'raw_file_fuzz', 'raw_file-fuzz.cc',
		dependencies: [ doctest, ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-raw_file', command: raw_file_fuzz)

	bz2_file_fuzz = executable(
		'bz2_file_fuzz', 'bz2_file-fuzz.cc',
		dependencies: [ doctest, bzip2, ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-bz2_file', command: bz2_file_fuzz)

	gzip_file_fuzz = executable(
		'gzip_file_fuzz', 'gzip_file-fuzz.cc',
		dependencies: [ doctest, zlib, ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-gzip_file', command: gzip_file_fuzz)

	lz4_file_fuzz = executable(
		'lz4_file_fuzz', 'lz4_file-fuzz.cc',
		dependencies: [ doctest, lz4, ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-lz4_file', command: lz4_file_fuzz)

	lzma_file_fuzz = executable(
		'lzma_file_fuzz', 'lzma_file-fuzz.cc',
		dependencies: [ doctest, liblzma, ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-lzma_file', command: lzma_file_fuzz)

	xz_file_fuzz = executable(
		'xz_file_fuzz', 'xz_file-fuzz.cc',
		dependencies: [ doctest, liblzma, ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-xz_file', command: xz_file_fuzz)

	zstd_file_fuzz = executable(
		'zstd_file_fuzz', 'zstd_file-fuzz.cc',
		dependencies: [ doctest, zstd, ],
		include_directories: [ root_inc ],
		cpp_args: [ test_cxx_args, test_fuzz_cxx_args ],
		link_args: [ test_link_args, test_fuzz_link_args ],
		override_options: test_overrides,
	)
	run_target('fuzzer-support-zstd_file', command: zstd_file_fuzz)
endif
