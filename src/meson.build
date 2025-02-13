# SPDX-License-Identifier: BSD-3-Clause

root_inc = include_directories('.')

global_defs = [ ]
# Core dissector engine
subdir('panko')

# Dissectors
subdir('dissectors')

if build_gui
	subdir('panko-gui')
endif

if build_cli
	subdir('panko-cli')
endif
