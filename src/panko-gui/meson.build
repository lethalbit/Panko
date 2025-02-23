# SPDX-License-Identifier: BSD-3-Clause

kde_libs = []
if kde_integration.allowed()
	message('KDE Framework integration enabled')
	kde_libs += [
		kf6_core,
		kf6_crash,
		kf6_win,
	]
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
		threads, qt_dep, kde_libs, spdlog, tomlpp, cxxopts,
	],
	link_with: [
		libpanko,
	],
	install: true,
)
