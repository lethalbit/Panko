# SPDX-License-Identifier: BSD-3-Clause

qt_modules = [
	'Core', 'Gui', 'Widgets'
]

qt_deps = [
	dependency('qt6', modules: qt_modules, required: true),
]
if is_windows
    qt_deps += dependency('qt6', modules: ['Main'], required: true)
endif

panko_gui_srcs = files([
	'main.cc',
])

subdir('dialogs')
subdir('models')
subdir('widgets')
subdir('windows')

panko_gui = executable(
	'panko',
	[ panko_gui_srcs, gui_resources, gui_translations, ],
	implicit_include_directories: false,
	include_directories: [
		root_inc, include_directories('.'),
	],
	cpp_args: [
		global_defs,
	],
	gnu_symbol_visibility: 'inlineshidden',
	dependencies: [
		threads, qt_deps, kde_libs, spdlog
	],
	link_with: [
		libpanko,
	],
	install: true,
)
