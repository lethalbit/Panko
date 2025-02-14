# SPDX-License-Identifier: BSD-3-Clause

test_cxx_args = [
	'-DPANKO_TEST_DATA_DIR="@0@"'.format(meson.project_source_root() / 'tests' / 'test-data')
]
test_link_args = [ ]
test_overrides = [ ]

if fuzzing_tests
	protobuf         = dependency('protobuf', required: false, version: '>=25.2')
	protobuf_mutator = dependency('protobuf-mutator', required: false, version: '>=1.4')

	if not protobuf.found()
		message('Did not find local protobuf install, bundling')
		protobuf_wrap = subproject('protobuf', default_options: [])
		protobuf = protobuf_wrap.dependency('protobuf_dep')
	endif


	if not protobuf_mutator.found()
		message('Did not find local protobuf-mutator install, bundling')
		protobuf_mutator_wrap = subproject('protobuf-mutator', default_options: [])
		protobuf_mutator = protobuf_mutator_wrap.get_variable('protobuf_mutator_dep')
	endif

	test_fuzz_cxx_args = [
		'-fsanitize=address,undefined,fuzzer',
	]
	test_fuzz_link_args = [
		'-fsanitize=address,undefined,fuzzer',
	]
	test_overrides += [
		'b_lundef=false',
	]

endif

subdir('test-data')

subdir('capture')
subdir('core')
subdir('dissectors')
subdir('filter')
subdir('internal')
subdir('support')
