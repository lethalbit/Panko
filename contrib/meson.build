# SPDX-License-Identifier: BSD-3-Clause

gen_test_data = find_program(
	'@0@/scripts/gen_test_data.sh'.format(meson.current_source_dir()),
	required: true
)

if build_gui
	subdir('xdg')
	subdir('resources')
endif
