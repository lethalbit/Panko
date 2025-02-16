# SPDX-License-Identifier: BSD-3-Clause

root_inc = include_directories('.')

global_defs = [ ]
# Core dissector engine
subdir('panko')

# Dissectors
subdir('dissectors')

# User-facing applications
if build_gui.allowed()
	subdir('panko-gui')
endif
if build_cli.allowed()
	subdir('panko-cli')
endif
