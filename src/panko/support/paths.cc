// SPDX-License-Identifier: BSD-3-Clause
/* paths.cc -  Path constants */

#include <filesystem>
#include <array>

#include "panko/support/paths.hh"

#include "panko/core/types.hh"
namespace Panko::support::paths {
	using Panko::core::types::make_array;
	namespace fs = std::filesystem;

	const static auto user_paths{make_array({
		/* If we are on windows we need to do some extra work */
		#if defined(_WIN32)
		CACHE_HOME,
		DATA_HOME,
		BIN_HOME,
		LIB_HOME,
		CONFIG_HOME,
		#endif

		CONFIG_DIR,
		CACHE_DIR,
		DATA_DIR_LOCAL,
		DISSECTORS_LOCAL,
		SO_DISSECTORS_LOCAL,
		PY_DISSECTORS_LOCAL,
		LUA_DISSECTORS_LOCAL,
	})};

	void initialize_dirs() noexcept {
		for(const auto& dir : user_paths) {
			if (!fs::exists(dir)) {
				fs::create_directories(dir);
			}
		}
	}
}
