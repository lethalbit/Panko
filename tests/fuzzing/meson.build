# SPDX-License-Identifier: BSD-3-Clause

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
