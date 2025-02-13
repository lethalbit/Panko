// SPDX-License-Identifier: BSD-3-Clause
/* paths.cc -  Path constants */

#include <filesystem>
#include <array>

#include "panko/core/paths.hh"

namespace Panko::core::paths {
	namespace fs = std::filesystem;

	const static std::array<fs::path, 4> user_paths{{
		CONFIG_DIR,
		CACHE_DIR,
		DATA_DIR_LOCAL,
		DISSECTORS_LOCAL,
	}};

	void initialize_dirs() noexcept {
		for(const auto& dir : user_paths) {
			if (!fs::exists(dir)) {
				fs::create_directories(dir);
			}
		}
	}
}
