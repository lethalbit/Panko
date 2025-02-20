// SPDX-License-Identifier: BSD-3-Clause
/* bits.hh - bitflags/bitfields and related */

#pragma once
#if !defined(PANKO_CORE_BITS_HH)
#define PANKO_CORE_BITS_HH

#include <atomic>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

#include "panko/internal/defs.hh"
#include "panko/core/types.hh"

namespace Panko::core {
	using Panko::core::types::is_atomic_integral_v;

	template<std::size_t _lsb, std::size_t _msb>
	struct bits_t final {
		static_assert(_lsb <= _msb, "bits pan LSB must be smaller than or equal to its MSB");

		static constexpr auto msb{_msb};
		static constexpr auto lsb{_lsb};
		static constexpr auto size{(_msb - _lsb) + 1zu};
	};

	template<std::size_t idx>
	using bit_t = bits_t<idx, idx>;

	namespace {
		template<std::size_t, typename...>
		struct type_at_index_t;

		template<std::size_t N, typename T, typename... U>
		struct type_at_index_t<N, T, U...> {
			using type = type_at_index_t<N - 1zu, U...>::type;
		};

		template<typename T, typename... U>
		struct type_at_index_t<0zu, T, U...> {
			using type = T;
		};

		template<std::size_t bits>
		using smallest_type_for = std::tuple_element_t<
			((bits + 7U) / 8U) - 1U, std::tuple<
			std::uint8_t,  /* <= 1 byte  */
			std::uint16_t, /* <= 2 bytes */
			std::uint32_t, /* <= 3 bytes */
			std::uint32_t, /* <= 4 bytes */
			std::uint64_t, /* <= 5 bytes */
			std::uint64_t, /* <= 6 bytes */
			std::uint64_t, /* <= 7 bytes */
			std::uint64_t  /* <= 8 bytes */
		>>;
	}

	template<typename T, typename E, typename... B>
	struct bitfield_t final {
		static_assert(std::is_enum_v<E>, "Key type to bitfield_t should be an enum");
		static_assert(is_atomic_integral_v<T>, "bitfield_t must be backed by an integral type");

		using vtype   = T;
		using vutype  = std::make_unsigned_t<vtype>;
		using ktype   = E;
		using kutype  = std::underlying_type_t<E>;
		using kuutype = std::make_unsigned_t<kutype>;

		static constexpr auto width{std::numeric_limits<T>::digits};
		static constexpr std::size_t field_count{sizeof... (B)};

	private:
		vutype _backing;

		template<std::size_t idx>
		using field = type_at_index_t<idx, B...>::type;

	public:
		constexpr bitfield_t() noexcept = default;
		constexpr bitfield_t(const bitfield_t& flags) noexcept :
			_backing{flags._backing}
		{ }
		constexpr bitfield_t(const T value) noexcept :
			_backing{value}
		{ }

		constexpr bitfield_t& operator=(const bitfield_t& flags) noexcept {
			if (&flags != this) {
				_backing = flags._backing;
			}
			return *this;
		}

		constexpr bitfield_t& operator=(const T value) noexcept {
			_backing = value;
			return *this;
		}

		template<E key>
		[[nodiscard, gnu::always_inline]]
		constexpr inline auto get() noexcept {
			static_assert(static_cast<vutype>(key) < field_count, "field index out of range");

			using element = field<static_cast<vutype>(key)>;
			using ET      = smallest_type_for<element::size>;

			constexpr auto computed_mask{((vutype(1) << (element::msb + vutype(1)) - element::lsb) - vutype(1)) << element::lsb};

			return ET((_backing & computed_mask) >> element::lsb);
		}

		template<E key>
		[[gnu::always_inline]]
		constexpr inline std::enable_if_t<field<static_cast<std::size_t>(key)>::size == 1> set() noexcept {
			static_assert(static_cast<vutype>(key) < field_count, "field index out of range");

			using element = field<static_cast<vutype>(key)>;

			constexpr auto computed_mask{((vutype(1) << (element::msb + vutype(1)) - element::lsb) - vutype(1)) << element::lsb};

			/* If we are setting a 1-bit value, we can just or the mask */
			_backing |= computed_mask;
		}

		template<E key>
		[[gnu::always_inline]]
		constexpr inline std::enable_if_t<field<static_cast<std::size_t>(key)>::size != 1> set(const T value) noexcept {
			static_assert(static_cast<vutype>(key) < field_count, "field index out of range");

			using element = field<static_cast<vutype>(key)>;

			constexpr auto computed_mask{((vutype(1) << (element::msb + vutype(1)) - element::lsb) - vutype(1)) << element::lsb};
			_backing = vutype((_backing & ~computed_mask) | ((vutype(value) << element::lsb) & computed_mask));
		}


		template<E key>
		[[gnu::always_inline]]
		constexpr inline void clr() noexcept {
			static_assert(static_cast<vutype>(key) < field_count, "field index out of range");

			using element = field<static_cast<vutype>(key)>;

			constexpr auto computed_mask{((vutype(1) << (element::msb + vutype(1)) - element::lsb) - vutype(1)) << element::lsb};

			_backing &= vutype(~(computed_mask));
		}

		template<E key>
		[[gnu::always_inline]]
		constexpr inline void tgl() noexcept {
			static_assert(static_cast<vutype>(key) < field_count, "field index out of range");

			using element = field<static_cast<vutype>(key)>;

			constexpr auto computed_mask{((vutype(1) << (element::msb + vutype(1)) - element::lsb) - vutype(1)) << element::lsb};

			/* If it is a single bit, then we can just flip it */
			if constexpr (element::size == 1) {
				_backing ^= computed_mask;
			} else {
				/* otherwise, we need to extract the current value, invert, then stick it back */
				set<key>(~get<key>());
			}
		}

		[[nodiscard]]
		operator T() const noexcept {
			return _backing;
		}

		[[nodiscard, gnu::always_inline]]
		constexpr bool operator==(const bitfield_t& flags) const noexcept {
			return _backing == flags._backing;
		}

		[[nodiscard, gnu::always_inline]]
		constexpr bool operator!=(const bitfield_t& flags) const noexcept {
			return _backing != flags._backing;
		}

		[[nodiscard, gnu::always_inline]]
		constexpr bool operator==(const T value) const noexcept {
			return _backing == value;
		}

		[[nodiscard, gnu::always_inline]]
		constexpr bool operator!=(const T value) const noexcept {
			return _backing != value;
		}
	};

}

#endif /* PANKO_CORE_BITS_HH */
