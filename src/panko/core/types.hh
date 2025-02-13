// SPDX-License-Identifier: BSD-3-Clause
/* types.hh - Type traits and definitions */

#pragma once
#if !defined(PANKO_CORE_TYPES_HH)
#define PANKO_CORE_TYPES_HH

#include <cstddef>
#include <type_traits>

#include <sys/types.h>

#include "panko/internal/defs.hh"


namespace Panko::core::types {
	using ssize_t = std::make_signed_t<std::size_t>;

	#if !defined(_WIN32)
		using mode_t = ::mode_t;
		using off_t = ::off_t;
	#else /* _WIN32 */
		using mode_t = std::int32_t;
		using off_t = std::int64_t;
	#endif
}

#endif /* PANKO_CORE_TYPES_HH */
