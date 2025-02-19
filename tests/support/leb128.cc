// SPDX-License-Identifier: BSD-3-Clause
/* leb128.cc - LEB128 encode/decode test harness */

#include <cstdint>
#include <random>
#include <limits>
#include <vector>
#include <ostream>


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

namespace doctest {
    template<typename T>
	struct StringMaker<std::vector<T>> {
        static String convert(const std::vector<T>& value) {
			std::ostringstream stream;
			stream << "{ ";
			for (auto& val : value) {
				stream << "0x" << std::hex << static_cast<std::uint16_t>(val) << " ";
			}
			stream << "}";
            return stream.str().c_str();
        }
    };
}

#include "panko/support/leb128.hh"

std::random_device rand_dev{};
/* NOTE(aki): uniform_int_distribution<int8_t> is UB */
std::uniform_int_distribution<std::int16_t>  int8_dist{
	std::numeric_limits<std::int8_t>::min(),
	std::numeric_limits<std::int8_t>::max()
};
std::uniform_int_distribution<std::int16_t> int16_dist{std::numeric_limits<std::int16_t>::min()};
std::uniform_int_distribution<std::int32_t> int32_dist{std::numeric_limits<std::int32_t>::min()};
std::uniform_int_distribution<std::int64_t> int64_dist{std::numeric_limits<std::int64_t>::min()};
/* NOTE(aki): uniform_int_distribution<uint8_t> is UB */
std::uniform_int_distribution<std::uint16_t>  uint8_dist{
	std::numeric_limits<std::uint8_t>::min(),
	std::numeric_limits<std::uint8_t>::max()
};
std::uniform_int_distribution<std::uint16_t> uint16_dist{std::numeric_limits<std::uint16_t>::min()};
std::uniform_int_distribution<std::uint32_t> uint32_dist{std::numeric_limits<std::uint32_t>::min()};
std::uniform_int_distribution<std::uint64_t> uint64_dist{std::numeric_limits<std::uint64_t>::min()};

using Panko::support::leb128_encode;
using Panko::support::leb128_decode;

constexpr static std::size_t chk_itr{128zu};

TEST_CASE("leb128 - encode - int8_t (neg)") {
	const std::int8_t input{-5};
	const std::vector<std::uint8_t> expected{{0x7B}};
	const auto result{leb128_encode<std::int8_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - encode - int16_t (neg)") {
	const std::int16_t input{-32568};
	const std::vector<std::uint8_t> expected{{0xC8, 0x81, 0x7E}};
	const auto result{leb128_encode<std::int16_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - encode - int32_t (neg)") {
	const std::int32_t input{-21574836};
	const std::vector<std::uint8_t> expected{{0xCC, 0x96, 0xDB, 0x75}};
	const auto result{leb128_encode<std::int32_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - encode - int64_t (neg)") {
	const std::int64_t input{-237240687758};
	const std::vector<std::uint8_t> expected{{0xF2, 0xC6, 0xE9, 0x9A, 0x7C}};
	const auto result{leb128_encode<std::int64_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - encode - int8_t (pos)") {
	const std::int8_t input{18};
	const std::vector<std::uint8_t> expected{{0x12}};
	const auto result{leb128_encode<std::int8_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - encode - int16_t (pos)") {
	const std::int16_t input{25535};
	const std::vector<std::uint8_t> expected{{0xBF, 0xC7, 0x01}};
	const auto result{leb128_encode<std::int16_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - encode - int32_t (pos)") {
	const std::int32_t input{114848627};
	const std::vector<std::uint8_t> expected{{0xF3, 0xE6, 0xE1, 0x36}};
	const auto result{leb128_encode<std::int32_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - encode - int64_t (pos)") {
	const std::int64_t input{823374036884795207};
	const std::vector<std::uint8_t> expected{{0xC7, 0xDE, 0xB0, 0x4E}};
	const auto result{leb128_encode<std::int64_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - encode - uint8_t") {
	const std::uint8_t input{29};
	const std::vector<std::uint8_t> expected{{0x1D}};
	const auto result{leb128_encode<std::uint8_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - encode - uint16_t") {
	const std::uint16_t input{46637};
	const std::vector<std::uint8_t> expected{{0xAD, 0xEC, 0x02}};
	const auto result{leb128_encode<std::uint16_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - encode - uint32_t") {
	const std::uint32_t input{327899725};
	const std::vector<std::uint8_t> expected{{0xCD, 0xB4, 0xAD, 0x9C, 0x01}};
	const auto result{leb128_encode<std::uint32_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - encode - uint64_t") {
	const std::uint64_t input{946747370551615};
	const std::vector<std::uint8_t> expected{{0xBF, 0xF2, 0xDA, 0xE5, 0x0C}};
	const auto result{leb128_encode<std::uint64_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - int8_t (neg)") {
	const std::vector<std::uint8_t> input{{0x49}};
	const std::int8_t expected{-55};
	const auto result{leb128_decode<std::int8_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - int16_t (neg)") {
	const std::vector<std::uint8_t> input{{0xCE, 0xCD, 0x7E}};
	const std::int16_t expected{-22834};
	const auto result{leb128_decode<std::int16_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - int32_t (neg)") {
	const std::vector<std::uint8_t> input{{0x90, 0xF1, 0x9B, 0x8C, 0x7F}};
	const std::int32_t expected{-242812784};
	const auto result{leb128_decode<std::int32_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - int64_t (neg)") {
	const std::vector<std::uint8_t> input{{0x96, 0xA2, 0xE0, 0x5D}};
	const std::int64_t expected{-9293261328346858};
	const auto result{leb128_decode<std::int64_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - int8_t (pos)") {
	const std::vector<std::uint8_t> input{{0xEE, 0x00}};
	const std::int8_t expected{110};
	const auto result{leb128_decode<std::int8_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - int16_t (pos)") {
	const std::vector<std::uint8_t> input{{0xB3, 0xF7, 0x01}};
	const std::int16_t expected{31667};
	const auto result{leb128_decode<std::int16_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - int32_t (pos)") {
	const std::vector<std::uint8_t> input{{0xBD, 0xE5, 0xD7, 0xA2, 0x04}};
	const std::int32_t expected{1146483389};
	const auto result{leb128_decode<std::int32_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - int64_t (pos)") {
	const std::vector<std::uint8_t> input{{0x99, 0x9B, 0xEA, 0xA5, 0x7D}};
	const std::int64_t expected{92127803687472537};
	const auto result{leb128_decode<std::int64_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - uint8_t") {
	const std::vector<std::uint8_t> input{{0xF0, 0x01}};
	const std::uint8_t expected{240};
	const auto result{leb128_decode<std::uint8_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - uint16_t") {
	const std::vector<std::uint8_t> input{{0x95, 0xA9, 0x03}};
	const std::uint16_t expected{54421};
	const auto result{leb128_decode<std::uint16_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - uint32_t") {
	const std::vector<std::uint8_t> input{{0x93, 0xAE, 0x99, 0xC6, 0x08}};
	const std::uint32_t expected{2294699795};
	const auto result{leb128_decode<std::uint32_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - decode - uint64_t") {
	const std::vector<std::uint8_t> input{{0xA9, 0xB4, 0x83, 0xB8, 0x0A}};
	const std::uint64_t expected{1246244098709445161};
	const auto result{leb128_decode<std::uint64_t>(input)};
	CHECK_EQ(expected, result);
}

TEST_CASE("leb128 - round-trip - int8_t") {
	auto input_value{static_cast<std::int8_t>(int8_dist(rand_dev))};
	/* Do one round */
	auto enc{leb128_encode<std::int8_t>(input_value)};
	auto dec{leb128_decode<std::int8_t>(enc)};
	CHECK_EQ(input_value, dec);

	/* Now check for numerical stability */
	for (std::size_t i{}; i < chk_itr; ++i) {
		enc = leb128_encode<std::int8_t>(dec);
		dec = leb128_decode<std::int8_t>(enc);
	}
	CHECK_EQ(input_value, dec);

	/* Now make sure we work over a wide distribution */
	for (std::size_t i{}; i < (chk_itr * 4zu); i++) {
		auto input{static_cast<std::int8_t>(int8_dist(rand_dev))};
		auto enc_val{leb128_encode<std::int8_t>(input)};
		auto dec_val{leb128_decode<std::int8_t>(enc_val)};
		CHECK_EQ(input, dec_val);
	}
}

TEST_CASE("leb128 - round-trip - int16_t") {
	auto input_value{int16_dist(rand_dev)};
	/* Do one round */
	auto enc{leb128_encode<std::int16_t>(input_value)};
	auto dec{leb128_decode<std::int16_t>(enc)};
	CHECK_EQ(input_value, dec);

	/* Now check for numerical stability */
	for (std::size_t i{}; i < chk_itr; ++i) {
		enc = leb128_encode<std::int16_t>(dec);
		dec = leb128_decode<std::int16_t>(enc);
	}
	CHECK_EQ(input_value, dec);

	/* Now make sure we work over a wide distribution */
	for (std::size_t i{}; i < (chk_itr * 4zu); i++) {
		auto input{int16_dist(rand_dev)};
		auto enc_val{leb128_encode<std::int16_t>(input)};
		auto dec_val{leb128_decode<std::int16_t>(enc_val)};
		CHECK_EQ(input, dec_val);
	}
}

TEST_CASE("leb128 - round-trip - int32_t") {
	auto input_value{int32_dist(rand_dev)};
	/* Do one round */
	auto enc{leb128_encode<std::int32_t>(input_value)};
	auto dec{leb128_decode<std::int32_t>(enc)};
	CHECK_EQ(input_value, dec);

	/* Now check for numerical stability */
	for (std::size_t i{}; i < chk_itr; ++i) {
		enc = leb128_encode<std::int32_t>(dec);
		dec = leb128_decode<std::int32_t>(enc);
	}
	CHECK_EQ(input_value, dec);

	/* Now make sure we work over a wide distribution */
	for (std::size_t i{}; i < (chk_itr * 4zu); i++) {
		auto input{int32_dist(rand_dev)};
		auto enc_val{leb128_encode<std::int32_t>(input)};
		auto dec_val{leb128_decode<std::int32_t>(enc_val)};
		CHECK_EQ(input, dec_val);
	}
}

TEST_CASE("leb128 - round-trip - int64_t") {
	auto input_value{int64_dist(rand_dev)};
	/* Do one round */
	auto enc{leb128_encode<std::int64_t>(input_value)};
	auto dec{leb128_decode<std::int64_t>(enc)};
	CHECK_EQ(input_value, dec);

	/* Now check for numerical stability */
	for (std::size_t i{}; i < chk_itr; ++i) {
		enc = leb128_encode<std::int64_t>(dec);
		dec = leb128_decode<std::int64_t>(enc);
	}
	CHECK_EQ(input_value, dec);

	/* Now make sure we work over a wide distribution */
	for (std::size_t i{}; i < (chk_itr * 4zu); i++) {
		auto input{int64_dist(rand_dev)};
		auto enc_val{leb128_encode<std::int64_t>(input)};
		auto dec_val{leb128_decode<std::int64_t>(enc_val)};
		CHECK_EQ(input, dec_val);
	}
}

TEST_CASE("leb128 - round-trip - uint8_t") {
	auto input_value{static_cast<std::uint8_t>(uint8_dist(rand_dev))};
	/* Do one round */
	auto enc{leb128_encode<std::uint8_t>(input_value)};
	auto dec{leb128_decode<std::uint8_t>(enc)};
	CHECK_EQ(input_value, dec);

	/* Now check for numerical stability */
	for (std::size_t i{}; i < chk_itr; ++i) {
		enc = leb128_encode<std::uint8_t>(dec);
		dec = leb128_decode<std::uint8_t>(enc);
	}
	CHECK_EQ(input_value, dec);

	/* Now make sure we work over a wide distribution */
	for (std::size_t i{}; i < (chk_itr * 4zu); i++) {
		auto input{static_cast<std::uint8_t>(uint8_dist(rand_dev))};
		auto enc_val{leb128_encode<std::uint8_t>(input)};
		auto dec_val{leb128_decode<std::uint8_t>(enc_val)};
		CHECK_EQ(input, dec_val);
	}
}

TEST_CASE("leb128 - round-trip - uint16_t") {
	auto input_value{uint16_dist(rand_dev)};
	/* Do one round */
	auto enc{leb128_encode<std::uint16_t>(input_value)};
	auto dec{leb128_decode<std::uint16_t>(enc)};
	CHECK_EQ(input_value, dec);

	/* Now check for numerical stability */
	for (std::size_t i{}; i < chk_itr; ++i) {
		enc = leb128_encode<std::uint16_t>(dec);
		dec = leb128_decode<std::uint16_t>(enc);
	}
	CHECK_EQ(input_value, dec);

	/* Now make sure we work over a wide distribution */
	for (std::size_t i{}; i < (chk_itr * 4zu); i++) {
		auto input{uint16_dist(rand_dev)};
		auto enc_val{leb128_encode<std::uint16_t>(input)};
		auto dec_val{leb128_decode<std::uint16_t>(enc_val)};
		CHECK_EQ(input, dec_val);
	}
}

TEST_CASE("leb128 - round-trip - uint32_t") {
	auto input_value{uint32_dist(rand_dev)};
	/* Do one round */
	auto enc{leb128_encode<std::uint32_t>(input_value)};
	auto dec{leb128_decode<std::uint32_t>(enc)};
	CHECK_EQ(input_value, dec);

	/* Now check for numerical stability */
	for (std::size_t i{}; i < chk_itr; ++i) {
		enc = leb128_encode<std::uint32_t>(dec);
		dec = leb128_decode<std::uint32_t>(enc);
	}
	CHECK_EQ(input_value, dec);

	/* Now make sure we work over a wide distribution */
	for (std::size_t i{}; i < (chk_itr * 4zu); i++) {
		auto input{uint32_dist(rand_dev)};
		auto enc_val{leb128_encode<std::uint32_t>(input)};
		auto dec_val{leb128_decode<std::uint32_t>(enc_val)};
		CHECK_EQ(input, dec_val);
	}
}

TEST_CASE("leb128 - round-trip - uint64_t") {
	auto input_value{uint64_dist(rand_dev)};
	/* Do one round */
	auto enc{leb128_encode<std::uint64_t>(input_value)};
	auto dec{leb128_decode<std::uint64_t>(enc)};
	CHECK_EQ(input_value, dec);

	/* Now check for numerical stability */
	for (std::size_t i{}; i < chk_itr; ++i) {
		enc = leb128_encode<std::uint64_t>(dec);
		dec = leb128_decode<std::uint64_t>(enc);
	}
	CHECK_EQ(input_value, dec);

	/* Now make sure we work over a wide distribution */
	for (std::size_t i{}; i < (chk_itr * 4zu); i++) {
		auto input{uint64_dist(rand_dev)};
		auto enc_val{leb128_encode<std::uint64_t>(input)};
		auto dec_val{leb128_decode<std::uint64_t>(enc_val)};
		CHECK_EQ(input, dec_val);
	}
}
