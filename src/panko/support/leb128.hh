// SPDX-License-Identifier: BSD-3-Clause
/* leb128.hh - LEB128 Encode/decode */

#pragma once
#if !defined(PANKO_SUPPORT_LEB128_HH)
#define PANKO_SUPPORT_LEB128_HH

#include <cstdint>
#include <vector>
#include <type_traits>

#include "panko/core/types.hh"

namespace Panko::support {

	template<typename T>
	[[nodiscard]]
	std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, std::vector<std::uint8_t>>
	leb128_encode(T num) noexcept {
		using U = std::make_unsigned_t<T>;
		using V = Panko::core::types::promoted_type_t<U>;

		std::vector<std::uint8_t> enc{};

		bool has_more{true};
		while (has_more) {
			auto byte{std::uint8_t(static_cast<V>(num) & 0x7FU)};
			num >>= 7U;

			if ((!num && !(byte & 0x40U)) || (num == -1 && (byte & 0x40U))) {
				has_more = false;
			} else {
				byte |= 0x80U;
			}

			enc.emplace_back(byte);
		}

		return enc;
	}

	template<typename T>
	[[nodiscard]]
	std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, std::vector<std::uint8_t>>
	leb128_encode(const T value) noexcept {
		using U = std::make_unsigned_t<T>;
		using V = Panko::core::types::promoted_type_t<U>;

		auto num{static_cast<V>(value)};
		std::vector<std::uint8_t> enc{};

		do {
			auto byte{static_cast<std::uint8_t>(num & 0x7FU)};
			num >>= 7U;

			if (num != 0U) {
				byte |= 0x80U;
			}

			enc.emplace_back(byte);
		} while (num != 0U);


		return enc;
	}

	template<typename T>
	[[nodiscard]]
	std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, T>
	leb128_decode(const std::vector<std::uint8_t>& bytes) noexcept {
		using U = std::make_unsigned_t<T>;
		using V = Panko::core::types::promoted_type_t<U>;

		V enc{};
		std::size_t shift{};

		for (const auto& byte : bytes) {
			enc |= V{byte & 0x7FU} << shift;
			shift += 7zu;
		}

		if (shift && shift < (sizeof(T) * 8zu)) {
			--shift;
			for (std::size_t i{1zu}; i < ((sizeof(T) * 8zu) - shift); ++i) {
				enc |= (enc & (V{1U} << shift)) << i;
			}
		}
		return static_cast<T>(enc);
	}

	template<typename T>
	[[nodiscard]]
	std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, T>
	leb128_decode(const std::vector<std::uint8_t>& bytes) noexcept {
		using U = std::make_unsigned_t<T>;
		using V = Panko::core::types::promoted_type_t<U>;

		V enc{};
		std::size_t shift{};

		for (const auto& byte : bytes) {
			enc |= V{byte & 0x7FU} << shift;
			shift += 7zu;
		}

		return static_cast<T>(enc);
	}
}

#endif /* PANKO_SUPPORT_LEB128_HH */
