// SPDX-License-Identifier: BSD-3-Clause
/* types.hh - Type traits and definitions */

#pragma once
#if !defined(PANKO_CORE_TYPES_HH)
#define PANKO_CORE_TYPES_HH

#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

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

	inline namespace _impl {
		template<typename T, std::size_t N, std::size_t... idx>
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
		constexpr std::array<T, N> make_array(T (&&elems)[N], std::index_sequence<idx...>) noexcept {
			return {{elems[idx]...}};
		}
	}
	template<typename T, std::size_t N>
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
	constexpr std::array<T, N> make_array(T (&&elems)[N]) noexcept {
		return _impl::make_array(std::move(elems), std::make_index_sequence<N>{});
	}

	/* For std::variant matching */
	template<typename... Ts> struct match_t : Ts... { using Ts::operator()...; };
	template<typename... Ts> match_t(Ts...) -> match_t<Ts...>;
}

#endif /* PANKO_CORE_TYPES_HH */
