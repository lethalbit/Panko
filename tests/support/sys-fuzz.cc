// SPDX-License-Identifier: BSD-3-Clause
/* sys-fuzz.cc - System helpers, fuzzing harness */

#include <cstdint>
#include <cstddef>
#include <string_view>

#include "panko/support/sys.hh"

using Panko::support::sys::expand_user;
using Panko::support::sys::getenv;

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t *data, std::size_t size) {
	/* We can't construct 0-sized `string_view`s */
	if (size == 0) {
		return -1;
	}

	const std::string_view str{reinterpret_cast<const char*>(data), size};

	[[maybe_unused]]
	auto _{expand_user(str)};
	[[maybe_unused]]
	auto _{getenv(str)};

	return 0;
}
