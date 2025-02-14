// SPDX-License-Identifier: BSD-3-Clause
/* frontend.hh - Some helpers for the CLI/GUI frontends, mostly for info printing */
#pragma once
#if !defined(PANKO_INTERNAL_FRONTEND_HH)
#define PANKO_INTERNAL_FRONTEND_HH

#include <string_view>

#include "panko/internal/defs.hh"

namespace Panko::internal {

	[[nodiscard]]
	PANKO_API std::string_view python_version() noexcept;
	[[nodiscard]]
	PANKO_API std::string_view lua_version() noexcept;
}

#endif /* PANKO_INTERNAL_FRONTEND_HH */
