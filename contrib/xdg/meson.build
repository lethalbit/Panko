# SPDX-License-Identifier: BSD-3-Clause

meta_config = configuration_data()

meta_config.set('BINARY_PATH', bin_dir / 'panko')

desktop_file = configure_file(
	configuration: meta_config,
	input: 'net.lethalbit.Panko.in',
	output: 'net.lethalbit.Panko.desktop'
)

install_data(desktop_file,              install_dir: XDG_APP_DIR)
install_data('net.lethalbit.Panko.xml', install_dir: XDG_MIME_DIR / 'packages')
