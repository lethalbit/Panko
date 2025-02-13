// SPDX-License-Identifier: BSD-3-Clause
/* units.hh - SI/IEC and Time Units */

#pragma once
#if !defined(PANKO_CORE_UNITS_HH)
#define PANKO_CORE_UNITS_HH

#include <cstdint>
#include <chrono>

namespace Panko::core::units {

	namespace IEC {
		[[nodiscard]]
		constexpr std::uint64_t operator ""_KiB(const unsigned long long value) noexcept { return uint64_t(value) * 1024; }
		[[nodiscard]]
		constexpr std::uint64_t operator ""_MiB(const unsigned long long value) noexcept { return uint64_t(value) * 1048576; }
		[[nodiscard]]
		constexpr std::uint64_t operator ""_GiB(const unsigned long long value) noexcept { return uint64_t(value) * 1073741824; }
		[[nodiscard]]
		constexpr std::uint64_t operator ""_TiB(const unsigned long long value) noexcept { return uint64_t(value) * 1099511627776; }
		[[nodiscard]]
		constexpr std::uint64_t operator ""_PiB(const unsigned long long value) noexcept { return uint64_t(value) * 1125899906842624; }
	}

	namespace SI {
		[[nodiscard]]
		constexpr std::uint64_t operator ""_kB(const unsigned long long value) noexcept { return uint64_t(value) * 1000; }
		[[nodiscard]]
		constexpr std::uint64_t operator ""_MB(const unsigned long long value) noexcept { return uint64_t(value) * 1000000; }
		[[nodiscard]]
		constexpr std::uint64_t operator ""_GB(const unsigned long long value) noexcept { return uint64_t(value) * 1000000000; }
		[[nodiscard]]
		constexpr std::uint64_t operator ""_TB(const unsigned long long value) noexcept { return uint64_t(value) * 1000000000000; }
		[[nodiscard]]
		constexpr std::uint64_t operator ""_PB(const unsigned long long value) noexcept { return uint64_t(value) * 1000000000000000; }
	}

	namespace time {
		constexpr static std::chrono::seconds unix_epoch{
			std::chrono::years{1970} + std::chrono::months{1} + std::chrono::days{1}
		};
	};
}

#endif /* PANKO_CORE_UNITS_HH */
