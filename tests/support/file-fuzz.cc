// SPDX-License-Identifier: BSD-3-Clause
/* file-fuzz.cc - file handling, fuzzing harness */

#include <cstdint>
#include <cstddef>
#include <string_view>

#include "panko/support/file.hh"
#include "panko/support/io/raw_file.hh"

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t *data, std::size_t size) {
	/* We can't construct 0-sized `string_view`s */
	if (size == 0) {
		return -1;
	}

	const std::string_view str{reinterpret_cast<const char*>(data), size};
	auto res{Panko::support::open(str)};
	if (res.has_value()) {
		auto file{std::get<Panko::support::io::raw_file_t>(std::move(*res))};
		file.valid();
	}

	return 0;
}
