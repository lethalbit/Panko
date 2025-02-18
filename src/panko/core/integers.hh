// SPDX-License-Identifier: BSD-3-Clause
/* integers.hh - "newtype" definitions for odd-sized integers */

#pragma once
#if !defined(PANKO_CORE_INTEGERS_HH)
#define PANKO_CORE_INTEGERS_HH

#include <cstdint>
#include <compare>
#include <type_traits>
#include <limits>

#include "panko/internal/defs.hh"
#include "panko/core/types.hh"

namespace Panko::core {
	namespace integers {
		using Panko::core::types::is_any;

		template<typename T, std::size_t _bits>
		struct intN_t final {
			public:
				using v_type  = T;
				using vu_type = std::make_unsigned_t<v_type>;
				static constexpr std::size_t bits{_bits};
				static constexpr vu_type computed_mask{(vu_type(1U) << vu_type(bits)) - vu_type(1U)};
			private:
				T _backing;

				template<typename U>
				constexpr U sign_extend() const noexcept {
					using uu_type = std::make_unsigned_t<U>;
					const auto sign{static_cast<vu_type>(_backing) & (vu_type(1U) << (bits - 1U))};
					if (sign) {
						/* BUG(aki): We *should* be doing conversion properly here, but GCC is complaining :/ */
						PANKO_DIAGNOSTICS_PUSH()
						PANKO_DIAGNOSTICS_IGNORE("-Wsign-conversion")
						/* Fill the upper N bits with 1's to smear the sign bit over */
						const auto extension_bits{static_cast<U>(~static_cast<uu_type>(0U) & ~computed_mask)};
						return extension_bits | static_cast<U>(_backing);
						PANKO_DIAGNOSTICS_POP()
					}
					return static_cast<U>(_backing);
				}

			public:
				constexpr intN_t() noexcept = default;

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				constexpr intN_t(const U value) noexcept :
					_backing{static_cast<T>(value)}
				{ }

				template<typename U, std::size_t value_bits>
				constexpr intN_t(const intN_t<U, value_bits> value) noexcept :
					_backing{static_cast<T>(value._backing)}
				{ }

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				constexpr intN_t& operator=(const U value) noexcept {
					_backing = static_cast<T>(value);
					return *this;
				}

				template<typename U, std::size_t value_bits>
				constexpr intN_t& operator=(const intN_t<U, value_bits> value) noexcept {
					_backing = static_cast<T>(value._backing);
					return *this;
				}

				template<typename U = T, typename = std::enable_if_t<std::is_signed_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr operator std::int8_t() const noexcept {
					return sign_extend<std::int8_t>();
				}

				template<typename U = T, typename = std::enable_if_t<std::is_unsigned_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr operator std::uint8_t() const noexcept {
					return static_cast<std::uint8_t>(_backing);
				}

				template<typename U = T, typename = std::enable_if_t<std::is_signed_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr operator std::int16_t() const noexcept {
					return sign_extend<std::int16_t>();
				}

				template<typename U = T, typename = std::enable_if_t<std::is_unsigned_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr operator std::uint16_t() const noexcept {
					return static_cast<std::uint16_t>(_backing);
				}

				template<typename U = T, typename = std::enable_if_t<std::is_signed_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr operator std::int32_t() const noexcept {
					return sign_extend<std::int32_t>();
				}

				template<typename U = T, typename = std::enable_if_t<std::is_unsigned_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr operator std::uint32_t() const noexcept {
					return static_cast<std::uint32_t>(_backing);
				}

				template<typename U = T, typename = std::enable_if_t<std::is_signed_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr operator std::int64_t() const noexcept {
					return sign_extend<std::int64_t>();
				}

				template<typename U = T, typename = std::enable_if_t<std::is_unsigned_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr operator std::uint64_t() const noexcept {
					return static_cast<std::uint64_t>(_backing);
				}

				/* operator+ */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator+(const intN_t<U, rhs_bits>& rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) + static_cast<vu_type>(rhs._backing)
					) & computed_mask)};
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator+(U rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) + static_cast<vu_type>(rhs)
					) & computed_mask)};
				}

				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator+() const noexcept {
					return {+_backing};
				}

				/* operator+= */
				template<typename U, std::size_t rhs_bits>
				[[gnu::always_inline]]
				constexpr intN_t& operator+=(const intN_t<U, rhs_bits>& rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) + static_cast<vu_type>(rhs._backing)
					) & computed_mask);
					return *this;
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[gnu::always_inline]]
				constexpr intN_t& operator+=(U rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) + static_cast<vu_type>(rhs)
					) & computed_mask);
					return *this;
				}

				/* operator- */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator-(const intN_t<U, rhs_bits>& rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) - static_cast<vu_type>(rhs._backing)
					) & computed_mask)};
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator-(U rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) - static_cast<vu_type>(rhs)
					) & computed_mask)};
				}

				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator-() const noexcept {
					return {-_backing};
				}

				/* operator-= */
				template<typename U, std::size_t rhs_bits>
				[[gnu::always_inline]]
				constexpr intN_t& operator-=(const intN_t<U, rhs_bits>& rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) - static_cast<vu_type>(rhs._backing)
					) & computed_mask);
					return *this;
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[gnu::always_inline]]
				constexpr intN_t& operator-=(U rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) - static_cast<vu_type>(rhs)
					) & computed_mask);
					return *this;
				}

				/* operator* */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator*(const intN_t<U, rhs_bits>& rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) * static_cast<vu_type>(rhs._backing)
					) & computed_mask)};
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator*(U rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) * static_cast<vu_type>(rhs)
					) & computed_mask)};
				}

				/* operator*= */
				template<typename U, std::size_t rhs_bits>
				[[gnu::always_inline]]
				constexpr intN_t& operator*=(const intN_t<U, rhs_bits>& rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) * static_cast<vu_type>(rhs._backing)
					) & computed_mask);
					return *this;
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[gnu::always_inline]]
				constexpr intN_t& operator*=(U rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) * static_cast<vu_type>(rhs)
					) & computed_mask);
					return *this;
				}

				/* operator/ */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator/(const intN_t<U, rhs_bits>& rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) / static_cast<vu_type>(rhs._backing)
					) & computed_mask)};
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator/(U rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) / static_cast<vu_type>(rhs)
					) & computed_mask)};
				}

				/* operator/= */
				template<typename U, std::size_t rhs_bits>
				[[gnu::always_inline]]
				constexpr intN_t& operator/=(const intN_t<U, rhs_bits>& rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) / static_cast<vu_type>(rhs._backing)
					) & computed_mask);
					return *this;
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[gnu::always_inline]]
				constexpr intN_t& operator/=(U rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) / static_cast<vu_type>(rhs)
					) & computed_mask);
					return *this;
				}

				/* operator% */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator%(const intN_t<U, rhs_bits>& rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) % static_cast<vu_type>(rhs._backing)
					) & computed_mask)};
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator%(U rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) % static_cast<vu_type>(rhs)
					) & computed_mask)};
				}

				/* operator%= */
				template<typename U, std::size_t rhs_bits>
				[[gnu::always_inline]]
				constexpr intN_t& operator%=(const intN_t<U, rhs_bits>& rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) % static_cast<vu_type>(rhs._backing)
					) & computed_mask);
					return *this;
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[gnu::always_inline]]
				constexpr intN_t& operator%=(U rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) % static_cast<vu_type>(rhs)
					) & computed_mask);
					return *this;
				}

				/* operator<< */
				template<typename U, typename = std::enable_if_t<std::is_unsigned_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator<<(U rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) << static_cast<std::uint8_t>(rhs)
					) & computed_mask)};
				}

				/* operator<<= */
				template<typename U, typename = std::enable_if_t<std::is_unsigned_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t& operator<<=(U rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) << static_cast<std::uint8_t>(rhs)
					) & computed_mask);
					return *this;
				}

				/* operator>> */
				template<typename U, typename = std::enable_if_t<std::is_unsigned_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator>>(U rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) >> static_cast<std::uint8_t>(rhs)
					) & computed_mask)};
				}

				/* operator>>= */
				template<typename U, typename = std::enable_if_t<std::is_unsigned_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t& operator>>=(U rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) >> static_cast<std::uint8_t>(rhs)
					) & computed_mask);
					return *this;
				}

				/* operator| */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator|(const intN_t<U, rhs_bits>& rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) | static_cast<vu_type>(rhs._backing)
					) & computed_mask)};
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator|(U rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) | static_cast<vu_type>(rhs)
					) & computed_mask)};
				}

				/* operator|= */
				template<typename U, std::size_t rhs_bits>
				[[gnu::always_inline]]
				constexpr intN_t& operator|=(const intN_t<U, rhs_bits>& rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) | static_cast<vu_type>(rhs._backing)
					) & computed_mask);
					return *this;
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[gnu::always_inline]]
				constexpr intN_t& operator|=(U rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) | static_cast<vu_type>(rhs)
					) & computed_mask);
					return *this;
				}

				/* operator& */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator&(const intN_t<U, rhs_bits>& rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) & static_cast<vu_type>(rhs._backing)
					) & computed_mask)};
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator&(U rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) & static_cast<vu_type>(rhs)
					) & computed_mask)};
				}

				/* operator&= */
				template<typename U, std::size_t rhs_bits>
				[[gnu::always_inline]]
				constexpr intN_t& operator&=(const intN_t<U, rhs_bits>& rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) & static_cast<vu_type>(rhs._backing)
					) & computed_mask);
					return *this;
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[gnu::always_inline]]
				constexpr intN_t& operator&=(U rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) & static_cast<vu_type>(rhs)
					) & computed_mask);
					return *this;
				}

				/* operator^ */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator^(const intN_t<U, rhs_bits>& rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) ^ static_cast<vu_type>(rhs._backing)
					) & computed_mask)};
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator^(U rhs) const noexcept {
					return {static_cast<T>((
						static_cast<vu_type>(_backing) ^ static_cast<vu_type>(rhs)
					) & computed_mask)};
				}

				/* operator^= */
				template<typename U, std::size_t rhs_bits>
				[[gnu::always_inline]]
				constexpr intN_t& operator^=(const intN_t<U, rhs_bits>& rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) ^ static_cast<vu_type>(rhs._backing)
					) & computed_mask);
					return *this;
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[gnu::always_inline]]
				constexpr intN_t& operator^=(U rhs) noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) ^ static_cast<vu_type>(rhs)
					) & computed_mask);
					return *this;
				}

				/* operator~ */
				[[nodiscard, gnu::always_inline]]
				constexpr intN_t operator~() const noexcept {
					return {static_cast<T>(~static_cast<vu_type>(_backing) & computed_mask)};
				}

				/* operator++ */
				[[gnu::always_inline]]
				constexpr intN_t& operator++() noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) + 1U
					) & computed_mask);
					return *this;
				}

				[[gnu::always_inline]]
				constexpr intN_t operator++(int) noexcept {
					auto ret{*this};
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) + 1U
					) & computed_mask);
					return ret;
				}

				/* operator-- */
				[[gnu::always_inline]]
				constexpr intN_t& operator--() noexcept {
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) - 1U
					) & computed_mask);
					return *this;
				}

				[[gnu::always_inline]]
				constexpr intN_t operator--(int) noexcept {
					auto ret{*this};
					_backing = static_cast<T>((
						static_cast<vu_type>(_backing) - 1U
					) & computed_mask);
					return ret;
				}

				/* operator<=> */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr std::strong_ordering operator<=>(const intN_t<U, rhs_bits>& rhs) const noexcept {
					if (_backing < static_cast<T>(rhs._backing)) {
						return std::strong_ordering::less;
					}

					if (_backing > static_cast<T>(rhs._backing)) {
						return std::strong_ordering::greater;
					}

					return std::strong_ordering::equal;
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr std::strong_ordering operator<=>(U rhs) const noexcept {
					const auto lhs{[&](){
						if constexpr (std::is_signed_v<T>) {
							return sign_extend<T>();
						} else {
							return _backing;
						}
					}()};

					if (lhs < static_cast<T>(rhs)) {
						return std::strong_ordering::less;
					}

					if (lhs > static_cast<T>(rhs)) {
						return std::strong_ordering::greater;
					}

					return std::strong_ordering::equal;
				}

				/* operator== */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr bool operator==(const intN_t<U, rhs_bits>& rhs) const noexcept {
					return _backing == static_cast<T>(rhs._backing);
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr bool operator==(U rhs) const noexcept {
					if constexpr (std::is_signed_v<T>) {
						return sign_extend<T>() == static_cast<T>(rhs);
					} else {
						return _backing == static_cast<T>(rhs);
					}
				}
					/* operator!= */
					/* operator< */
					/* operator> */
					/* operator<= */
					/* operator>= */


				/* operator! */
				[[nodiscard, gnu::always_inline]]
				constexpr bool operator!() const noexcept {
					return !_backing;
				}

				/* operator|| */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr bool operator||(const intN_t<U, rhs_bits>& rhs) const noexcept {
					return _backing || rhs._backing;
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr bool operator||(U rhs) const noexcept {
					return _backing || rhs;
				}

				[[nodiscard, gnu::always_inline]]
				constexpr bool operator||(bool rhs) const noexcept {
					return _backing || rhs;
				}

				/* operator&& */
				template<typename U, std::size_t rhs_bits>
				[[nodiscard, gnu::always_inline]]
				constexpr bool operator&&(const intN_t<U, rhs_bits>& rhs) const noexcept {
					return _backing && rhs._backing;
				};

				template<typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
				[[nodiscard, gnu::always_inline]]
				constexpr bool operator&&(U rhs) const noexcept {
					return _backing && rhs;
				}

				[[nodiscard, gnu::always_inline]]
				constexpr bool operator&&(bool rhs) const noexcept {
					return _backing && rhs;
				}
		};


		using int24_t = intN_t<std::int32_t, 24zu>;
		using uint24_t = intN_t<std::uint32_t, 24zu>;

		using int40_t = intN_t<std::int64_t, 40zu>;
		using uint40_t = intN_t<std::uint64_t, 40zu>;

		using int48_t = intN_t<std::int64_t, 48zu>;
		using uint48_t = intN_t<std::uint64_t, 48zu>;

		using int50_t = intN_t<std::int64_t, 50zu>;
		using uint50_t = intN_t<std::uint64_t, 50zu>;

		using int56_t = intN_t<std::int64_t, 56zu>;
		using uint56_t = intN_t<std::uint64_t, 56zu>;

		template<typename T>
		using is_intn = is_any<T, int24_t, uint24_t, int40_t, uint40_t, int48_t, uint48_t, int50_t, uint50_t, int56_t, uint56_t>;
		template<typename T>
		constexpr bool is_intn_v = is_intn<T>::value;

		namespace _impl {

			/* These are helpers for defining the `std::numeric_limits<>` appropriately  */
			template<typename T>
			[[nodiscard, gnu::always_inline]]
			constexpr std::size_t _sign_correction() noexcept {
				return std::size_t(T(-1) < T(0));
			}

			template<typename T>
			[[nodiscard, gnu::always_inline]]
			constexpr std::size_t _digits() noexcept {
				return (T::bits - _sign_correction<T>());
			}

			template<typename T>
			[[nodiscard, gnu::always_inline]]
			constexpr std::size_t _digits10() noexcept {
				/* The 643/2136 approximates log10 to 7 sig, +1 due to integer truncation */
				return std::size_t(_digits<T>() * 643zu / 2136zu) + 1zu;
			}

			template<typename T>
			[[nodiscard, gnu::always_inline]]
			constexpr T _max() noexcept {
				using v_type  = T::v_type;
				using vu_type = T::vu_type;
				if constexpr (_sign_correction<T>()) {
					return {v_type((vu_type(1U) << (T::bits - 1U)) - 1U)};
				} else {
					return ~T(0U);
				}
			}

			template<typename T>
			[[nodiscard, gnu::always_inline]]
			constexpr T _min() noexcept {
				if constexpr (_sign_correction<T>()) {
					return -_max<T>() - 1;
				} else {
					return T(0U);
				}
			}
		}
	}
	namespace types {
		template<typename T, std::size_t bits>
		struct promoted_type<Panko::core::integers::intN_t<T, bits>, std::is_unsigned_v<T>> {
			using type = promoted_type_t<T>;
		};
	}
}

/* std::numeric_limits specializations */
template<>
struct std::numeric_limits<Panko::core::integers::int24_t> {
	using _ty = Panko::core::integers::int24_t;

	static constexpr bool is_specialized{true};
	static constexpr bool is_signed{true};
	static constexpr bool is_integer{true};
	static constexpr bool is_exact{true};
	static constexpr bool is_iec559{false};
	static constexpr bool is_bounded{true};
	static constexpr bool is_modulo{false};

	static constexpr bool has_infinity{false};
	static constexpr bool has_quiet_NaN{false};
	static constexpr bool has_signaling_NaN{false};
	static constexpr bool has_denorm_loss{false};

	static constexpr std::float_denorm_style has_denorm{std::denorm_absent};
	static constexpr std::float_round_style round_style{std::round_toward_zero};

	static constexpr int digits{int(Panko::core::integers::_impl::_digits<_ty>())};
	static constexpr int digits10{int(Panko::core::integers::_impl::_digits10<_ty>())};
	static constexpr int max_digits10{0};
	static constexpr int radix{2};
	static constexpr int min_exponent{0};
	static constexpr int max_exponent{0};
	static constexpr int min_exponent10{0};
	static constexpr int max_exponent10{0};

	static constexpr bool traps{true};
	static constexpr bool tinyness_before{false};

	[[nodiscard]]
	static constexpr _ty min() noexcept { return Panko::core::integers::_impl::_min<_ty>(); }
	[[nodiscard]]
	static constexpr _ty lowest() noexcept { return min(); }
	[[nodiscard]]
	static constexpr _ty max() noexcept { return Panko::core::integers::_impl::_max<_ty>(); }
	[[nodiscard]]
	static constexpr _ty epsilon() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty round_error() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty infinity() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty quiet_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty signaling_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty denorm_min() noexcept { return _ty(0); }
};

template<>
struct std::numeric_limits<Panko::core::integers::uint24_t> {
	using _ty = Panko::core::integers::uint24_t;

	static constexpr bool is_specialized{true};
	static constexpr bool is_signed{false};
	static constexpr bool is_integer{true};
	static constexpr bool is_exact{true};
	static constexpr bool is_iec559{false};
	static constexpr bool is_bounded{true};
	static constexpr bool is_modulo{false};

	static constexpr bool has_infinity{false};
	static constexpr bool has_quiet_NaN{false};
	static constexpr bool has_signaling_NaN{false};
	static constexpr bool has_denorm_loss{false};

	static constexpr std::float_denorm_style has_denorm{std::denorm_absent};
	static constexpr std::float_round_style round_style{std::round_toward_zero};

	static constexpr int digits{int(Panko::core::integers::_impl::_digits<_ty>())};
	static constexpr int digits10{int(Panko::core::integers::_impl::_digits10<_ty>())};
	static constexpr int max_digits10{0};
	static constexpr int radix{2};
	static constexpr int min_exponent{0};
	static constexpr int max_exponent{0};
	static constexpr int min_exponent10{0};
	static constexpr int max_exponent10{0};

	static constexpr bool traps{true};
	static constexpr bool tinyness_before{false};

	[[nodiscard]]
	static constexpr _ty min() noexcept { return Panko::core::integers::_impl::_min<_ty>(); }
	[[nodiscard]]
	static constexpr _ty lowest() noexcept { return min(); }
	[[nodiscard]]
	static constexpr _ty max() noexcept { return Panko::core::integers::_impl::_max<_ty>(); }
	[[nodiscard]]
	static constexpr _ty epsilon() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty round_error() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty infinity() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty quiet_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty signaling_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty denorm_min() noexcept { return _ty(0); }
};

template<>
struct std::numeric_limits<Panko::core::integers::int40_t> {
	using _ty = Panko::core::integers::int40_t;

	static constexpr bool is_specialized{true};
	static constexpr bool is_signed{true};
	static constexpr bool is_integer{true};
	static constexpr bool is_exact{true};
	static constexpr bool is_iec559{false};
	static constexpr bool is_bounded{true};
	static constexpr bool is_modulo{false};

	static constexpr bool has_infinity{false};
	static constexpr bool has_quiet_NaN{false};
	static constexpr bool has_signaling_NaN{false};
	static constexpr bool has_denorm_loss{false};

	static constexpr std::float_denorm_style has_denorm{std::denorm_absent};
	static constexpr std::float_round_style round_style{std::round_toward_zero};

	static constexpr int digits{int(Panko::core::integers::_impl::_digits<_ty>())};
	static constexpr int digits10{int(Panko::core::integers::_impl::_digits10<_ty>())};
	static constexpr int max_digits10{0};
	static constexpr int radix{2};
	static constexpr int min_exponent{0};
	static constexpr int max_exponent{0};
	static constexpr int min_exponent10{0};
	static constexpr int max_exponent10{0};

	static constexpr bool traps{true};
	static constexpr bool tinyness_before{false};

	[[nodiscard]]
	static constexpr _ty min() noexcept { return Panko::core::integers::_impl::_min<_ty>(); }
	[[nodiscard]]
	static constexpr _ty lowest() noexcept { return min(); }
	[[nodiscard]]
	static constexpr _ty max() noexcept { return Panko::core::integers::_impl::_max<_ty>(); }
	[[nodiscard]]
	static constexpr _ty epsilon() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty round_error() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty infinity() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty quiet_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty signaling_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty denorm_min() noexcept { return _ty(0); }
};

template<>
struct std::numeric_limits<Panko::core::integers::uint40_t> {
	using _ty = Panko::core::integers::uint40_t;

	static constexpr bool is_specialized{true};
	static constexpr bool is_signed{false};
	static constexpr bool is_integer{true};
	static constexpr bool is_exact{true};
	static constexpr bool is_iec559{false};
	static constexpr bool is_bounded{true};
	static constexpr bool is_modulo{false};

	static constexpr bool has_infinity{false};
	static constexpr bool has_quiet_NaN{false};
	static constexpr bool has_signaling_NaN{false};
	static constexpr bool has_denorm_loss{false};

	static constexpr std::float_denorm_style has_denorm{std::denorm_absent};
	static constexpr std::float_round_style round_style{std::round_toward_zero};

	static constexpr int digits{int(Panko::core::integers::_impl::_digits<_ty>())};
	static constexpr int digits10{int(Panko::core::integers::_impl::_digits10<_ty>())};
	static constexpr int max_digits10{0};
	static constexpr int radix{2};
	static constexpr int min_exponent{0};
	static constexpr int max_exponent{0};
	static constexpr int min_exponent10{0};
	static constexpr int max_exponent10{0};

	static constexpr bool traps{true};
	static constexpr bool tinyness_before{false};

	[[nodiscard]]
	static constexpr _ty min() noexcept { return Panko::core::integers::_impl::_min<_ty>(); }
	[[nodiscard]]
	static constexpr _ty lowest() noexcept { return min(); }
	[[nodiscard]]
	static constexpr _ty max() noexcept { return Panko::core::integers::_impl::_max<_ty>(); }
	[[nodiscard]]
	static constexpr _ty epsilon() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty round_error() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty infinity() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty quiet_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty signaling_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty denorm_min() noexcept { return _ty(0); }
};

template<>
struct std::numeric_limits<Panko::core::integers::int48_t> {
	using _ty = Panko::core::integers::int48_t;

	static constexpr bool is_specialized{true};
	static constexpr bool is_signed{true};
	static constexpr bool is_integer{true};
	static constexpr bool is_exact{true};
	static constexpr bool is_iec559{false};
	static constexpr bool is_bounded{true};
	static constexpr bool is_modulo{false};

	static constexpr bool has_infinity{false};
	static constexpr bool has_quiet_NaN{false};
	static constexpr bool has_signaling_NaN{false};
	static constexpr bool has_denorm_loss{false};

	static constexpr std::float_denorm_style has_denorm{std::denorm_absent};
	static constexpr std::float_round_style round_style{std::round_toward_zero};

	static constexpr int digits{int(Panko::core::integers::_impl::_digits<_ty>())};
	static constexpr int digits10{int(Panko::core::integers::_impl::_digits10<_ty>())};
	static constexpr int max_digits10{0};
	static constexpr int radix{2};
	static constexpr int min_exponent{0};
	static constexpr int max_exponent{0};
	static constexpr int min_exponent10{0};
	static constexpr int max_exponent10{0};

	static constexpr bool traps{true};
	static constexpr bool tinyness_before{false};

	[[nodiscard]]
	static constexpr _ty min() noexcept { return Panko::core::integers::_impl::_min<_ty>(); }
	[[nodiscard]]
	static constexpr _ty lowest() noexcept { return min(); }
	[[nodiscard]]
	static constexpr _ty max() noexcept { return Panko::core::integers::_impl::_max<_ty>(); }
	[[nodiscard]]
	static constexpr _ty epsilon() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty round_error() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty infinity() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty quiet_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty signaling_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty denorm_min() noexcept { return _ty(0); }
};

template<>
struct std::numeric_limits<Panko::core::integers::uint48_t> {
	using _ty = Panko::core::integers::uint48_t;

	static constexpr bool is_specialized{true};
	static constexpr bool is_signed{false};
	static constexpr bool is_integer{true};
	static constexpr bool is_exact{true};
	static constexpr bool is_iec559{false};
	static constexpr bool is_bounded{true};
	static constexpr bool is_modulo{false};

	static constexpr bool has_infinity{false};
	static constexpr bool has_quiet_NaN{false};
	static constexpr bool has_signaling_NaN{false};
	static constexpr bool has_denorm_loss{false};

	static constexpr std::float_denorm_style has_denorm{std::denorm_absent};
	static constexpr std::float_round_style round_style{std::round_toward_zero};

	static constexpr int digits{int(Panko::core::integers::_impl::_digits<_ty>())};
	static constexpr int digits10{int(Panko::core::integers::_impl::_digits10<_ty>())};
	static constexpr int max_digits10{0};
	static constexpr int radix{2};
	static constexpr int min_exponent{0};
	static constexpr int max_exponent{0};
	static constexpr int min_exponent10{0};
	static constexpr int max_exponent10{0};

	static constexpr bool traps{true};
	static constexpr bool tinyness_before{false};

	[[nodiscard]]
	static constexpr _ty min() noexcept { return Panko::core::integers::_impl::_min<_ty>(); }
	[[nodiscard]]
	static constexpr _ty lowest() noexcept { return min(); }
	[[nodiscard]]
	static constexpr _ty max() noexcept { return Panko::core::integers::_impl::_max<_ty>(); }
	[[nodiscard]]
	static constexpr _ty epsilon() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty round_error() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty infinity() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty quiet_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty signaling_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty denorm_min() noexcept { return _ty(0); }
};

template<>
struct std::numeric_limits<Panko::core::integers::int50_t> {
	using _ty = Panko::core::integers::int50_t;

	static constexpr bool is_specialized{true};
	static constexpr bool is_signed{true};
	static constexpr bool is_integer{true};
	static constexpr bool is_exact{true};
	static constexpr bool is_iec559{false};
	static constexpr bool is_bounded{true};
	static constexpr bool is_modulo{false};

	static constexpr bool has_infinity{false};
	static constexpr bool has_quiet_NaN{false};
	static constexpr bool has_signaling_NaN{false};
	static constexpr bool has_denorm_loss{false};

	static constexpr std::float_denorm_style has_denorm{std::denorm_absent};
	static constexpr std::float_round_style round_style{std::round_toward_zero};

	static constexpr int digits{int(Panko::core::integers::_impl::_digits<_ty>())};
	static constexpr int digits10{int(Panko::core::integers::_impl::_digits10<_ty>())};
	static constexpr int max_digits10{0};
	static constexpr int radix{2};
	static constexpr int min_exponent{0};
	static constexpr int max_exponent{0};
	static constexpr int min_exponent10{0};
	static constexpr int max_exponent10{0};

	static constexpr bool traps{true};
	static constexpr bool tinyness_before{false};

	[[nodiscard]]
	static constexpr _ty min() noexcept { return Panko::core::integers::_impl::_min<_ty>(); }
	[[nodiscard]]
	static constexpr _ty lowest() noexcept { return min(); }
	[[nodiscard]]
	static constexpr _ty max() noexcept { return Panko::core::integers::_impl::_max<_ty>(); }
	[[nodiscard]]
	static constexpr _ty epsilon() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty round_error() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty infinity() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty quiet_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty signaling_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty denorm_min() noexcept { return _ty(0); }
};

template<>
struct std::numeric_limits<Panko::core::integers::uint50_t> {
	using _ty = Panko::core::integers::uint50_t;

	static constexpr bool is_specialized{true};
	static constexpr bool is_signed{false};
	static constexpr bool is_integer{true};
	static constexpr bool is_exact{true};
	static constexpr bool is_iec559{false};
	static constexpr bool is_bounded{true};
	static constexpr bool is_modulo{false};

	static constexpr bool has_infinity{false};
	static constexpr bool has_quiet_NaN{false};
	static constexpr bool has_signaling_NaN{false};
	static constexpr bool has_denorm_loss{false};

	static constexpr std::float_denorm_style has_denorm{std::denorm_absent};
	static constexpr std::float_round_style round_style{std::round_toward_zero};

	static constexpr int digits{int(Panko::core::integers::_impl::_digits<_ty>())};
	static constexpr int digits10{int(Panko::core::integers::_impl::_digits10<_ty>())};
	static constexpr int max_digits10{0};
	static constexpr int radix{2};
	static constexpr int min_exponent{0};
	static constexpr int max_exponent{0};
	static constexpr int min_exponent10{0};
	static constexpr int max_exponent10{0};

	static constexpr bool traps{true};
	static constexpr bool tinyness_before{false};

	[[nodiscard]]
	static constexpr _ty min() noexcept { return Panko::core::integers::_impl::_min<_ty>(); }
	[[nodiscard]]
	static constexpr _ty lowest() noexcept { return min(); }
	[[nodiscard]]
	static constexpr _ty max() noexcept { return Panko::core::integers::_impl::_max<_ty>(); }
	[[nodiscard]]
	static constexpr _ty epsilon() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty round_error() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty infinity() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty quiet_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty signaling_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty denorm_min() noexcept { return _ty(0); }
};

template<>
struct std::numeric_limits<Panko::core::integers::int56_t> {
	using _ty = Panko::core::integers::int56_t;

	static constexpr bool is_specialized{true};
	static constexpr bool is_signed{true};
	static constexpr bool is_integer{true};
	static constexpr bool is_exact{true};
	static constexpr bool is_iec559{false};
	static constexpr bool is_bounded{true};
	static constexpr bool is_modulo{false};

	static constexpr bool has_infinity{false};
	static constexpr bool has_quiet_NaN{false};
	static constexpr bool has_signaling_NaN{false};
	static constexpr bool has_denorm_loss{false};

	static constexpr std::float_denorm_style has_denorm{std::denorm_absent};
	static constexpr std::float_round_style round_style{std::round_toward_zero};

	static constexpr int digits{int(Panko::core::integers::_impl::_digits<_ty>())};
	static constexpr int digits10{int(Panko::core::integers::_impl::_digits10<_ty>())};
	static constexpr int max_digits10{0};
	static constexpr int radix{2};
	static constexpr int min_exponent{0};
	static constexpr int max_exponent{0};
	static constexpr int min_exponent10{0};
	static constexpr int max_exponent10{0};

	static constexpr bool traps{true};
	static constexpr bool tinyness_before{false};

	[[nodiscard]]
	static constexpr _ty min() noexcept { return Panko::core::integers::_impl::_min<_ty>(); }
	[[nodiscard]]
	static constexpr _ty lowest() noexcept { return min(); }
	[[nodiscard]]
	static constexpr _ty max() noexcept { return Panko::core::integers::_impl::_max<_ty>(); }
	[[nodiscard]]
	static constexpr _ty epsilon() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty round_error() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty infinity() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty quiet_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty signaling_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty denorm_min() noexcept { return _ty(0); }
};

template<>
struct std::numeric_limits<Panko::core::integers::uint56_t> {
	using _ty = Panko::core::integers::uint56_t;

	static constexpr bool is_specialized{true};
	static constexpr bool is_signed{false};
	static constexpr bool is_integer{true};
	static constexpr bool is_exact{true};
	static constexpr bool is_iec559{false};
	static constexpr bool is_bounded{true};
	static constexpr bool is_modulo{false};

	static constexpr bool has_infinity{false};
	static constexpr bool has_quiet_NaN{false};
	static constexpr bool has_signaling_NaN{false};
	static constexpr bool has_denorm_loss{false};

	static constexpr std::float_denorm_style has_denorm{std::denorm_absent};
	static constexpr std::float_round_style round_style{std::round_toward_zero};

	static constexpr int digits{int(Panko::core::integers::_impl::_digits<_ty>())};
	static constexpr int digits10{int(Panko::core::integers::_impl::_digits10<_ty>())};
	static constexpr int max_digits10{0};
	static constexpr int radix{2};
	static constexpr int min_exponent{0};
	static constexpr int max_exponent{0};
	static constexpr int min_exponent10{0};
	static constexpr int max_exponent10{0};

	static constexpr bool traps{true};
	static constexpr bool tinyness_before{false};

	[[nodiscard]]
	static constexpr _ty min() noexcept { return Panko::core::integers::_impl::_min<_ty>(); }
	[[nodiscard]]
	static constexpr _ty lowest() noexcept { return min(); }
	[[nodiscard]]
	static constexpr _ty max() noexcept { return Panko::core::integers::_impl::_max<_ty>(); }
	[[nodiscard]]
	static constexpr _ty epsilon() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty round_error() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty infinity() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty quiet_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty signaling_NaN() noexcept { return _ty(0); }
	[[nodiscard]]
	static constexpr _ty denorm_min() noexcept { return _ty(0); }
};

#endif /* PANKO_CORE_INTEGERS_HH */
