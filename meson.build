# SPDX-License-Identifier: BSD-3-Clause

project(
	'Panko',
	'cpp',
	default_options: [
		'buildtype=release',
		'cpp_std=c++23',
		'warning_level=3',
		'b_ndebug=if-release',
		'b_lto=true',
		'strip=false'
	],
	license: 'BSD-3-Clause',
	license_files: 'LICENSE',
	version: '0.1.0',
	meson_version: '>= 1.4.0',
	subproject_dir: 'deps'
)


cxx_std = get_option('cpp_std')
cxx     = meson.get_compiler('cpp')
if cxx_std not in ['c++23', 'gnu++23']
	error(f'Unsupported C++ Version @cxx_std@, must be c++23/gnu++23 or newer')
endif

# Not all C++23 compliant compilers ship with everything
contentious_headers = [
	'format',
	'print',
	'bit',
]

foreach header : contentious_headers
	if not cxx.has_header(header)
		error(f'You\'re compiler does not seem to have full C++23 support (missing header @header@')
		error('GCC version >= 14 or clang version >= 18 are required to build Panko')
	endif
endforeach


extended_warnings = [
	'-Wdouble-promotion',
	'-Wformat=2',
	'-Wformat-overflow=2',
	'-Wformat-signedness',
	'-Wformat-truncation',
	'-Wnull-dereference',
	'-Wmissing-attributes',
	'-Wmissing-braces',
	'-Wsequence-point',
	'-Werror=return-type',
	'-Wunused',
	'-Wunused-local-typedefs',
	'-Wunused-const-variable=2',
	'-Wmaybe-uninitialized',
	'-Wunknown-pragmas',
	'-Wstrict-aliasing',
	'-Wstrict-overflow=3',
	'-Wstring-compare',
	'-Wstringop-overflow',
	'-Warith-conversion',
	'-Wvla-parameter',
	'-Wduplicated-branches',
	'-Wshadow=local',
	'-Wunsafe-loop-optimizations',
	'-Wbad-function-cast',
	'-Wcast-qual',
	'-Wcast-align=strict',
	'-Wcast-function-type',
	'-Wconversion',
	'-Wdangling-else',
	'-Wsign-conversion',
	'-Wfloat-conversion',
	'-Wredundant-decls',
	'-Wvla',
	'-Wstack-protector',
	'-Wunsuffixed-float-constant',
	'-Wimplicit-fallthrough',
	'-Wxor-used-as-pow',
	'-Wself-move',
	'-Wdangling-reference',
	'-Werror=switch',
	'-Werror=return-type',
]

extra_flags = [ ]


pfx_dir           = get_option('prefix')
bin_dir           = pfx_dir / get_option('bindir')
lib_dir           = pfx_dir / get_option('libdir')
data_dir          = pfx_dir / get_option('datadir')
panko_data_dir    = data_dir / 'panko'

# XDG Directories
XDG_ICON_DIR     = data_dir / 'icons'/ 'hicolor'
XDG_APP_DIR      = data_dir / 'applications'
XDG_MIME_DIR     = data_dir / 'mime'

is_windows       = (target_machine.system() == 'windows')
is_macos         = (target_machine.system() == 'darwin')
is_linux         = (target_machine.system() == 'linux')
is_clang         = (cxx.get_id() == 'clang')
is_gcc           = (cxx.get_id() == 'gcc')
compiler_version = cxx.version()

tests_enabled    = get_option('enable_tests')
mtune            = get_option('mtune')
build_cli        = get_option('cli')
lua_backend      = get_option('lua_backend')
with_brotli      = get_option('with_brotli').allowed()
with_snappy      = get_option('with_snappy').allowed()

qt               = import('qt6')
fs               = import('fs')
pkgconfig        = import('pkgconfig')
py               = import('python')

if not is_linux
	warning('Platforms other than Linux are highly experimental, be warned')
endif

# Because this is added prior to add_project_arguments we don't need to gate it
# on compiler support
if mtune != 'default'
	extra_flags += f'-mtune=@mtune@'
endif

add_project_arguments(
	cxx.get_supported_arguments(extended_warnings),
	language: 'cpp'
)

add_project_arguments(
	cxx.get_supported_arguments(extra_flags),
	language: 'cpp'
)

git = find_program('git', required: false, native: true)
py  = py.find_installation(
	'python3',
	required: true,
	modules: []
)

qt_modules = [ 'Core', 'Gui', 'Widgets' ]
# If we are building the GUI on Windows, we will use Qt to stub the `WinMain` garbage
if is_windows
	qt_modules += [ 'Main' ]
endif

# Core deps
threads   = dependency('threads', required: true)
spdlog    = dependency('spdlog',  required: false, version: '>=1.15.1')
# Compressed PCAP/PCAPNG Reading
bzip2     = dependency('bzip2',   required: false, version: '>=1.0.8'  )
liblzma   = dependency('liblzma', required: false, version: '>=5.2.12' )
lz4       = dependency('liblz4',  required: false, version: '>=1.9.4'  )
zlib      = dependency('zlib',    required: false, version: '>=1.2.12' )
zstd      = dependency('zstd',    required: false, version: '>=1.4.5', modules: ['zstd::libzstd_shared'])
# Dissector Bindings
pybind11  = dependency('pybind11', required: false, version: '>=2.13.5' )
lua       = dependency('lua',      required: false, version: '>=5.4.6' )
luajit    = dependency('luajit',   required: false, version: '>=2.1' )
# Optional/Feature deps for core
brotli    = dependency('libbrotlidec',  required: false, version: '>=1.1.0')
snappy    = dependency('snappy',        required: false, version: '>=1.1.0')
# GUI/CLI Common
cxxopts   = dependency('cxxopts', required: false, version: '>=3.2.0')
# GUI
qt_dep    = dependency('qt6',      modules: qt_modules, required: false)
# GUI - KDE Integration
kf6_crash = dependency('KF6Crash',          method: 'cmake', modules: ['KF6::Crash'       ], required: false)
kf6_core  = dependency('KF6CoreAddons',     method: 'cmake', modules: ['KF6::CoreAddons'  ], required: false)
kf6_win   = dependency('KF6WindowSystem',   method: 'cmake', modules: ['KF6::WindowSystem'], required: false)
# GUI/CLI
tomlpp    = dependency('tomlplusplus', required: false, version: '>=3.4.0')
# Test
doctest   = dependency('doctest', required: false, version: '>=2.4.11')

if not spdlog.found()
	message('Did not find local spdlog install, bundling')
	spdlog_wrap = subproject('spdlog', default_options: [])
	spdlog = spdlog_wrap.get_variable('spdlog_dep')
endif

if not bzip2.found()
	message('Did not find local bzip2 install, bundling')
	bzip2_wrap = subproject('bzip2', default_options: [])
	bzip2 = dependency('bzip2')
endif

if not liblzma.found()
	message('Did not find local liblzma install, bundling')
	liblzma_wrap = subproject('liblzma', default_options: [])
	liblzma = liblzma_wrap.get_variable('lzma_dep')
endif

if not lz4.found()
	message('Did not find local liblz4 install, bundling')
	lz4_wrap = subproject('lz4', default_options: [])
	lz4 = lz4_wrap.get_variable('liblz4_dep')
endif

if not zlib.found()
	message('Did not find local zlib install, bundling')
	zlib_wrap = subproject('zlib', default_options: [])
	zlib = zlib_wrap.get_variable('zlib_dep')
endif

if not zstd.found()
	message('Did not find local zstd install, bundling')
	zstd_wrap = subproject('zstd', default_options: [])
	zstd = zstd_wrap.get_variable('libzstd_dep')
endif

if not pybind11.found()
	message('Did not find local pybind11 install, bundling')
	pybind11_wrap = subproject('pybind11', default_options: [])
	pybind11 = pybind11_wrap.get_variable('pybind11_dep')
endif

if lua_backend == 'lua'
	if not lua.found()
		message('Did not find local lua install, bundling')
		lua_wrap = subproject('lua', default_options: [])
		lua = lua_wrap.get_variable('lua_dep')
	endif
	lua_dep = lua
elif lua_backend == 'luajit'
	if not luajit.found()
		message('Did not find local luajit install, bundling')
		luajit_wrap = subproject('luajit', default_options: [])
		luajit = luajit_wrap.get_variable('luajit')
	endif
	lua_dep = luajit
endif
message(f'Using lua backend: @lua_backend@')

if not tomlpp.found()
	message('Did not find local tomlplusplus install, bundling')
	tomlpp_wrap = subproject('tomlplusplus', default_options: [])
	tomlpp = dependency('tomlplusplus')
endif

if not doctest.found() and tests_enabled
	message('Did not find local doctest install, bundling')
	doctest_wrap = subproject('doctest', default_options: [])
	doctest = dependency('doctest')
endif

if with_brotli and not brotli.found()
	message('Did not find local brotli install and brotli was requested, bundling')
	brotli_wrap = subproject('google-brotli', default_options: [])
	brotli = dependency('libbrotlidec')
endif

if with_snappy and not snappy.found()
	message('Did not find local brotli install and brotli was requested, bundling')
	snappy_wrap = subproject('google-snappy', default_options: [])
	snappy = snappy_wrap.get_variable('snappy_dep')
endif

if not cxxopts.found()
	message('Did not find local cxxopts install, bundling')
	cxxopts_wrap = subproject('cxxopts', default_options: [])
	cxxopts = cxxopts_wrap.get_variable('cxxopts_dep')
endif

# GUI - Disable GUI if we don't have QT
build_gui = get_option('gui').require(
	qt_dep.found(),
	error_message: 'Building the Panko GUI requires Qt6'
)

# GUI - Disable KDE integration if the option is not explicitly enabaled and we can't find the deps
kde_integration = get_option('kde_integration').require(
	(kf6_core.found() and kf6_crash.found() and kf6_win.found()) and build_gui.allowed(),
	error_message: 'KDE Integrataion requires KDE Framework 6 and building the GUI to be enabled'
)

summary({
	'Python Version': py.language_version(),
	'pybind11 Version': pybind11.version(),
	'Lua Backend': lua_backend,
	'Lua Version': lua_dep.version(),
	'Has brotli support': with_brotli,
	'Has snappy support': with_snappy,
	'Catgirl': 'ニャ〜'
}, bool_yn: true, section: 'Dissector Engine')

if build_gui.allowed()
	summary({
		'KDE Integration': kde_integration.allowed(),
	}, bool_yn: true, section: 'GUI')
endif

subdir('contrib')
subdir('src')

if tests_enabled
	subdir('tests')
endif
