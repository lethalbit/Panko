// SPDX-License-Identifier: BSD-3-Clause
/* integers.cc - strange-width integers test harness */

#include <cstdint>
#include <limits>
#include <ostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "panko/core/integers.hh"

using Panko::core::integers::int24_t;
using Panko::core::integers::uint24_t;
using Panko::core::integers::int40_t;
using Panko::core::integers::uint40_t;
using Panko::core::integers::int48_t;
using Panko::core::integers::uint48_t;
using Panko::core::integers::int50_t;
using Panko::core::integers::uint50_t;
using Panko::core::integers::int56_t;
using Panko::core::integers::uint56_t;


namespace Panko::core::integers {
	template<typename T, std::size_t bits>
	std::ostream& operator<<(std::ostream& os, const intN_t<T, bits>& value) {
		os << static_cast<T>(value);
		return os;
	}
}

/* int24_t/uint24_t */
TEST_CASE("type information - int24_t/uint24_t") {
	CHECK(std::numeric_limits<int24_t>::is_specialized);
	CHECK(std::numeric_limits<int24_t>::is_signed);
	CHECK(std::numeric_limits<int24_t>::is_integer);
	CHECK(std::numeric_limits<int24_t>::is_exact);
	CHECK(!std::numeric_limits<int24_t>::is_iec559);
	CHECK(std::numeric_limits<int24_t>::is_bounded);
	CHECK(!std::numeric_limits<int24_t>::is_modulo);
	CHECK(!std::numeric_limits<int24_t>::has_infinity);
	CHECK(!std::numeric_limits<int24_t>::has_quiet_NaN);
	CHECK(!std::numeric_limits<int24_t>::has_signaling_NaN);
	CHECK_EQ(std::numeric_limits<int24_t>::has_denorm, std::denorm_absent);
	CHECK(!std::numeric_limits<int24_t>::has_denorm_loss);
	CHECK_EQ(std::numeric_limits<int24_t>::digits, 23);
	CHECK_EQ(std::numeric_limits<int24_t>::digits10, 7);
	CHECK_EQ(std::numeric_limits<int24_t>::max_digits10, 0);
	CHECK_EQ(std::numeric_limits<int24_t>::radix, 2);
	CHECK_EQ(std::numeric_limits<int24_t>::max_exponent, 0);
	CHECK_EQ(std::numeric_limits<int24_t>::min_exponent, 0);
	CHECK_EQ(std::numeric_limits<int24_t>::max_exponent10, 0);
	CHECK_EQ(std::numeric_limits<int24_t>::min_exponent10, 0);
	CHECK(std::numeric_limits<int24_t>::traps);
	CHECK(!std::numeric_limits<int24_t>::tinyness_before);
	CHECK_EQ(std::numeric_limits<int24_t>::min(), -8388608);
	CHECK_EQ(std::numeric_limits<int24_t>::lowest(), -8388608);
	CHECK_EQ(std::numeric_limits<int24_t>::max(), 8388607);
	CHECK_EQ(std::numeric_limits<int24_t>::epsilon(), 0);
	CHECK_EQ(std::numeric_limits<int24_t>::round_error(), 0);
	CHECK_EQ(std::numeric_limits<int24_t>::round_style, std::round_toward_zero);
	CHECK_EQ(std::numeric_limits<int24_t>::infinity(), 0);
	CHECK_EQ(std::numeric_limits<int24_t>::quiet_NaN(), 0);
	CHECK_EQ(std::numeric_limits<int24_t>::signaling_NaN(), 0);
	CHECK_EQ(std::numeric_limits<int24_t>::denorm_min(), 0);

	CHECK(std::numeric_limits<uint24_t>::is_specialized);
	CHECK(!std::numeric_limits<uint24_t>::is_signed);
	CHECK(std::numeric_limits<uint24_t>::is_integer);
	CHECK(std::numeric_limits<uint24_t>::is_exact);
	CHECK(!std::numeric_limits<uint24_t>::is_iec559);
	CHECK(std::numeric_limits<uint24_t>::is_bounded);
	CHECK(!std::numeric_limits<uint24_t>::is_modulo);
	CHECK(!std::numeric_limits<uint24_t>::has_infinity);
	CHECK(!std::numeric_limits<uint24_t>::has_quiet_NaN);
	CHECK(!std::numeric_limits<uint24_t>::has_signaling_NaN);
	CHECK_EQ(std::numeric_limits<uint24_t>::has_denorm, std::denorm_absent);
	CHECK(!std::numeric_limits<uint24_t>::has_denorm_loss);
	CHECK_EQ(std::numeric_limits<uint24_t>::digits, 24);
	CHECK_EQ(std::numeric_limits<uint24_t>::digits10, 8);
	CHECK_EQ(std::numeric_limits<uint24_t>::max_digits10, 0);
	CHECK_EQ(std::numeric_limits<uint24_t>::radix, 2);
	CHECK_EQ(std::numeric_limits<uint24_t>::max_exponent, 0);
	CHECK_EQ(std::numeric_limits<uint24_t>::min_exponent, 0);
	CHECK_EQ(std::numeric_limits<uint24_t>::max_exponent10, 0);
	CHECK_EQ(std::numeric_limits<uint24_t>::min_exponent10, 0);
	CHECK(std::numeric_limits<uint24_t>::traps);
	CHECK(!std::numeric_limits<uint24_t>::tinyness_before);
	CHECK_EQ(std::numeric_limits<uint24_t>::min(), 0);
	CHECK_EQ(std::numeric_limits<uint24_t>::lowest(), 0);
	CHECK_EQ(std::numeric_limits<uint24_t>::max(), 16777215);
	CHECK_EQ(std::numeric_limits<uint24_t>::epsilon(), 0);
	CHECK_EQ(std::numeric_limits<uint24_t>::round_error(), 0);
	CHECK_EQ(std::numeric_limits<uint24_t>::round_style, std::round_toward_zero);
	CHECK_EQ(std::numeric_limits<uint24_t>::infinity(), 0);
	CHECK_EQ(std::numeric_limits<uint24_t>::quiet_NaN(), 0);
	CHECK_EQ(std::numeric_limits<uint24_t>::signaling_NaN(), 0);
	CHECK_EQ(std::numeric_limits<uint24_t>::denorm_min(), 0);
}

TEST_CASE("assignment operators - int24_t/uint24_t") {
	using _ty = int24_t;
	using _tu = uint24_t;

	std::int8_t _t0{};
	std::uint8_t _t1{};
	std::int16_t _t2{};
	std::uint16_t _t3{};
	std::int32_t _t4{};
	std::uint32_t _t5{};
	std::int64_t _t6{};
	std::uint64_t _t7{};

	/* Signed */
	_ty a{0};
	CHECK_EQ(a += 4, 4);
	CHECK_EQ(a -= 2, 2);
	CHECK_EQ(a *= 1, 2);
	CHECK_EQ(a /= 2, 1);
	CHECK_EQ(a %= 2, 1);
	CHECK_EQ(a = 0xA5, 0xA5);
	CHECK_EQ(a &= 0x05, 0x05);
	CHECK_EQ(a |= 0xF0, 0xF5);
	CHECK_EQ(a ^= 0xAA, 0x5F);
	CHECK_EQ(a <<= 1U, 0xBE);
	CHECK_EQ(a >>= 1U, 0x5F);
	CHECK_EQ(a = -5, -5);
	CHECK_EQ(a = std::int8_t(1), 1);
	CHECK_EQ(a = std::uint8_t(2), 2);
	CHECK_EQ(a = std::int16_t(3), 3);
	CHECK_EQ(a = std::uint16_t(4), 4);
	CHECK_EQ(a = std::int32_t(5), 5);
	CHECK_EQ(a = std::uint32_t(6), 6);
	CHECK_EQ(a = std::int64_t(7), 7);
	CHECK_EQ(a = std::uint64_t(8), 8);
	CHECK_EQ(a = int24_t(9), 9);
	// CHECK_EQ(a = uint24_t(10), 10);
	// CHECK_EQ(a = int40_t(11), 11);
	// CHECK_EQ(a = uint40_t(12), 12);
	// CHECK_EQ(a = int48_t(13), 13);
	// CHECK_EQ(a = uint48_t(14), 14);
	// CHECK_EQ(a = int50_t(15), 15);
	// CHECK_EQ(a = uint50_t(16), 16);
	// CHECK_EQ(a = int56_t(17), 17);
	// CHECK_EQ(a = uint56_t(18), 18);
	CHECK_EQ(a = 19, 19);
	CHECK_EQ(_t0 = a, 19);
	/* unsigned <- signed */
	CHECK_EQ(a = 20, 20);
	// CHECK_EQ(_t1 = a, 20);
	CHECK_EQ(a = 21, 21);
	CHECK_EQ(_t2 = a, 21);
	/* unsigned <- signed */
	CHECK_EQ(a = 22, 22);
	// CHECK_EQ(_t3 = a, 22);
	CHECK_EQ(a = 23, 23);
	CHECK_EQ(_t4 = a, 23);
	/* unsigned <- signed */
	CHECK_EQ(a = 24, 24);
	// CHECK_EQ(_t5 = a, 24);
	CHECK_EQ(a = 25, 25);
	CHECK_EQ(_t6 = a, 25);
	/* unsigned <- signed */
	CHECK_EQ(a = 26, 26);
	// CHECK_EQ(_t7 = a, 26);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b += 4, 4);
	CHECK_EQ(b -= 2, 2);
	CHECK_EQ(b *= 1, 2);
	CHECK_EQ(b /= 2, 1);
	CHECK_EQ(b %= 2, 1);
	CHECK_EQ(b = 0xA5, 0xA5);
	CHECK_EQ(b &= 0x05, 0x05);
	CHECK_EQ(b |= 0xF0, 0xF5);
	CHECK_EQ(b ^= 0xAA, 0x5F);
	CHECK_EQ(b <<= 1U, 0xBE);
	CHECK_EQ(b >>= 1U, 0x5F);
	CHECK_EQ(b = 5, 5);
	CHECK_EQ(b = std::int8_t(1), 1);
	CHECK_EQ(b = std::uint8_t(2), 2);
	CHECK_EQ(b = std::int16_t(3), 3);
	CHECK_EQ(b = std::uint16_t(4), 4);
	CHECK_EQ(b = std::int32_t(5), 5);
	CHECK_EQ(b = std::uint32_t(6), 6);
	CHECK_EQ(b = std::int64_t(7), 7);
	CHECK_EQ(b = std::uint64_t(8), 8);
	// CHECK_EQ(b = int24_t(9), 9);
	CHECK_EQ(b = uint24_t(10), 10);
	// CHECK_EQ(b = int40_t(11), 11);
	// CHECK_EQ(b = uint40_t(12), 12);
	// CHECK_EQ(b = int48_t(13), 13);
	// CHECK_EQ(b = uint48_t(14), 14);
	// CHECK_EQ(b = int50_t(15), 15);
	// CHECK_EQ(b = uint50_t(16), 16);
	// CHECK_EQ(b = int56_t(17), 17);
	// CHECK_EQ(b = uint56_t(18), 18);
	/* signed <- unsigned */
	CHECK_EQ(b = 19, 19);
	// CHECK_EQ(_t0 = b, 19);
	CHECK_EQ(b = 20, 20);
	CHECK_EQ(_t1 = b, 20);
	/* signed <- unsigned */
	CHECK_EQ(b = 21, 21);
	// CHECK_EQ(_t2 = b, 21);
	CHECK_EQ(b = 22, 22);
	CHECK_EQ(_t3 = b, 22);
	/* signed <- unsigned */
	CHECK_EQ(b = 23, 23);
	// CHECK_EQ(_t4 = b, 23);
	CHECK_EQ(b = 23, 23);
	CHECK_EQ(_t5 = b, 23);
	/* signed <- unsigned */
	CHECK_EQ(b = 24, 24);
	// CHECK_EQ(_t6 = b, 24);
	CHECK_EQ(b = 25, 25);
	CHECK_EQ(_t7 = b, 25);
}

TEST_CASE("increment operators - int24_t/uint24_t") {
	using _ty = int24_t;
	using _tu = uint24_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a++, 0);
	CHECK_EQ(a, 1);
	CHECK_EQ(++a, 2);
	CHECK_EQ(a = std::numeric_limits<_ty>::max(), std::numeric_limits<_ty>::max());
	CHECK_EQ(++a, std::numeric_limits<_ty>::min());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b++, 0);
	CHECK_EQ(b, 1);
	CHECK_EQ(++b, 2);
	CHECK_EQ(b = std::numeric_limits<_tu>::max(), std::numeric_limits<_tu>::max());
	CHECK_EQ(++b, std::numeric_limits<_tu>::min());
}

TEST_CASE("decrement operators - int24_t/uint24_t") {
	using _ty = int24_t;
	using _tu = uint24_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = 2, 2);
	CHECK_EQ(a--, 2);
	CHECK_EQ(a, 1);
	CHECK_EQ(--a, 0);
	CHECK_EQ(a = std::numeric_limits<_ty>::min(), std::numeric_limits<_ty>::min());
	CHECK_EQ(--a, std::numeric_limits<_ty>::max());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 2, 2);
	CHECK_EQ(b--, 2);
	CHECK_EQ(b, 1);
	CHECK_EQ(--b, 0);
	CHECK_EQ(b = std::numeric_limits<_tu>::min(), std::numeric_limits<_tu>::min());
	CHECK_EQ(--b, std::numeric_limits<_tu>::max());
}

TEST_CASE("arithmetic operators - int24_t/uint24_t") {
	using _ty = int24_t;
	using _tu = uint24_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = -5, -5);
	CHECK_EQ(+a, -5);
	CHECK_EQ(-a, 5);
	CHECK_EQ(a + 5, 0);
	CHECK_EQ(a - 5, -10);
	CHECK_EQ(a = -a, 5);
	CHECK_EQ(a * 2, 10);
	CHECK_EQ(a / 2, 2);
	CHECK_EQ(a % 2, 1);
	CHECK_EQ(a = 0xA5, 0xA5);
	CHECK_EQ(~a, -166);
	CHECK_EQ(a & 0x05, 0x05);
	CHECK_EQ(a | 0xF0, 0xF5);
	CHECK_EQ(a ^ 0xAA, 0x0F);
	CHECK_EQ(a << 1U, 0x14A);
	CHECK_EQ(a >> 1U, 0x52);
	CHECK_EQ(a = std::numeric_limits<_ty>::max(), std::numeric_limits<_ty>::max());
	CHECK_EQ(a + 1, std::numeric_limits<_ty>::min());
	CHECK_EQ(a = std::numeric_limits<_ty>::min(), std::numeric_limits<_ty>::min());
	CHECK_EQ(a - 1, std::numeric_limits<_ty>::max());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 5, 5);
	CHECK_EQ(+b, 5);
	CHECK_EQ(b + 5, 10);
	CHECK_EQ(b - 5, 0);
	CHECK_EQ(b * 2, 10);
	CHECK_EQ(b / 2, 2);
	CHECK_EQ(b % 2, 1);
	CHECK_EQ(b = 0xA5, 0xA5);
	CHECK_EQ(~b, 0xFFFF5A);
	CHECK_EQ(b & 0x05, 0x05);
	CHECK_EQ(b | 0xF0, 0xF5);
	CHECK_EQ(b ^ 0xAA, 0x0F);
	CHECK_EQ(b << 1U, 0x14A);
	CHECK_EQ(b >> 1U, 0x52);
	CHECK_EQ(b = std::numeric_limits<_tu>::max(), std::numeric_limits<_tu>::max());
	CHECK_EQ(b + 1, std::numeric_limits<_tu>::min());
	CHECK_EQ(b = std::numeric_limits<_tu>::min(), std::numeric_limits<_tu>::min());
	CHECK_EQ(b - 1, std::numeric_limits<_tu>::max());
}

TEST_CASE("logical operators - int24_t/uint24_t") {
	using _ty = int24_t;
	using _tu = uint24_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(!a, true);
	CHECK_EQ(a && true, false);
	CHECK_EQ(a && false, false);
	CHECK_EQ(a || true, true);
	CHECK_EQ(a || false, false);
	CHECK_EQ(a = 1, 1);
	CHECK_EQ(!a, false);
	CHECK_EQ(a && true, true);
	CHECK_EQ(a && false, false);
	CHECK_EQ(a || true, true);
	CHECK_EQ(a || false, true);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(!b, true);
	CHECK_EQ(b && true, false);
	CHECK_EQ(b && false, false);
	CHECK_EQ(b || true, true);
	CHECK_EQ(b || false, false);
	CHECK_EQ(b = 1, 1);
	CHECK_EQ(!b, false);
	CHECK_EQ(b && true, true);
	CHECK_EQ(b && false, false);
	CHECK_EQ(b || true, true);
	CHECK_EQ(b || false, true);
}

TEST_CASE("comparison operators - int24_t/uint24_t") {
	using _ty = int24_t;
	using _tu = uint24_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = -1, -1);
	CHECK_EQ(a == -1, true);
	CHECK_EQ(a != 0, true);
	CHECK_EQ(a < 1, true);
	CHECK_EQ(a <= 1, true);
	CHECK_EQ(a <= -1, true);
	CHECK_EQ(a  > -2, true);
	CHECK_EQ(a >= -2, true);
	CHECK_EQ(a >= -1, true);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 10, 10);
	CHECK_EQ(b == 10, true);
	CHECK_EQ(b != 5, true);
	CHECK_EQ(b < 5, false);
	CHECK_EQ(b <= 5, false);
	CHECK_EQ(b <= 10, true);
	CHECK_EQ(b > 20, false);
	CHECK_EQ(b >= 20, false);
	CHECK_EQ(b >= 10, true);
}

/* int40_t/uint40_t */
TEST_CASE("type information - int40_t/uint40_t") {
	CHECK(std::numeric_limits<int40_t>::is_specialized);
	CHECK(std::numeric_limits<int40_t>::is_signed);
	CHECK(std::numeric_limits<int40_t>::is_integer);
	CHECK(std::numeric_limits<int40_t>::is_exact);
	CHECK(!std::numeric_limits<int40_t>::is_iec559);
	CHECK(std::numeric_limits<int40_t>::is_bounded);
	CHECK(!std::numeric_limits<int40_t>::is_modulo);
	CHECK(!std::numeric_limits<int40_t>::has_infinity);
	CHECK(!std::numeric_limits<int40_t>::has_quiet_NaN);
	CHECK(!std::numeric_limits<int40_t>::has_signaling_NaN);
	CHECK_EQ(std::numeric_limits<int40_t>::has_denorm, std::denorm_absent);
	CHECK(!std::numeric_limits<int40_t>::has_denorm_loss);
	CHECK_EQ(std::numeric_limits<int40_t>::digits, 39);
	CHECK_EQ(std::numeric_limits<int40_t>::digits10, 12);
	CHECK_EQ(std::numeric_limits<int40_t>::max_digits10, 0);
	CHECK_EQ(std::numeric_limits<int40_t>::radix, 2);
	CHECK_EQ(std::numeric_limits<int40_t>::max_exponent, 0);
	CHECK_EQ(std::numeric_limits<int40_t>::min_exponent, 0);
	CHECK_EQ(std::numeric_limits<int40_t>::max_exponent10, 0);
	CHECK_EQ(std::numeric_limits<int40_t>::min_exponent10, 0);
	CHECK(std::numeric_limits<int40_t>::traps);
	CHECK(!std::numeric_limits<int40_t>::tinyness_before);
	CHECK_EQ(std::numeric_limits<int40_t>::min(), -549755813888);
	CHECK_EQ(std::numeric_limits<int40_t>::lowest(), -549755813888);
	CHECK_EQ(std::numeric_limits<int40_t>::max(), 549755813887);
	CHECK_EQ(std::numeric_limits<int40_t>::epsilon(), 0);
	CHECK_EQ(std::numeric_limits<int40_t>::round_error(), 0);
	CHECK_EQ(std::numeric_limits<int40_t>::round_style, std::round_toward_zero);
	CHECK_EQ(std::numeric_limits<int40_t>::infinity(), 0);
	CHECK_EQ(std::numeric_limits<int40_t>::quiet_NaN(), 0);
	CHECK_EQ(std::numeric_limits<int40_t>::signaling_NaN(), 0);
	CHECK_EQ(std::numeric_limits<int40_t>::denorm_min(), 0);

	CHECK(std::numeric_limits<uint40_t>::is_specialized);
	CHECK(!std::numeric_limits<uint40_t>::is_signed);
	CHECK(std::numeric_limits<uint40_t>::is_integer);
	CHECK(std::numeric_limits<uint40_t>::is_exact);
	CHECK(!std::numeric_limits<uint40_t>::is_iec559);
	CHECK(std::numeric_limits<uint40_t>::is_bounded);
	CHECK(!std::numeric_limits<uint40_t>::is_modulo);
	CHECK(!std::numeric_limits<uint40_t>::has_infinity);
	CHECK(!std::numeric_limits<uint40_t>::has_quiet_NaN);
	CHECK(!std::numeric_limits<uint40_t>::has_signaling_NaN);
	CHECK_EQ(std::numeric_limits<uint40_t>::has_denorm, std::denorm_absent);
	CHECK(!std::numeric_limits<uint40_t>::has_denorm_loss);
	CHECK_EQ(std::numeric_limits<uint40_t>::digits, 40);
	CHECK_EQ(std::numeric_limits<uint40_t>::digits10, 13);
	CHECK_EQ(std::numeric_limits<uint40_t>::max_digits10, 0);
	CHECK_EQ(std::numeric_limits<uint40_t>::radix, 2);
	CHECK_EQ(std::numeric_limits<uint40_t>::max_exponent, 0);
	CHECK_EQ(std::numeric_limits<uint40_t>::min_exponent, 0);
	CHECK_EQ(std::numeric_limits<uint40_t>::max_exponent10, 0);
	CHECK_EQ(std::numeric_limits<uint40_t>::min_exponent10, 0);
	CHECK(std::numeric_limits<uint40_t>::traps);
	CHECK(!std::numeric_limits<uint40_t>::tinyness_before);
	CHECK_EQ(std::numeric_limits<uint40_t>::min(), 0);
	CHECK_EQ(std::numeric_limits<uint40_t>::lowest(), 0);
	CHECK_EQ(std::numeric_limits<uint40_t>::max(), 1099511627775);
	CHECK_EQ(std::numeric_limits<uint40_t>::epsilon(), 0);
	CHECK_EQ(std::numeric_limits<uint40_t>::round_error(), 0);
	CHECK_EQ(std::numeric_limits<uint40_t>::round_style, std::round_toward_zero);
	CHECK_EQ(std::numeric_limits<uint40_t>::infinity(), 0);
	CHECK_EQ(std::numeric_limits<uint40_t>::quiet_NaN(), 0);
	CHECK_EQ(std::numeric_limits<uint40_t>::signaling_NaN(), 0);
	CHECK_EQ(std::numeric_limits<uint40_t>::denorm_min(), 0);
}

TEST_CASE("assignment operators - int40_t/uint40_t") {
	using _ty = int40_t;
	using _tu = uint40_t;

	std::int8_t _t0{};
	std::uint8_t _t1{};
	std::int16_t _t2{};
	std::uint16_t _t3{};
	std::int32_t _t4{};
	std::uint32_t _t5{};
	std::int64_t _t6{};
	std::uint64_t _t7{};

	/* Signed */
	_ty a{0};
	CHECK_EQ(a += 4, 4);
	CHECK_EQ(a -= 2, 2);
	CHECK_EQ(a *= 1, 2);
	CHECK_EQ(a /= 2, 1);
	CHECK_EQ(a %= 2, 1);
	CHECK_EQ(a = 0xA5, 0xA5);
	CHECK_EQ(a &= 0x05, 0x05);
	CHECK_EQ(a |= 0xF0, 0xF5);
	CHECK_EQ(a ^= 0xAA, 0x5F);
	CHECK_EQ(a <<= 1U, 0xBE);
	CHECK_EQ(a >>= 1U, 0x5F);
	CHECK_EQ(a = -5, -5);
	CHECK_EQ(a = std::int8_t(1), 1);
	CHECK_EQ(a = std::uint8_t(2), 2);
	CHECK_EQ(a = std::int16_t(3), 3);
	CHECK_EQ(a = std::uint16_t(4), 4);
	CHECK_EQ(a = std::int32_t(5), 5);
	CHECK_EQ(a = std::uint32_t(6), 6);
	CHECK_EQ(a = std::int64_t(7), 7);
	CHECK_EQ(a = std::uint64_t(8), 8);
	// CHECK_EQ(a = int24_t(9), 9);
	// CHECK_EQ(a = uint24_t(10), 10);
	CHECK_EQ(a = int40_t(11), 11);
	// CHECK_EQ(a = uint40_t(12), 12);
	// CHECK_EQ(a = int48_t(13), 13);
	// CHECK_EQ(a = uint48_t(14), 14);
	// CHECK_EQ(a = int50_t(15), 15);
	// CHECK_EQ(a = uint50_t(16), 16);
	// CHECK_EQ(a = int56_t(17), 17);
	// CHECK_EQ(a = uint56_t(18), 18);
	CHECK_EQ(a = 19, 19);
	CHECK_EQ(_t0 = a, 19);
	/* unsigned <- signed */
	CHECK_EQ(a = 20, 20);
	// CHECK_EQ(_t1 = a, 20);
	CHECK_EQ(a = 21, 21);
	CHECK_EQ(_t2 = a, 21);
	/* unsigned <- signed */
	CHECK_EQ(a = 22, 22);
	// CHECK_EQ(_t3 = a, 22);
	CHECK_EQ(a = 23, 23);
	CHECK_EQ(_t4 = a, 23);
	/* unsigned <- signed */
	CHECK_EQ(a = 24, 24);
	// CHECK_EQ(_t5 = a, 24);
	CHECK_EQ(a = 25, 25);
	CHECK_EQ(_t6 = a, 25);
	/* unsigned <- signed */
	CHECK_EQ(a = 26, 26);
	// CHECK_EQ(_t7 = a, 26);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b += 4, 4);
	CHECK_EQ(b -= 2, 2);
	CHECK_EQ(b *= 1, 2);
	CHECK_EQ(b /= 2, 1);
	CHECK_EQ(b %= 2, 1);
	CHECK_EQ(b = 0xA5, 0xA5);
	CHECK_EQ(b &= 0x05, 0x05);
	CHECK_EQ(b |= 0xF0, 0xF5);
	CHECK_EQ(b ^= 0xAA, 0x5F);
	CHECK_EQ(b <<= 1U, 0xBE);
	CHECK_EQ(b >>= 1U, 0x5F);
	CHECK_EQ(b = 5, 5);
	CHECK_EQ(b = std::int8_t(1), 1);
	CHECK_EQ(b = std::uint8_t(2), 2);
	CHECK_EQ(b = std::int16_t(3), 3);
	CHECK_EQ(b = std::uint16_t(4), 4);
	CHECK_EQ(b = std::int32_t(5), 5);
	CHECK_EQ(b = std::uint32_t(6), 6);
	CHECK_EQ(b = std::int64_t(7), 7);
	CHECK_EQ(b = std::uint64_t(8), 8);
	// CHECK_EQ(b = int24_t(9), 9);
	// CHECK_EQ(b = uint24_t(10), 10);
	// CHECK_EQ(b = int40_t(11), 11);
	CHECK_EQ(b = uint40_t(12), 12);
	// CHECK_EQ(b = int48_t(13), 13);
	// CHECK_EQ(b = uint48_t(14), 14);
	// CHECK_EQ(b = int50_t(15), 15);
	// CHECK_EQ(b = uint50_t(16), 16);
	// CHECK_EQ(b = int56_t(17), 17);
	// CHECK_EQ(b = uint56_t(18), 18);
	/* signed <- unsigned */
	CHECK_EQ(b = 19, 19);
	// CHECK_EQ(_t0 = b, 19);
	CHECK_EQ(b = 20, 20);
	CHECK_EQ(_t1 = b, 20);
	/* signed <- unsigned */
	CHECK_EQ(b = 21, 21);
	// CHECK_EQ(_t2 = b, 21);
	CHECK_EQ(b = 22, 22);
	CHECK_EQ(_t3 = b, 22);
	/* signed <- unsigned */
	CHECK_EQ(b = 23, 23);
	// CHECK_EQ(_t4 = b, 23);
	CHECK_EQ(b = 23, 23);
	CHECK_EQ(_t5 = b, 23);
	/* signed <- unsigned */
	CHECK_EQ(b = 24, 24);
	// CHECK_EQ(_t6 = b, 24);
	CHECK_EQ(b = 25, 25);
	CHECK_EQ(_t7 = b, 25);
}

TEST_CASE("increment operators - int40_t/uint40_t") {
	using _ty = int40_t;
	using _tu = uint40_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a++, 0);
	CHECK_EQ(a, 1);
	CHECK_EQ(++a, 2);
	CHECK_EQ(a = std::numeric_limits<_ty>::max(), std::numeric_limits<_ty>::max());
	CHECK_EQ(++a, std::numeric_limits<_ty>::min());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b++, 0);
	CHECK_EQ(b, 1);
	CHECK_EQ(++b, 2);
	CHECK_EQ(b = std::numeric_limits<_tu>::max(), std::numeric_limits<_tu>::max());
	CHECK_EQ(++b, std::numeric_limits<_tu>::min());
}

TEST_CASE("decrement operators - int40_t/uint40_t") {
	using _ty = int40_t;
	using _tu = uint40_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = 2, 2);
	CHECK_EQ(a--, 2);
	CHECK_EQ(a, 1);
	CHECK_EQ(--a, 0);
	CHECK_EQ(a = std::numeric_limits<_ty>::min(), std::numeric_limits<_ty>::min());
	CHECK_EQ(--a, std::numeric_limits<_ty>::max());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 2, 2);
	CHECK_EQ(b--, 2);
	CHECK_EQ(b, 1);
	CHECK_EQ(--b, 0);
	CHECK_EQ(b = std::numeric_limits<_tu>::min(), std::numeric_limits<_tu>::min());
	CHECK_EQ(--b, std::numeric_limits<_tu>::max());
}

TEST_CASE("arithmetic operators - int40_t/uint40_t") {
	using _ty = int40_t;
	using _tu = uint40_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = -5, -5);
	CHECK_EQ(+a, -5);
	CHECK_EQ(-a, 5);
	CHECK_EQ(a + 5, 0);
	CHECK_EQ(a - 5, -10);
	CHECK_EQ(a = -a, 5);
	CHECK_EQ(a * 2, 10);
	CHECK_EQ(a / 2, 2);
	CHECK_EQ(a % 2, 1);
	CHECK_EQ(a = 0xA5, 0xA5);
	CHECK_EQ(~a, -166);
	CHECK_EQ(a & 0x05, 0x05);
	CHECK_EQ(a | 0xF0, 0xF5);
	CHECK_EQ(a ^ 0xAA, 0x0F);
	CHECK_EQ(a << 1U, 0x14A);
	CHECK_EQ(a >> 1U, 0x52);
	CHECK_EQ(a = std::numeric_limits<_ty>::max(), std::numeric_limits<_ty>::max());
	CHECK_EQ(a + 1, std::numeric_limits<_ty>::min());
	CHECK_EQ(a = std::numeric_limits<_ty>::min(), std::numeric_limits<_ty>::min());
	CHECK_EQ(a - 1, std::numeric_limits<_ty>::max());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 5, 5);
	CHECK_EQ(+b, 5);
	CHECK_EQ(b + 5, 10);
	CHECK_EQ(b - 5, 0);
	CHECK_EQ(b * 2, 10);
	CHECK_EQ(b / 2, 2);
	CHECK_EQ(b % 2, 1);
	CHECK_EQ(b = 0xA5, 0xA5);
	CHECK_EQ(~b, 0xFFFFFFFF5A);
	CHECK_EQ(b & 0x05, 0x05);
	CHECK_EQ(b | 0xF0, 0xF5);
	CHECK_EQ(b ^ 0xAA, 0x0F);
	CHECK_EQ(b << 1U, 0x14A);
	CHECK_EQ(b >> 1U, 0x52);
	CHECK_EQ(b = std::numeric_limits<_tu>::max(), std::numeric_limits<_tu>::max());
	CHECK_EQ(b + 1, std::numeric_limits<_tu>::min());
	CHECK_EQ(b = std::numeric_limits<_tu>::min(), std::numeric_limits<_tu>::min());
	CHECK_EQ(b - 1, std::numeric_limits<_tu>::max());
}

TEST_CASE("logical operators - int40_t/uint40_t") {
	using _ty = int40_t;
	using _tu = uint40_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(!a, true);
	CHECK_EQ(a && true, false);
	CHECK_EQ(a && false, false);
	CHECK_EQ(a || true, true);
	CHECK_EQ(a || false, false);
	CHECK_EQ(a = 1, 1);
	CHECK_EQ(!a, false);
	CHECK_EQ(a && true, true);
	CHECK_EQ(a && false, false);
	CHECK_EQ(a || true, true);
	CHECK_EQ(a || false, true);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(!b, true);
	CHECK_EQ(b && true, false);
	CHECK_EQ(b && false, false);
	CHECK_EQ(b || true, true);
	CHECK_EQ(b || false, false);
	CHECK_EQ(b = 1, 1);
	CHECK_EQ(!b, false);
	CHECK_EQ(b && true, true);
	CHECK_EQ(b && false, false);
	CHECK_EQ(b || true, true);
	CHECK_EQ(b || false, true);
}

TEST_CASE("comparison operators - int40_t/uint40_t") {
	using _ty = int40_t;
	using _tu = uint40_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = -1, -1);
	CHECK_EQ(a == -1, true);
	CHECK_EQ(a != 0, true);
	CHECK_EQ(a < 1, true);
	CHECK_EQ(a <= 1, true);
	CHECK_EQ(a <= -1, true);
	CHECK_EQ(a  > -2, true);
	CHECK_EQ(a >= -2, true);
	CHECK_EQ(a >= -1, true);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 10, 10);
	CHECK_EQ(b == 10, true);
	CHECK_EQ(b != 5, true);
	CHECK_EQ(b < 5, false);
	CHECK_EQ(b <= 5, false);
	CHECK_EQ(b <= 10, true);
	CHECK_EQ(b > 20, false);
	CHECK_EQ(b >= 20, false);
	CHECK_EQ(b >= 10, true);
}

/* int48_t/uint48_t */
TEST_CASE("type information - int48_t/uint48_t") {
	CHECK(std::numeric_limits<int48_t>::is_specialized);
	CHECK(std::numeric_limits<int48_t>::is_signed);
	CHECK(std::numeric_limits<int48_t>::is_integer);
	CHECK(std::numeric_limits<int48_t>::is_exact);
	CHECK(!std::numeric_limits<int48_t>::is_iec559);
	CHECK(std::numeric_limits<int48_t>::is_bounded);
	CHECK(!std::numeric_limits<int48_t>::is_modulo);
	CHECK(!std::numeric_limits<int48_t>::has_infinity);
	CHECK(!std::numeric_limits<int48_t>::has_quiet_NaN);
	CHECK(!std::numeric_limits<int48_t>::has_signaling_NaN);
	CHECK_EQ(std::numeric_limits<int48_t>::has_denorm, std::denorm_absent);
	CHECK(!std::numeric_limits<int48_t>::has_denorm_loss);
	CHECK_EQ(std::numeric_limits<int48_t>::digits, 47);
	CHECK_EQ(std::numeric_limits<int48_t>::digits10, 15);
	CHECK_EQ(std::numeric_limits<int48_t>::max_digits10, 0);
	CHECK_EQ(std::numeric_limits<int48_t>::radix, 2);
	CHECK_EQ(std::numeric_limits<int48_t>::max_exponent, 0);
	CHECK_EQ(std::numeric_limits<int48_t>::min_exponent, 0);
	CHECK_EQ(std::numeric_limits<int48_t>::max_exponent10, 0);
	CHECK_EQ(std::numeric_limits<int48_t>::min_exponent10, 0);
	CHECK(std::numeric_limits<int48_t>::traps);
	CHECK(!std::numeric_limits<int48_t>::tinyness_before);
	CHECK_EQ(std::numeric_limits<int48_t>::min(), -140737488355328);
	CHECK_EQ(std::numeric_limits<int48_t>::lowest(), -140737488355328);
	CHECK_EQ(std::numeric_limits<int48_t>::max(), 140737488355327);
	CHECK_EQ(std::numeric_limits<int48_t>::epsilon(), 0);
	CHECK_EQ(std::numeric_limits<int48_t>::round_error(), 0);
	CHECK_EQ(std::numeric_limits<int48_t>::round_style, std::round_toward_zero);
	CHECK_EQ(std::numeric_limits<int48_t>::infinity(), 0);
	CHECK_EQ(std::numeric_limits<int48_t>::quiet_NaN(), 0);
	CHECK_EQ(std::numeric_limits<int48_t>::signaling_NaN(), 0);
	CHECK_EQ(std::numeric_limits<int48_t>::denorm_min(), 0);

	CHECK(std::numeric_limits<uint48_t>::is_specialized);
	CHECK(!std::numeric_limits<uint48_t>::is_signed);
	CHECK(std::numeric_limits<uint48_t>::is_integer);
	CHECK(std::numeric_limits<uint48_t>::is_exact);
	CHECK(!std::numeric_limits<uint48_t>::is_iec559);
	CHECK(std::numeric_limits<uint48_t>::is_bounded);
	CHECK(!std::numeric_limits<uint48_t>::is_modulo);
	CHECK(!std::numeric_limits<uint48_t>::has_infinity);
	CHECK(!std::numeric_limits<uint48_t>::has_quiet_NaN);
	CHECK(!std::numeric_limits<uint48_t>::has_signaling_NaN);
	CHECK_EQ(std::numeric_limits<uint48_t>::has_denorm, std::denorm_absent);
	CHECK(!std::numeric_limits<uint48_t>::has_denorm_loss);
	CHECK_EQ(std::numeric_limits<uint48_t>::digits, 48);
	CHECK_EQ(std::numeric_limits<uint48_t>::digits10, 15);
	CHECK_EQ(std::numeric_limits<uint48_t>::max_digits10, 0);
	CHECK_EQ(std::numeric_limits<uint48_t>::radix, 2);
	CHECK_EQ(std::numeric_limits<uint48_t>::max_exponent, 0);
	CHECK_EQ(std::numeric_limits<uint48_t>::min_exponent, 0);
	CHECK_EQ(std::numeric_limits<uint48_t>::max_exponent10, 0);
	CHECK_EQ(std::numeric_limits<uint48_t>::min_exponent10, 0);
	CHECK(std::numeric_limits<uint48_t>::traps);
	CHECK(!std::numeric_limits<uint48_t>::tinyness_before);
	CHECK_EQ(std::numeric_limits<uint48_t>::min(), 0);
	CHECK_EQ(std::numeric_limits<uint48_t>::lowest(), 0);
	CHECK_EQ(std::numeric_limits<uint48_t>::max(), 281474976710655);
	CHECK_EQ(std::numeric_limits<uint48_t>::epsilon(), 0);
	CHECK_EQ(std::numeric_limits<uint48_t>::round_error(), 0);
	CHECK_EQ(std::numeric_limits<uint48_t>::round_style, std::round_toward_zero);
	CHECK_EQ(std::numeric_limits<uint48_t>::infinity(), 0);
	CHECK_EQ(std::numeric_limits<uint48_t>::quiet_NaN(), 0);
	CHECK_EQ(std::numeric_limits<uint48_t>::signaling_NaN(), 0);
	CHECK_EQ(std::numeric_limits<uint48_t>::denorm_min(), 0);
}

TEST_CASE("assignment operators - int48_t/uint48_t") {
	using _ty = int48_t;
	using _tu = uint48_t;

	std::int8_t _t0{};
	std::uint8_t _t1{};
	std::int16_t _t2{};
	std::uint16_t _t3{};
	std::int32_t _t4{};
	std::uint32_t _t5{};
	std::int64_t _t6{};
	std::uint64_t _t7{};

	/* Signed */
	_ty a{0};
	CHECK_EQ(a += 4, 4);
	CHECK_EQ(a -= 2, 2);
	CHECK_EQ(a *= 1, 2);
	CHECK_EQ(a /= 2, 1);
	CHECK_EQ(a %= 2, 1);
	CHECK_EQ(a = 0xA5, 0xA5);
	CHECK_EQ(a &= 0x05, 0x05);
	CHECK_EQ(a |= 0xF0, 0xF5);
	CHECK_EQ(a ^= 0xAA, 0x5F);
	CHECK_EQ(a <<= 1U, 0xBE);
	CHECK_EQ(a >>= 1U, 0x5F);
	CHECK_EQ(a = -5, -5);
	CHECK_EQ(a = std::int8_t(1), 1);
	CHECK_EQ(a = std::uint8_t(2), 2);
	CHECK_EQ(a = std::int16_t(3), 3);
	CHECK_EQ(a = std::uint16_t(4), 4);
	CHECK_EQ(a = std::int32_t(5), 5);
	CHECK_EQ(a = std::uint32_t(6), 6);
	CHECK_EQ(a = std::int64_t(7), 7);
	CHECK_EQ(a = std::uint64_t(8), 8);
	// CHECK_EQ(a = int24_t(9), 9);
	// CHECK_EQ(a = uint24_t(10), 10);
	// CHECK_EQ(a = int40_t(11), 11);
	// CHECK_EQ(a = uint40_t(12), 12);
	CHECK_EQ(a = int48_t(13), 13);
	// CHECK_EQ(a = uint48_t(14), 14);
	// CHECK_EQ(a = int50_t(15), 15);
	// CHECK_EQ(a = uint50_t(16), 16);
	// CHECK_EQ(a = int56_t(17), 17);
	// CHECK_EQ(a = uint56_t(18), 18);
	CHECK_EQ(a = 19, 19);
	CHECK_EQ(_t0 = a, 19);
	/* unsigned <- signed */
	CHECK_EQ(a = 20, 20);
	// CHECK_EQ(_t1 = a, 20);
	CHECK_EQ(a = 21, 21);
	CHECK_EQ(_t2 = a, 21);
	/* unsigned <- signed */
	CHECK_EQ(a = 22, 22);
	// CHECK_EQ(_t3 = a, 22);
	CHECK_EQ(a = 23, 23);
	CHECK_EQ(_t4 = a, 23);
	/* unsigned <- signed */
	CHECK_EQ(a = 24, 24);
	// CHECK_EQ(_t5 = a, 24);
	CHECK_EQ(a = 25, 25);
	CHECK_EQ(_t6 = a, 25);
	/* unsigned <- signed */
	CHECK_EQ(a = 26, 26);
	// CHECK_EQ(_t7 = a, 26);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b += 4, 4);
	CHECK_EQ(b -= 2, 2);
	CHECK_EQ(b *= 1, 2);
	CHECK_EQ(b /= 2, 1);
	CHECK_EQ(b %= 2, 1);
	CHECK_EQ(b = 0xA5, 0xA5);
	CHECK_EQ(b &= 0x05, 0x05);
	CHECK_EQ(b |= 0xF0, 0xF5);
	CHECK_EQ(b ^= 0xAA, 0x5F);
	CHECK_EQ(b <<= 1U, 0xBE);
	CHECK_EQ(b >>= 1U, 0x5F);
	CHECK_EQ(b = 5, 5);
	CHECK_EQ(b = std::int8_t(1), 1);
	CHECK_EQ(b = std::uint8_t(2), 2);
	CHECK_EQ(b = std::int16_t(3), 3);
	CHECK_EQ(b = std::uint16_t(4), 4);
	CHECK_EQ(b = std::int32_t(5), 5);
	CHECK_EQ(b = std::uint32_t(6), 6);
	CHECK_EQ(b = std::int64_t(7), 7);
	CHECK_EQ(b = std::uint64_t(8), 8);
	// CHECK_EQ(b = int24_t(9), 9);
	// CHECK_EQ(b = uint24_t(10), 10);
	// CHECK_EQ(b = int40_t(11), 11);
	// CHECK_EQ(b = uint40_t(12), 12);
	// CHECK_EQ(b = int48_t(13), 13);
	CHECK_EQ(b = uint48_t(14), 14);
	// CHECK_EQ(b = int50_t(15), 15);
	// CHECK_EQ(b = uint50_t(16), 16);
	// CHECK_EQ(b = int56_t(17), 17);
	// CHECK_EQ(b = uint56_t(18), 18);
	/* signed <- unsigned */
	CHECK_EQ(b = 19, 19);
	// CHECK_EQ(_t0 = b, 19);
	CHECK_EQ(b = 20, 20);
	CHECK_EQ(_t1 = b, 20);
	/* signed <- unsigned */
	CHECK_EQ(b = 21, 21);
	// CHECK_EQ(_t2 = b, 21);
	CHECK_EQ(b = 22, 22);
	CHECK_EQ(_t3 = b, 22);
	/* signed <- unsigned */
	CHECK_EQ(b = 23, 23);
	// CHECK_EQ(_t4 = b, 23);
	CHECK_EQ(b = 23, 23);
	CHECK_EQ(_t5 = b, 23);
	/* signed <- unsigned */
	CHECK_EQ(b = 24, 24);
	// CHECK_EQ(_t6 = b, 24);
	CHECK_EQ(b = 25, 25);
	CHECK_EQ(_t7 = b, 25);
}

TEST_CASE("increment operators - int48_t/uint48_t") {
	using _ty = int48_t;
	using _tu = uint48_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a++, 0);
	CHECK_EQ(a, 1);
	CHECK_EQ(++a, 2);
	CHECK_EQ(a = std::numeric_limits<_ty>::max(), std::numeric_limits<_ty>::max());
	CHECK_EQ(++a, std::numeric_limits<_ty>::min());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b++, 0);
	CHECK_EQ(b, 1);
	CHECK_EQ(++b, 2);
	CHECK_EQ(b = std::numeric_limits<_tu>::max(), std::numeric_limits<_tu>::max());
	CHECK_EQ(++b, std::numeric_limits<_tu>::min());
}

TEST_CASE("decrement operators - int48_t/uint48_t") {
	using _ty = int48_t;
	using _tu = uint48_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = 2, 2);
	CHECK_EQ(a--, 2);
	CHECK_EQ(a, 1);
	CHECK_EQ(--a, 0);
	CHECK_EQ(a = std::numeric_limits<_ty>::min(), std::numeric_limits<_ty>::min());
	CHECK_EQ(--a, std::numeric_limits<_ty>::max());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 2, 2);
	CHECK_EQ(b--, 2);
	CHECK_EQ(b, 1);
	CHECK_EQ(--b, 0);
	CHECK_EQ(b = std::numeric_limits<_tu>::min(), std::numeric_limits<_tu>::min());
	CHECK_EQ(--b, std::numeric_limits<_tu>::max());
}

TEST_CASE("arithmetic operators - int48_t/uint48_t") {
	using _ty = int48_t;
	using _tu = uint48_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = -5, -5);
	CHECK_EQ(+a, -5);
	CHECK_EQ(-a, 5);
	CHECK_EQ(a + 5, 0);
	CHECK_EQ(a - 5, -10);
	CHECK_EQ(a = -a, 5);
	CHECK_EQ(a * 2, 10);
	CHECK_EQ(a / 2, 2);
	CHECK_EQ(a % 2, 1);
	CHECK_EQ(a = 0xA5, 0xA5);
	CHECK_EQ(~a, -166);
	CHECK_EQ(a & 0x05, 0x05);
	CHECK_EQ(a | 0xF0, 0xF5);
	CHECK_EQ(a ^ 0xAA, 0x0F);
	CHECK_EQ(a << 1U, 0x14A);
	CHECK_EQ(a >> 1U, 0x52);
	CHECK_EQ(a = std::numeric_limits<_ty>::max(), std::numeric_limits<_ty>::max());
	CHECK_EQ(a + 1, std::numeric_limits<_ty>::min());
	CHECK_EQ(a = std::numeric_limits<_ty>::min(), std::numeric_limits<_ty>::min());
	CHECK_EQ(a - 1, std::numeric_limits<_ty>::max());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 5, 5);
	CHECK_EQ(+b, 5);
	CHECK_EQ(b + 5, 10);
	CHECK_EQ(b - 5, 0);
	CHECK_EQ(b * 2, 10);
	CHECK_EQ(b / 2, 2);
	CHECK_EQ(b % 2, 1);
	CHECK_EQ(b = 0xA5, 0xA5);
	CHECK_EQ(~b, 0xFFFFFFFFFF5A);
	CHECK_EQ(b & 0x05, 0x05);
	CHECK_EQ(b | 0xF0, 0xF5);
	CHECK_EQ(b ^ 0xAA, 0x0F);
	CHECK_EQ(b << 1U, 0x14A);
	CHECK_EQ(b >> 1U, 0x52);
	CHECK_EQ(b = std::numeric_limits<_tu>::max(), std::numeric_limits<_tu>::max());
	CHECK_EQ(b + 1, std::numeric_limits<_tu>::min());
	CHECK_EQ(b = std::numeric_limits<_tu>::min(), std::numeric_limits<_tu>::min());
	CHECK_EQ(b - 1, std::numeric_limits<_tu>::max());
}

TEST_CASE("logical operators - int48_t/uint48_t") {
	using _ty = int48_t;
	using _tu = uint48_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(!a, true);
	CHECK_EQ(a && true, false);
	CHECK_EQ(a && false, false);
	CHECK_EQ(a || true, true);
	CHECK_EQ(a || false, false);
	CHECK_EQ(a = 1, 1);
	CHECK_EQ(!a, false);
	CHECK_EQ(a && true, true);
	CHECK_EQ(a && false, false);
	CHECK_EQ(a || true, true);
	CHECK_EQ(a || false, true);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(!b, true);
	CHECK_EQ(b && true, false);
	CHECK_EQ(b && false, false);
	CHECK_EQ(b || true, true);
	CHECK_EQ(b || false, false);
	CHECK_EQ(b = 1, 1);
	CHECK_EQ(!b, false);
	CHECK_EQ(b && true, true);
	CHECK_EQ(b && false, false);
	CHECK_EQ(b || true, true);
	CHECK_EQ(b || false, true);
}

TEST_CASE("comparison operators - int48_t/uint48_t") {
	using _ty = int48_t;
	using _tu = uint48_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = -1, -1);
	CHECK_EQ(a == -1, true);
	CHECK_EQ(a != 0, true);
	CHECK_EQ(a < 1, true);
	CHECK_EQ(a <= 1, true);
	CHECK_EQ(a <= -1, true);
	CHECK_EQ(a  > -2, true);
	CHECK_EQ(a >= -2, true);
	CHECK_EQ(a >= -1, true);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 10, 10);
	CHECK_EQ(b == 10, true);
	CHECK_EQ(b != 5, true);
	CHECK_EQ(b < 5, false);
	CHECK_EQ(b <= 5, false);
	CHECK_EQ(b <= 10, true);
	CHECK_EQ(b > 20, false);
	CHECK_EQ(b >= 20, false);
	CHECK_EQ(b >= 10, true);
}

/* int50_t/uint50_t */
TEST_CASE("type information - int50_t/uint50_t") {
	CHECK(std::numeric_limits<int50_t>::is_specialized);
	CHECK(std::numeric_limits<int50_t>::is_signed);
	CHECK(std::numeric_limits<int50_t>::is_integer);
	CHECK(std::numeric_limits<int50_t>::is_exact);
	CHECK(!std::numeric_limits<int50_t>::is_iec559);
	CHECK(std::numeric_limits<int50_t>::is_bounded);
	CHECK(!std::numeric_limits<int50_t>::is_modulo);
	CHECK(!std::numeric_limits<int50_t>::has_infinity);
	CHECK(!std::numeric_limits<int50_t>::has_quiet_NaN);
	CHECK(!std::numeric_limits<int50_t>::has_signaling_NaN);
	CHECK_EQ(std::numeric_limits<int50_t>::has_denorm, std::denorm_absent);
	CHECK(!std::numeric_limits<int50_t>::has_denorm_loss);
	CHECK_EQ(std::numeric_limits<int50_t>::digits, 49);
	CHECK_EQ(std::numeric_limits<int50_t>::digits10, 15);
	CHECK_EQ(std::numeric_limits<int50_t>::max_digits10, 0);
	CHECK_EQ(std::numeric_limits<int50_t>::radix, 2);
	CHECK_EQ(std::numeric_limits<int50_t>::max_exponent, 0);
	CHECK_EQ(std::numeric_limits<int50_t>::min_exponent, 0);
	CHECK_EQ(std::numeric_limits<int50_t>::max_exponent10, 0);
	CHECK_EQ(std::numeric_limits<int50_t>::min_exponent10, 0);
	CHECK(std::numeric_limits<int50_t>::traps);
	CHECK(!std::numeric_limits<int50_t>::tinyness_before);
	CHECK_EQ(std::numeric_limits<int50_t>::min(), -562949953421312);
	CHECK_EQ(std::numeric_limits<int50_t>::lowest(), -562949953421312);
	CHECK_EQ(std::numeric_limits<int50_t>::max(), 562949953421311);
	CHECK_EQ(std::numeric_limits<int50_t>::epsilon(), 0);
	CHECK_EQ(std::numeric_limits<int50_t>::round_error(), 0);
	CHECK_EQ(std::numeric_limits<int50_t>::round_style, std::round_toward_zero);
	CHECK_EQ(std::numeric_limits<int50_t>::infinity(), 0);
	CHECK_EQ(std::numeric_limits<int50_t>::quiet_NaN(), 0);
	CHECK_EQ(std::numeric_limits<int50_t>::signaling_NaN(), 0);
	CHECK_EQ(std::numeric_limits<int50_t>::denorm_min(), 0);

	CHECK(std::numeric_limits<uint50_t>::is_specialized);
	CHECK(!std::numeric_limits<uint50_t>::is_signed);
	CHECK(std::numeric_limits<uint50_t>::is_integer);
	CHECK(std::numeric_limits<uint50_t>::is_exact);
	CHECK(!std::numeric_limits<uint50_t>::is_iec559);
	CHECK(std::numeric_limits<uint50_t>::is_bounded);
	CHECK(!std::numeric_limits<uint50_t>::is_modulo);
	CHECK(!std::numeric_limits<uint50_t>::has_infinity);
	CHECK(!std::numeric_limits<uint50_t>::has_quiet_NaN);
	CHECK(!std::numeric_limits<uint50_t>::has_signaling_NaN);
	CHECK_EQ(std::numeric_limits<uint50_t>::has_denorm, std::denorm_absent);
	CHECK(!std::numeric_limits<uint50_t>::has_denorm_loss);
	CHECK_EQ(std::numeric_limits<uint50_t>::digits, 50);
	CHECK_EQ(std::numeric_limits<uint50_t>::digits10, 16);
	CHECK_EQ(std::numeric_limits<uint50_t>::max_digits10, 0);
	CHECK_EQ(std::numeric_limits<uint50_t>::radix, 2);
	CHECK_EQ(std::numeric_limits<uint50_t>::max_exponent, 0);
	CHECK_EQ(std::numeric_limits<uint50_t>::min_exponent, 0);
	CHECK_EQ(std::numeric_limits<uint50_t>::max_exponent10, 0);
	CHECK_EQ(std::numeric_limits<uint50_t>::min_exponent10, 0);
	CHECK(std::numeric_limits<uint50_t>::traps);
	CHECK(!std::numeric_limits<uint50_t>::tinyness_before);
	CHECK_EQ(std::numeric_limits<uint50_t>::min(), 0);
	CHECK_EQ(std::numeric_limits<uint50_t>::lowest(), 0);
	CHECK_EQ(std::numeric_limits<uint50_t>::max(), 1125899906842623);
	CHECK_EQ(std::numeric_limits<uint50_t>::epsilon(), 0);
	CHECK_EQ(std::numeric_limits<uint50_t>::round_error(), 0);
	CHECK_EQ(std::numeric_limits<uint50_t>::round_style, std::round_toward_zero);
	CHECK_EQ(std::numeric_limits<uint50_t>::infinity(), 0);
	CHECK_EQ(std::numeric_limits<uint50_t>::quiet_NaN(), 0);
	CHECK_EQ(std::numeric_limits<uint50_t>::signaling_NaN(), 0);
	CHECK_EQ(std::numeric_limits<uint50_t>::denorm_min(), 0);
}

TEST_CASE("assignment operators - int50_t/uint50_t") {
	using _ty = int50_t;
	using _tu = uint50_t;

	std::int8_t _t0{};
	std::uint8_t _t1{};
	std::int16_t _t2{};
	std::uint16_t _t3{};
	std::int32_t _t4{};
	std::uint32_t _t5{};
	std::int64_t _t6{};
	std::uint64_t _t7{};

	/* Signed */
	_ty a{0};
	CHECK_EQ(a += 4, 4);
	CHECK_EQ(a -= 2, 2);
	CHECK_EQ(a *= 1, 2);
	CHECK_EQ(a /= 2, 1);
	CHECK_EQ(a %= 2, 1);
	CHECK_EQ(a = 0xA5, 0xA5);
	CHECK_EQ(a &= 0x05, 0x05);
	CHECK_EQ(a |= 0xF0, 0xF5);
	CHECK_EQ(a ^= 0xAA, 0x5F);
	CHECK_EQ(a <<= 1U, 0xBE);
	CHECK_EQ(a >>= 1U, 0x5F);
	CHECK_EQ(a = -5, -5);
	CHECK_EQ(a = std::int8_t(1), 1);
	CHECK_EQ(a = std::uint8_t(2), 2);
	CHECK_EQ(a = std::int16_t(3), 3);
	CHECK_EQ(a = std::uint16_t(4), 4);
	CHECK_EQ(a = std::int32_t(5), 5);
	CHECK_EQ(a = std::uint32_t(6), 6);
	CHECK_EQ(a = std::int64_t(7), 7);
	CHECK_EQ(a = std::uint64_t(8), 8);
	// CHECK_EQ(a = int24_t(9), 9);
	// CHECK_EQ(a = uint24_t(10), 10);
	// CHECK_EQ(a = int40_t(11), 11);
	// CHECK_EQ(a = uint40_t(12), 12);
	// CHECK_EQ(a = int48_t(13), 13);
	// CHECK_EQ(a = uint48_t(14), 14);
	CHECK_EQ(a = int50_t(15), 15);
	// CHECK_EQ(a = uint50_t(16), 16);
	// CHECK_EQ(a = int56_t(17), 17);
	// CHECK_EQ(a = uint56_t(18), 18);
	CHECK_EQ(a = 19, 19);
	CHECK_EQ(_t0 = a, 19);
	/* unsigned <- signed */
	CHECK_EQ(a = 20, 20);
	// CHECK_EQ(_t1 = a, 20);
	CHECK_EQ(a = 21, 21);
	CHECK_EQ(_t2 = a, 21);
	/* unsigned <- signed */
	CHECK_EQ(a = 22, 22);
	// CHECK_EQ(_t3 = a, 22);
	CHECK_EQ(a = 23, 23);
	CHECK_EQ(_t4 = a, 23);
	/* unsigned <- signed */
	CHECK_EQ(a = 24, 24);
	// CHECK_EQ(_t5 = a, 24);
	CHECK_EQ(a = 25, 25);
	CHECK_EQ(_t6 = a, 25);
	/* unsigned <- signed */
	CHECK_EQ(a = 26, 26);
	// CHECK_EQ(_t7 = a, 26);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b += 4, 4);
	CHECK_EQ(b -= 2, 2);
	CHECK_EQ(b *= 1, 2);
	CHECK_EQ(b /= 2, 1);
	CHECK_EQ(b %= 2, 1);
	CHECK_EQ(b = 0xA5, 0xA5);
	CHECK_EQ(b &= 0x05, 0x05);
	CHECK_EQ(b |= 0xF0, 0xF5);
	CHECK_EQ(b ^= 0xAA, 0x5F);
	CHECK_EQ(b <<= 1U, 0xBE);
	CHECK_EQ(b >>= 1U, 0x5F);
	CHECK_EQ(b = 5, 5);
	CHECK_EQ(b = std::int8_t(1), 1);
	CHECK_EQ(b = std::uint8_t(2), 2);
	CHECK_EQ(b = std::int16_t(3), 3);
	CHECK_EQ(b = std::uint16_t(4), 4);
	CHECK_EQ(b = std::int32_t(5), 5);
	CHECK_EQ(b = std::uint32_t(6), 6);
	CHECK_EQ(b = std::int64_t(7), 7);
	CHECK_EQ(b = std::uint64_t(8), 8);
	// CHECK_EQ(b = int24_t(9), 9);
	// CHECK_EQ(b = uint24_t(10), 10);
	// CHECK_EQ(b = int40_t(11), 11);
	// CHECK_EQ(b = uint40_t(12), 12);
	// CHECK_EQ(b = int48_t(13), 13);
	// CHECK_EQ(b = uint48_t(14), 14);
	// CHECK_EQ(b = int50_t(15), 15);
	CHECK_EQ(b = uint50_t(16), 16);
	// CHECK_EQ(b = int56_t(17), 17);
	// CHECK_EQ(b = uint56_t(18), 18);
	/* signed <- unsigned */
	CHECK_EQ(b = 19, 19);
	// CHECK_EQ(_t0 = b, 19);
	CHECK_EQ(b = 20, 20);
	CHECK_EQ(_t1 = b, 20);
	/* signed <- unsigned */
	CHECK_EQ(b = 21, 21);
	// CHECK_EQ(_t2 = b, 21);
	CHECK_EQ(b = 22, 22);
	CHECK_EQ(_t3 = b, 22);
	/* signed <- unsigned */
	CHECK_EQ(b = 23, 23);
	// CHECK_EQ(_t4 = b, 23);
	CHECK_EQ(b = 23, 23);
	CHECK_EQ(_t5 = b, 23);
	/* signed <- unsigned */
	CHECK_EQ(b = 24, 24);
	// CHECK_EQ(_t6 = b, 24);
	CHECK_EQ(b = 25, 25);
	CHECK_EQ(_t7 = b, 25);
}

TEST_CASE("increment operators - int50_t/uint50_t") {
	using _ty = int50_t;
	using _tu = uint50_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a++, 0);
	CHECK_EQ(a, 1);
	CHECK_EQ(++a, 2);
	CHECK_EQ(a = std::numeric_limits<_ty>::max(), std::numeric_limits<_ty>::max());
	CHECK_EQ(++a, std::numeric_limits<_ty>::min());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b++, 0);
	CHECK_EQ(b, 1);
	CHECK_EQ(++b, 2);
	CHECK_EQ(b = std::numeric_limits<_tu>::max(), std::numeric_limits<_tu>::max());
	CHECK_EQ(++b, std::numeric_limits<_tu>::min());
}

TEST_CASE("decrement operators - int50_t/uint50_t") {
	using _ty = int50_t;
	using _tu = uint50_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = 2, 2);
	CHECK_EQ(a--, 2);
	CHECK_EQ(a, 1);
	CHECK_EQ(--a, 0);
	CHECK_EQ(a = std::numeric_limits<_ty>::min(), std::numeric_limits<_ty>::min());
	CHECK_EQ(--a, std::numeric_limits<_ty>::max());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 2, 2);
	CHECK_EQ(b--, 2);
	CHECK_EQ(b, 1);
	CHECK_EQ(--b, 0);
	CHECK_EQ(b = std::numeric_limits<_tu>::min(), std::numeric_limits<_tu>::min());
	CHECK_EQ(--b, std::numeric_limits<_tu>::max());
}

TEST_CASE("arithmetic operators - int50_t/uint50_t") {
	using _ty = int50_t;
	using _tu = uint50_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = -5, -5);
	CHECK_EQ(+a, -5);
	CHECK_EQ(-a, 5);
	CHECK_EQ(a + 5, 0);
	CHECK_EQ(a - 5, -10);
	CHECK_EQ(a = -a, 5);
	CHECK_EQ(a * 2, 10);
	CHECK_EQ(a / 2, 2);
	CHECK_EQ(a % 2, 1);
	CHECK_EQ(a = 0xA5, 0xA5);
	CHECK_EQ(~a, -166);
	CHECK_EQ(a & 0x05, 0x05);
	CHECK_EQ(a | 0xF0, 0xF5);
	CHECK_EQ(a ^ 0xAA, 0x0F);
	CHECK_EQ(a << 1U, 0x14A);
	CHECK_EQ(a >> 1U, 0x52);
	CHECK_EQ(a = std::numeric_limits<_ty>::max(), std::numeric_limits<_ty>::max());
	CHECK_EQ(a + 1, std::numeric_limits<_ty>::min());
	CHECK_EQ(a = std::numeric_limits<_ty>::min(), std::numeric_limits<_ty>::min());
	CHECK_EQ(a - 1, std::numeric_limits<_ty>::max());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 5, 5);
	CHECK_EQ(+b, 5);
	CHECK_EQ(b + 5, 10);
	CHECK_EQ(b - 5, 0);
	CHECK_EQ(b * 2, 10);
	CHECK_EQ(b / 2, 2);
	CHECK_EQ(b % 2, 1);
	CHECK_EQ(b = 0xA5, 0xA5);
	CHECK_EQ(~b, 0x3FFFFFFFFFF5A);
	CHECK_EQ(b & 0x05, 0x05);
	CHECK_EQ(b | 0xF0, 0xF5);
	CHECK_EQ(b ^ 0xAA, 0x0F);
	CHECK_EQ(b << 1U, 0x14A);
	CHECK_EQ(b >> 1U, 0x52);
	CHECK_EQ(b = std::numeric_limits<_tu>::max(), std::numeric_limits<_tu>::max());
	CHECK_EQ(b + 1, std::numeric_limits<_tu>::min());
	CHECK_EQ(b = std::numeric_limits<_tu>::min(), std::numeric_limits<_tu>::min());
	CHECK_EQ(b - 1, std::numeric_limits<_tu>::max());
}

TEST_CASE("logical operators - int50_t/uint50_t") {
	using _ty = int50_t;
	using _tu = uint50_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(!a, true);
	CHECK_EQ(a && true, false);
	CHECK_EQ(a && false, false);
	CHECK_EQ(a || true, true);
	CHECK_EQ(a || false, false);
	CHECK_EQ(a = 1, 1);
	CHECK_EQ(!a, false);
	CHECK_EQ(a && true, true);
	CHECK_EQ(a && false, false);
	CHECK_EQ(a || true, true);
	CHECK_EQ(a || false, true);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(!b, true);
	CHECK_EQ(b && true, false);
	CHECK_EQ(b && false, false);
	CHECK_EQ(b || true, true);
	CHECK_EQ(b || false, false);
	CHECK_EQ(b = 1, 1);
	CHECK_EQ(!b, false);
	CHECK_EQ(b && true, true);
	CHECK_EQ(b && false, false);
	CHECK_EQ(b || true, true);
	CHECK_EQ(b || false, true);
}

TEST_CASE("comparison operators - int50_t/uint50_t") {
	using _ty = int50_t;
	using _tu = uint50_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = -1, -1);
	CHECK_EQ(a == -1, true);
	CHECK_EQ(a != 0, true);
	CHECK_EQ(a < 1, true);
	CHECK_EQ(a <= 1, true);
	CHECK_EQ(a <= -1, true);
	CHECK_EQ(a  > -2, true);
	CHECK_EQ(a >= -2, true);
	CHECK_EQ(a >= -1, true);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 10, 10);
	CHECK_EQ(b == 10, true);
	CHECK_EQ(b != 5, true);
	CHECK_EQ(b < 5, false);
	CHECK_EQ(b <= 5, false);
	CHECK_EQ(b <= 10, true);
	CHECK_EQ(b > 20, false);
	CHECK_EQ(b >= 20, false);
	CHECK_EQ(b >= 10, true);
}

/* int56_t/uint56_t */
TEST_CASE("type information - int56_t/uint56_t") {
	CHECK(std::numeric_limits<int56_t>::is_specialized);
	CHECK(std::numeric_limits<int56_t>::is_signed);
	CHECK(std::numeric_limits<int56_t>::is_integer);
	CHECK(std::numeric_limits<int56_t>::is_exact);
	CHECK(!std::numeric_limits<int56_t>::is_iec559);
	CHECK(std::numeric_limits<int56_t>::is_bounded);
	CHECK(!std::numeric_limits<int56_t>::is_modulo);
	CHECK(!std::numeric_limits<int56_t>::has_infinity);
	CHECK(!std::numeric_limits<int56_t>::has_quiet_NaN);
	CHECK(!std::numeric_limits<int56_t>::has_signaling_NaN);
	CHECK_EQ(std::numeric_limits<int56_t>::has_denorm, std::denorm_absent);
	CHECK(!std::numeric_limits<int56_t>::has_denorm_loss);
	CHECK_EQ(std::numeric_limits<int56_t>::digits, 55);
	CHECK_EQ(std::numeric_limits<int56_t>::digits10, 17);
	CHECK_EQ(std::numeric_limits<int56_t>::max_digits10, 0);
	CHECK_EQ(std::numeric_limits<int56_t>::radix, 2);
	CHECK_EQ(std::numeric_limits<int56_t>::max_exponent, 0);
	CHECK_EQ(std::numeric_limits<int56_t>::min_exponent, 0);
	CHECK_EQ(std::numeric_limits<int56_t>::max_exponent10, 0);
	CHECK_EQ(std::numeric_limits<int56_t>::min_exponent10, 0);
	CHECK(std::numeric_limits<int56_t>::traps);
	CHECK(!std::numeric_limits<int56_t>::tinyness_before);
	CHECK_EQ(std::numeric_limits<int56_t>::min(), -36028797018963968);
	CHECK_EQ(std::numeric_limits<int56_t>::lowest(), -36028797018963968);
	CHECK_EQ(std::numeric_limits<int56_t>::max(), 36028797018963967);
	CHECK_EQ(std::numeric_limits<int56_t>::epsilon(), 0);
	CHECK_EQ(std::numeric_limits<int56_t>::round_error(), 0);
	CHECK_EQ(std::numeric_limits<int56_t>::round_style, std::round_toward_zero);
	CHECK_EQ(std::numeric_limits<int56_t>::infinity(), 0);
	CHECK_EQ(std::numeric_limits<int56_t>::quiet_NaN(), 0);
	CHECK_EQ(std::numeric_limits<int56_t>::signaling_NaN(), 0);
	CHECK_EQ(std::numeric_limits<int56_t>::denorm_min(), 0);

	CHECK(std::numeric_limits<uint56_t>::is_specialized);
	CHECK(!std::numeric_limits<uint56_t>::is_signed);
	CHECK(std::numeric_limits<uint56_t>::is_integer);
	CHECK(std::numeric_limits<uint56_t>::is_exact);
	CHECK(!std::numeric_limits<uint56_t>::is_iec559);
	CHECK(std::numeric_limits<uint56_t>::is_bounded);
	CHECK(!std::numeric_limits<uint56_t>::is_modulo);
	CHECK(!std::numeric_limits<uint56_t>::has_infinity);
	CHECK(!std::numeric_limits<uint56_t>::has_quiet_NaN);
	CHECK(!std::numeric_limits<uint56_t>::has_signaling_NaN);
	CHECK_EQ(std::numeric_limits<uint56_t>::has_denorm, std::denorm_absent);
	CHECK(!std::numeric_limits<uint56_t>::has_denorm_loss);
	CHECK_EQ(std::numeric_limits<uint56_t>::digits, 56);
	CHECK_EQ(std::numeric_limits<uint56_t>::digits10, 17);
	CHECK_EQ(std::numeric_limits<uint56_t>::max_digits10, 0);
	CHECK_EQ(std::numeric_limits<uint56_t>::radix, 2);
	CHECK_EQ(std::numeric_limits<uint56_t>::max_exponent, 0);
	CHECK_EQ(std::numeric_limits<uint56_t>::min_exponent, 0);
	CHECK_EQ(std::numeric_limits<uint56_t>::max_exponent10, 0);
	CHECK_EQ(std::numeric_limits<uint56_t>::min_exponent10, 0);
	CHECK(std::numeric_limits<uint56_t>::traps);
	CHECK(!std::numeric_limits<uint56_t>::tinyness_before);
	CHECK_EQ(std::numeric_limits<uint56_t>::min(), 0);
	CHECK_EQ(std::numeric_limits<uint56_t>::lowest(), 0);
	CHECK_EQ(std::numeric_limits<uint56_t>::max(), 72057594037927935);
	CHECK_EQ(std::numeric_limits<uint56_t>::epsilon(), 0);
	CHECK_EQ(std::numeric_limits<uint56_t>::round_error(), 0);
	CHECK_EQ(std::numeric_limits<uint56_t>::round_style, std::round_toward_zero);
	CHECK_EQ(std::numeric_limits<uint56_t>::infinity(), 0);
	CHECK_EQ(std::numeric_limits<uint56_t>::quiet_NaN(), 0);
	CHECK_EQ(std::numeric_limits<uint56_t>::signaling_NaN(), 0);
	CHECK_EQ(std::numeric_limits<uint56_t>::denorm_min(), 0);
}

TEST_CASE("assignment operators - int56_t/uint56_t") {
	using _ty = int56_t;
	using _tu = uint56_t;

	std::int8_t _t0{};
	std::uint8_t _t1{};
	std::int16_t _t2{};
	std::uint16_t _t3{};
	std::int32_t _t4{};
	std::uint32_t _t5{};
	std::int64_t _t6{};
	std::uint64_t _t7{};

	/* Signed */
	_ty a{0};
	CHECK_EQ(a += 4, 4);
	CHECK_EQ(a -= 2, 2);
	CHECK_EQ(a *= 1, 2);
	CHECK_EQ(a /= 2, 1);
	CHECK_EQ(a %= 2, 1);
	CHECK_EQ(a = 0xA5, 0xA5);
	CHECK_EQ(a &= 0x05, 0x05);
	CHECK_EQ(a |= 0xF0, 0xF5);
	CHECK_EQ(a ^= 0xAA, 0x5F);
	CHECK_EQ(a <<= 1U, 0xBE);
	CHECK_EQ(a >>= 1U, 0x5F);
	CHECK_EQ(a = -5, -5);
	CHECK_EQ(a = std::int8_t(1), 1);
	CHECK_EQ(a = std::uint8_t(2), 2);
	CHECK_EQ(a = std::int16_t(3), 3);
	CHECK_EQ(a = std::uint16_t(4), 4);
	CHECK_EQ(a = std::int32_t(5), 5);
	CHECK_EQ(a = std::uint32_t(6), 6);
	CHECK_EQ(a = std::int64_t(7), 7);
	CHECK_EQ(a = std::uint64_t(8), 8);
	// CHECK_EQ(a = int24_t(9), 9);
	// CHECK_EQ(a = uint24_t(10), 10);
	// CHECK_EQ(a = int40_t(11), 11);
	// CHECK_EQ(a = uint40_t(12), 12);
	// CHECK_EQ(a = int48_t(13), 13);
	// CHECK_EQ(a = uint48_t(14), 14);
	// CHECK_EQ(a = int50_t(15), 15);
	// CHECK_EQ(a = uint50_t(16), 16);
	CHECK_EQ(a = int56_t(17), 17);
	// CHECK_EQ(a = uint56_t(18), 18);
	CHECK_EQ(a = 19, 19);
	CHECK_EQ(_t0 = a, 19);
	/* unsigned <- signed */
	CHECK_EQ(a = 20, 20);
	// CHECK_EQ(_t1 = a, 20);
	CHECK_EQ(a = 21, 21);
	CHECK_EQ(_t2 = a, 21);
	/* unsigned <- signed */
	CHECK_EQ(a = 22, 22);
	// CHECK_EQ(_t3 = a, 22);
	CHECK_EQ(a = 23, 23);
	CHECK_EQ(_t4 = a, 23);
	/* unsigned <- signed */
	CHECK_EQ(a = 24, 24);
	// CHECK_EQ(_t5 = a, 24);
	CHECK_EQ(a = 25, 25);
	CHECK_EQ(_t6 = a, 25);
	/* unsigned <- signed */
	CHECK_EQ(a = 26, 26);
	// CHECK_EQ(_t7 = a, 26);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b += 4, 4);
	CHECK_EQ(b -= 2, 2);
	CHECK_EQ(b *= 1, 2);
	CHECK_EQ(b /= 2, 1);
	CHECK_EQ(b %= 2, 1);
	CHECK_EQ(b = 0xA5, 0xA5);
	CHECK_EQ(b &= 0x05, 0x05);
	CHECK_EQ(b |= 0xF0, 0xF5);
	CHECK_EQ(b ^= 0xAA, 0x5F);
	CHECK_EQ(b <<= 1U, 0xBE);
	CHECK_EQ(b >>= 1U, 0x5F);
	CHECK_EQ(b = 5, 5);
	CHECK_EQ(b = std::int8_t(1), 1);
	CHECK_EQ(b = std::uint8_t(2), 2);
	CHECK_EQ(b = std::int16_t(3), 3);
	CHECK_EQ(b = std::uint16_t(4), 4);
	CHECK_EQ(b = std::int32_t(5), 5);
	CHECK_EQ(b = std::uint32_t(6), 6);
	CHECK_EQ(b = std::int64_t(7), 7);
	CHECK_EQ(b = std::uint64_t(8), 8);
	// CHECK_EQ(b = int24_t(9), 9);
	// CHECK_EQ(b = uint24_t(10), 10);
	// CHECK_EQ(b = int40_t(11), 11);
	// CHECK_EQ(b = uint40_t(12), 12);
	// CHECK_EQ(b = int48_t(13), 13);
	// CHECK_EQ(b = uint48_t(14), 14);
	// CHECK_EQ(b = int50_t(15), 15);
	// CHECK_EQ(b = uint50_t(16), 16);
	// CHECK_EQ(b = int56_t(17), 17);
	CHECK_EQ(b = uint56_t(18), 18);
	/* signed <- unsigned */
	CHECK_EQ(b = 19, 19);
	// CHECK_EQ(_t0 = b, 19);
	CHECK_EQ(b = 20, 20);
	CHECK_EQ(_t1 = b, 20);
	/* signed <- unsigned */
	CHECK_EQ(b = 21, 21);
	// CHECK_EQ(_t2 = b, 21);
	CHECK_EQ(b = 22, 22);
	CHECK_EQ(_t3 = b, 22);
	/* signed <- unsigned */
	CHECK_EQ(b = 23, 23);
	// CHECK_EQ(_t4 = b, 23);
	CHECK_EQ(b = 23, 23);
	CHECK_EQ(_t5 = b, 23);
	/* signed <- unsigned */
	CHECK_EQ(b = 24, 24);
	// CHECK_EQ(_t6 = b, 24);
	CHECK_EQ(b = 25, 25);
	CHECK_EQ(_t7 = b, 25);
}

TEST_CASE("increment operators - int56_t/uint56_t") {
	using _ty = int56_t;
	using _tu = uint56_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a++, 0);
	CHECK_EQ(a, 1);
	CHECK_EQ(++a, 2);
	CHECK_EQ(a = std::numeric_limits<_ty>::max(), std::numeric_limits<_ty>::max());
	CHECK_EQ(++a, std::numeric_limits<_ty>::min());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b++, 0);
	CHECK_EQ(b, 1);
	CHECK_EQ(++b, 2);
	CHECK_EQ(b = std::numeric_limits<_tu>::max(), std::numeric_limits<_tu>::max());
	CHECK_EQ(++b, std::numeric_limits<_tu>::min());
}

TEST_CASE("decrement operators - int56_t/uint56_t") {
	using _ty = int56_t;
	using _tu = uint56_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = 2, 2);
	CHECK_EQ(a--, 2);
	CHECK_EQ(a, 1);
	CHECK_EQ(--a, 0);
	CHECK_EQ(a = std::numeric_limits<_ty>::min(), std::numeric_limits<_ty>::min());
	CHECK_EQ(--a, std::numeric_limits<_ty>::max());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 2, 2);
	CHECK_EQ(b--, 2);
	CHECK_EQ(b, 1);
	CHECK_EQ(--b, 0);
	CHECK_EQ(b = std::numeric_limits<_tu>::min(), std::numeric_limits<_tu>::min());
	CHECK_EQ(--b, std::numeric_limits<_tu>::max());
}

TEST_CASE("arithmetic operators - int56_t/uint56_t") {
	using _ty = int56_t;
	using _tu = uint56_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = -5, -5);
	CHECK_EQ(+a, -5);
	CHECK_EQ(-a, 5);
	CHECK_EQ(a + 5, 0);
	CHECK_EQ(a - 5, -10);
	CHECK_EQ(a = -a, 5);
	CHECK_EQ(a * 2, 10);
	CHECK_EQ(a / 2, 2);
	CHECK_EQ(a % 2, 1);
	CHECK_EQ(a = 0xA5, 0xA5);
	CHECK_EQ(~a, -166);
	CHECK_EQ(a & 0x05, 0x05);
	CHECK_EQ(a | 0xF0, 0xF5);
	CHECK_EQ(a ^ 0xAA, 0x0F);
	CHECK_EQ(a << 1U, 0x14A);
	CHECK_EQ(a >> 1U, 0x52);
	CHECK_EQ(a = std::numeric_limits<_ty>::max(), std::numeric_limits<_ty>::max());
	CHECK_EQ(a + 1, std::numeric_limits<_ty>::min());
	CHECK_EQ(a = std::numeric_limits<_ty>::min(), std::numeric_limits<_ty>::min());
	CHECK_EQ(a - 1, std::numeric_limits<_ty>::max());

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 5, 5);
	CHECK_EQ(+b, 5);
	CHECK_EQ(b + 5, 10);
	CHECK_EQ(b - 5, 0);
	CHECK_EQ(b * 2, 10);
	CHECK_EQ(b / 2, 2);
	CHECK_EQ(b % 2, 1);
	CHECK_EQ(b = 0xA5, 0xA5);
	CHECK_EQ(~b, 0xFFFFFFFFFFFF5A);
	CHECK_EQ(b & 0x05, 0x05);
	CHECK_EQ(b | 0xF0, 0xF5);
	CHECK_EQ(b ^ 0xAA, 0x0F);
	CHECK_EQ(b << 1U, 0x14A);
	CHECK_EQ(b >> 1U, 0x52);
	CHECK_EQ(b = std::numeric_limits<_tu>::max(), std::numeric_limits<_tu>::max());
	CHECK_EQ(b + 1, std::numeric_limits<_tu>::min());
	CHECK_EQ(b = std::numeric_limits<_tu>::min(), std::numeric_limits<_tu>::min());
	CHECK_EQ(b - 1, std::numeric_limits<_tu>::max());
}

TEST_CASE("logical operators - int56_t/uint56_t") {
	using _ty = int56_t;
	using _tu = uint56_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(!a, true);
	CHECK_EQ(a && true, false);
	CHECK_EQ(a && false, false);
	CHECK_EQ(a || true, true);
	CHECK_EQ(a || false, false);
	CHECK_EQ(a = 1, 1);
	CHECK_EQ(!a, false);
	CHECK_EQ(a && true, true);
	CHECK_EQ(a && false, false);
	CHECK_EQ(a || true, true);
	CHECK_EQ(a || false, true);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(!b, true);
	CHECK_EQ(b && true, false);
	CHECK_EQ(b && false, false);
	CHECK_EQ(b || true, true);
	CHECK_EQ(b || false, false);
	CHECK_EQ(b = 1, 1);
	CHECK_EQ(!b, false);
	CHECK_EQ(b && true, true);
	CHECK_EQ(b && false, false);
	CHECK_EQ(b || true, true);
	CHECK_EQ(b || false, true);
}

TEST_CASE("comparison operators - int56_t/uint56_t") {
	using _ty = int56_t;
	using _tu = uint56_t;

	/* Signed */
	_ty a{};
	CHECK_EQ(a = -1, -1);
	CHECK_EQ(a == -1, true);
	CHECK_EQ(a != 0, true);
	CHECK_EQ(a < 1, true);
	CHECK_EQ(a <= 1, true);
	CHECK_EQ(a <= -1, true);
	CHECK_EQ(a  > -2, true);
	CHECK_EQ(a >= -2, true);
	CHECK_EQ(a >= -1, true);

	/* Unsigned */
	_tu b{};
	CHECK_EQ(b = 10, 10);
	CHECK_EQ(b == 10, true);
	CHECK_EQ(b != 5, true);
	CHECK_EQ(b < 5, false);
	CHECK_EQ(b <= 5, false);
	CHECK_EQ(b <= 10, true);
	CHECK_EQ(b > 20, false);
	CHECK_EQ(b >= 20, false);
	CHECK_EQ(b >= 10, true);
}
