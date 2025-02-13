# SPDX-License-Identifier: BSD-3-Clause

panko_dissector_dir = panko_data_dir / 'dissectors'

# Iterate over the enabled dissectors and subdir into them to add them to the build tree
enabled_dissectors = get_option('dissectors')
foreach dissector : enabled_dissectors
	subdir(dissector)
endforeach
