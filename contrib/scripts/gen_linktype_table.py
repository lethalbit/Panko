#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-3-Clause

from urllib  import request
from pathlib import Path

IETF_LINKTYPE_URL = 'https://raw.githubusercontent.com/IETF-OPSAWG-WG/draft-ietf-opsawg-pcap/refs/heads/master/linktypes.csv'
PRIV_USE_BEG = 0xFDE9
PRIV_USE_END = 0xFFFF


def generate_enum(data, ranges):
	print('enum struct linktype_t : std::uint16_t {')
	for name, value, comment in data:
		# special case for the BSD Loopback
		if name == 'NULL':
			name = 'BSD_LOOPBACK'

		print(f'\t{name: <26} = 0x{value:04X}U, /*!< {comment} */')

		if len(ranges) > 0 and value > ranges[0][0]:
			print(f'\t{ranges.pop()[1]}')

	# Print reserved ranges
	for raw_idx in range(PRIV_USE_BEG, PRIV_USE_END + 1):
		priv_idx = raw_idx - PRIV_USE_BEG
		if raw_idx == PRIV_USE_BEG:
			print(f'\t{"PRIVATE_USE_BEG": <26} = 0x{raw_idx:04X}U, /*!< Beginning of private use range */')
		name = f'PRIVATE_USE_{priv_idx:03d}'
		print(f'\t{name: <26} = 0x{raw_idx:04X}U, /*!< Reserved for private use */')
		if raw_idx == PRIV_USE_END:
			print(f'\t{"PRIVATE_USE_END": <26} = 0x{raw_idx:04X}U, /*!< End of private use range */')
	print('};')


def generate_strs(data):
	print('const std::unordered_map<linktype_t, std::string_view> linktype_names{')
	for name, _, comment in data:
		# special case for the BSD Loopback
		if name == 'NULL':
			name = 'BSD_LOOPBACK'

		print(f'\t{{ linktype_t::{name: <26}, "{comment}"sv }},')

	for raw_idx in range(PRIV_USE_BEG, PRIV_USE_END + 1):
		priv_idx = raw_idx - PRIV_USE_BEG
		name = f'PRIVATE_USE_{priv_idx:03d}'
		print(f'\t{{ linktype_t::{name: <26}, "Reserved for private use #{priv_idx}"sv }},')

	print('};')


def main() -> int:
	def_file, _ = request.urlretrieve(IETF_LINKTYPE_URL)

	# Read records
	with Path(def_file).open('r') as f:
		data = f.readlines()
	# Split record on `|`
	data = map(lambda l: l.split('|'), data)
	# Strip whitespace from each component of each record
	data = map(lambda l: map(str.strip, l), data)
	# Filter out empty string
	data = map(lambda l: filter(lambda s: s != '', l), data)
	# De-quote
	data = map(lambda l: list(map(lambda s: s.replace('"', ''), l)), data)
	# Fixup linktype name and drop reference column
	data = list(map(lambda l: (l[0].removeprefix('LINKTYPE_'), l[1], l[2]), data))
	# Filter the range values
	ranges = filter(lambda l: '-' in l[1], data.copy())
	data   = filter(lambda l: '-' not in l[1], data)
	# Construct enum record lines
	data = list(map(lambda l: (l[0], int(l[1]), l[2]), data))
	# Build range records
	ranges = sorted(list(map(lambda r: (int(r[1].split('-')[0]), f'/* {r[0]}; {r[1]}; {r[2]} */'), ranges)))

	# Generate the enum
	generate_enum(data.copy(), ranges)

	print('')

	# Generate the string table
	generate_strs(data)

	return 0


if __name__ == '__main__':
	raise SystemExit(main())
