// SPDX-License-Identifier: BSD-3-Clause
/* sys.hh - System helpers */

#pragma once
#if !defined(PANKO_SUPPORT_SYS_HH)
#define PANKO_SUPPORT_SYS_HH

#include <string>
#include <string_view>
#include <optional>
#include <filesystem>

#include "panko/internal/defs.hh"

namespace Panko::support::sys {

	namespace fs = std::filesystem;

	[[nodiscard]]
	PANKO_API std::optional<std::string> getenv(const std::string_view& name);

	[[nodiscard]]
	PANKO_API fs::path get_home() noexcept;
	[[nodiscard]]
	PANKO_API fs::path expand_user(const std::string_view& path) noexcept;
}

#endif /* PANKO_SUPPORT_SYS_HH */
