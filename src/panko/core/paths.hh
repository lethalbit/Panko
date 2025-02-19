// SPDX-License-Identifier: BSD-3-Clause
/* paths.hh - Path constants */

#pragma once
#if !defined(PANKO_CORE_PATHS_HH)
#define PANKO_CORE_PATHS_HH

#include <string_view>
#include <filesystem>

#include "panko/config.hh"
#include "panko/internal/defs.hh"
#include "panko/support/sys.hh"


// FIXME(aki): Currently these assume absolute paths, especially for
// the data dir path, this would likely break on non system-installed
// instances of Panko such as a "portable" binary package.
// It /might/ be fine, but needs proper testing and validation

// HACK(aki): The `cert-err58-cpp` lint is disabled because we can't really
// do anything about it, I know it's naughty but *shrug*
// NOLINTBEGIN(cert-err58-cpp)
namespace Panko::core::paths {
	using namespace std::literals::string_view_literals;

	namespace cfg = Panko::config;
	namespace fs  = std::filesystem;

	/* XDG Directories */
	namespace xdg {
		[[maybe_unused]]
		const static auto HOME{Panko::support::sys::get_home()};
		[[maybe_unused]]
		const static auto CACHE_HOME{HOME / ".cache"sv};
		[[maybe_unused]]
		const static auto DATA_HOME{HOME / ".local/share"sv};
		[[maybe_unused]]
		const static auto BIN_HOME{HOME / ".local/bin"sv};
		[[maybe_unused]]
		const static auto LIB_HOME{HOME / ".local/lib"sv};
		[[maybe_unused]]
		const static auto CONFIG_HOME{HOME / ".config"sv};
	}

	/* TODO(aki): We should probably do cross-platform specialization rather than cramming XDG into Win32/macOS */
	/* Panko Specific Directories */
	[[maybe_unused]]
	const static auto CONFIG_DIR{xdg::CONFIG_HOME / "panko"sv};
	[[maybe_unused]]
	const static auto CACHE_DIR{xdg::CACHE_HOME / "panko"sv};
	[[maybe_unused]]
	const static auto DATA_DIR_SYS{fs::path(cfg::data_dir)};
	[[maybe_unused]]
	const static auto DATA_DIR_LOCAL{xdg::DATA_HOME / "panko"sv};

	/* Dissector Paths */
	[[maybe_unused]]
	const static auto DISSECTORS_SYS{DATA_DIR_SYS / "dissectors"sv};
	[[maybe_unused]]
	const static auto DISSECTORS_LOCAL{DATA_DIR_LOCAL / "dissectors"sv};

	PANKO_API void initialize_dirs() noexcept;
}
// NOLINTEND(cert-err58-cpp)

#endif /* PANKO_CORE_PATHS_HH */
