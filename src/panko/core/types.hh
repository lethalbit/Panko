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

	template<typename T>
	struct has_nullable_ctor final {
		template<typename U>
		static std::true_type _ctor(decltype((U(std::nullptr_t())))*);
		template<typename U>
		static std::false_type _ctor(...);

		static const bool value{std::is_same_v<decltype(_ctor<T>(nullptr)), std::true_type>};
	};
	template<typename T>
	constexpr bool has_nullable_ctor_v{has_nullable_ctor<T>::value};

	template<typename T, typename... Ts>
	struct is_any : std::disjunction<std::is_same<T, Ts>...> {};
	template<typename T, typename... Ts>
	constexpr bool is_any_v = is_any<T, Ts...>::value;

	template<typename T, bool = std::is_unsigned_v<T>>
	struct promoted_type;
	template<typename T>
	struct promoted_type<T, true> {
		using type = std::uint32_t;
	};
	template<typename T>
	struct promoted_type<T, false> {
		using type = std::int32_t;
	};
	template<>
	struct promoted_type<std::uint64_t, true> {
		using type = std::uint64_t;
	};
	template<>
	struct promoted_type<std::int64_t, false> {
		using type = std::int64_t;
	};
	template<>
	struct promoted_type<std::size_t, !std::is_same_v<std::uint64_t, std::size_t>> {
		using type = std::size_t;
	};
	template<>
	struct promoted_type<ssize_t, std::is_same_v<std::int64_t, ssize_t>> {
		using type = ssize_t;
	};
	template<typename T>
	using promoted_type_t = typename promoted_type<T>::type;

}

#endif /* PANKO_CORE_TYPES_HH */
