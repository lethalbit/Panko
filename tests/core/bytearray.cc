// SPDX-License-Identifier: BSD-3-Clause
/* bytearray.cc - bytearray_t test harness */

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <ranges>
#include <string_view>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#include "panko/config.hh"
#include "panko/core/bytearray.hh"

int main(int argc, char** argv) {
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	/* Set up the logger */
	auto logger{spdlog::stdout_color_mt("panko")};

	return context.run();
}

using Panko::core::bytearray_t;

TEST_CASE("bytearray_t - ") { }

TEST_CASE("bytearray_t - Empty and Invalid") {
	bytearray_t buff{};

	CHECK_EQ(buff.length(), 0zu);
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), false);
}

TEST_CASE("bytearray_t - Empty") {
	bytearray_t buff{256zu};

	CHECK_EQ(buff.length(), 256zu);
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);
}

TEST_CASE("bytearray_t - From raw (std::uint8_t*)") {
	constexpr static auto buff_size{256zu};
	std::unique_ptr<std::uint8_t> raw_buff{new std::uint8_t[buff_size]};


	bytearray_t buff{raw_buff.get(), buff_size};

	CHECK_EQ(buff.length(), buff_size);
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);
}

TEST_CASE("bytearray_t - From raw (std::byte*)") {
	constexpr static auto buff_size{256zu};
	std::unique_ptr<std::byte> raw_buff{new std::byte[buff_size]};

	bytearray_t buff{raw_buff.get(), buff_size};

	CHECK_EQ(buff.length(), buff_size);
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);
}

TEST_CASE("bytearray_t - From std::shared_ptr") {
	constexpr static auto buff_size{256zu};
	std::shared_ptr<std::byte> shared_buff{
		reinterpret_cast<std::byte*>(std::malloc(buff_size)), std::free
	};

	CHECK_EQ(shared_buff.use_count(), 1);

	bytearray_t buff{shared_buff, buff_size};

	CHECK_EQ(shared_buff.use_count(), 2);

	CHECK_EQ(buff.length(), buff_size);
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);
}

TEST_CASE("bytearray_t - From std::unique_ptr") {
	constexpr static auto buff_size{256zu};
	std::unique_ptr<std::byte> unique_buff{new std::byte[buff_size]};

	bytearray_t buff{unique_buff, buff_size};

	CHECK_EQ(unique_buff.get(), nullptr);

	CHECK_EQ(buff.length(), buff_size);
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);
}

TEST_CASE("bytearray_t - From std::array<std::uint8_t, ...>") {
	std::array<std::uint8_t, 256zu> array{{}};

	bytearray_t buff{array};

	CHECK_EQ(buff.length(), array.size());
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);
}

TEST_CASE("bytearray_t - From std::array<std::byte, ...>") {
	std::array<std::byte, 256zu> array{{}};

	bytearray_t buff{array};

	CHECK_EQ(buff.length(), array.size());
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);
}

TEST_CASE("bytearray_t - From std::vector<std::uint8_t>") {
	std::vector<std::uint8_t> vec{};
	vec.resize(256zu);

	bytearray_t buff{vec};

	CHECK_EQ(buff.length(), vec.size());
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);
}

TEST_CASE("bytearray_t - From std::vector<std::byte>") {
	std::vector<std::byte> vec{};
	vec.resize(256zu);

	bytearray_t buff{vec};

	CHECK_EQ(buff.length(), vec.size());
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);
}

TEST_CASE("bytearray_t - Forward iterators") {
	std::vector<std::uint8_t> vec{};
	vec.resize(256zu);

	std::generate(vec.begin(), vec.end(), [n = 0] () mutable { return n++; });

	bytearray_t buff{vec};

	CHECK_EQ(buff.length(), vec.size());
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);

	std::uint8_t e{0};
	for (auto elem : buff) {
		CHECK_EQ(static_cast<std::uint8_t>(elem), e++);
	}
}

TEST_CASE("bytearray_t - Reverse iterators") {
	std::vector<std::uint8_t> vec{};
	vec.resize(256zu);

	std::generate(vec.begin(), vec.end(), [n = 0] () mutable { return n++; });

	bytearray_t buff{vec};

	CHECK_EQ(buff.length(), vec.size());
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);

	std::uint8_t e{255};
	for (auto elem : std::ranges::views::reverse(buff)) {
		CHECK_EQ(static_cast<std::uint8_t>(elem), e--);
	}
}


TEST_CASE("bytearray_t - operator[](idx)") {
	std::vector<std::uint8_t> vec{};
	vec.resize(256zu);

	std::generate(vec.begin(), vec.end(), [n = 0] () mutable { return n++; });

	bytearray_t buff{vec};

	CHECK_EQ(buff.length(), vec.size());
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);

	/* TODO(aki): This, operator[] is broken */
}

TEST_CASE("bytearray_t - slices") {
	std::vector<std::uint8_t> vec{};
	vec.resize(256zu);

	std::generate(vec.begin(), vec.end(), [n = 0] () mutable { return n++; });

	bytearray_t buff{vec};

	CHECK_EQ(buff.length(), vec.size());
	CHECK_EQ(buff.offset(), 0zu);
	CHECK_EQ(buff.valid(), true);

	auto slice0{buff[0zu, 127zu]};
	auto slice1{buff.slice(128zu, 255zu)};

	CHECK_EQ(slice0.length(), 128zu);
	CHECK_EQ(slice1.length(), 128zu);

	CHECK_EQ(slice0.offset(), 0zu);
	CHECK_EQ(slice1.offset(), 128zu);

	CHECK_EQ(slice0.valid(), true);
	CHECK_EQ(slice1.valid(), true);

	std::uint8_t e{0};
	for (auto elem : slice0) {
		CHECK_EQ(static_cast<std::uint8_t>(elem), e++);
	}

	CHECK_EQ(e, 128);

	for (auto elem : slice1) {
		CHECK_EQ(static_cast<std::uint8_t>(elem), e++);
	}

	CHECK_EQ(e, 0);

	auto slice2{slice1[0zu, 15zu]};
	CHECK_EQ(slice2.length(), 16zu);
	CHECK_EQ(slice2.offset(), 0zu);
	CHECK_EQ(slice0.valid(), true);

	e = 128;
	for (auto elem : slice2) {
		CHECK_EQ(static_cast<std::uint8_t>(elem), e++);
	}

	CHECK_EQ(e, 144);
}

TEST_CASE("bytearray_t - at<T, endian>(idx)") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - next<T, endian>") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii_zp") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii_zt") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii_lp8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii_lp16") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii_lp24") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii_lp32") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii_lp64") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii_leb128") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii7") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii7_zp") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii7_zt") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii7_lp8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii7_lp16") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii7_lp24") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii7_lp32") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii7_lp64") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ascii7_leb128") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ebcdic") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ebcdic_zp") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ebcdic_zt") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ebcdic_lp8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ebcdic_lp16") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ebcdic_lp24") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ebcdic_lp32") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ebcdic_lp64") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_ebcdic_leb128") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf8_zp") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf8_zt") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf8_lp8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf8_lp16") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf8_lp24") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf8_lp32") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf8_lp64") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf8_leb128") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16_bom") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16_bom_zp") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16_bom_zt") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16_bom_lp8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16_bom_lp16") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16_bom_lp24") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16_bom_lp32") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16_bom_lp64") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16_bom_leb128") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16le") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16le_zp") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16le_zt") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16le_lp8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16le_lp16") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16le_lp24") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16le_lp32") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16le_lp64") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16le_leb128") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16be") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16be_zp") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16be_zt") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16be_lp8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16be_lp16") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16be_lp24") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16be_lp32") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16be_lp64") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16be_leb128") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf16_bom") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32_bom_zp") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32_bom_zt") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32_bom_lp8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32_bom_lp16") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32_bom_lp24") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32_bom_lp32") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32_bom_lp64") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32_bom_leb128") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32le") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32le_zp") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32le_zt") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32le_lp8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32le_lp16") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32le_lp24") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32le_lp32") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32le_lp64") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32le_leb128") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32be") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32be_zp") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32be_zt") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32be_lp8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32be_lp16") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32be_lp24") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32be_lp32") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32be_lp64") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_utf32be_leb128") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_shiftjis") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_shiftjis_zp") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_shiftjis_zt") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_shiftjis_lp8") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_shiftjis_lp16") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_shiftjis_lp24") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_shiftjis_lp32") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_shiftjis_lp64") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - string_shiftjis_leb128") {
	/* TODO(aki): Implement & Test */
}

#if defined(PANKO_WITH_BROTLI)
TEST_CASE("bytearray_t - decompress_brotli") {
	/* TODO(aki): Implement & Test */
}
#endif /* PANKO_WITH_BROTLI */

TEST_CASE("bytearray_t - decompress_bz2") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - decompress_deflate") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - decompress_lz4") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - decompress_lz77") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - decompress_lz77huff") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - decompress_lzma") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - decompress_lznt1") {
	/* TODO(aki): Implement & Test */
}

#if defined(PANKO_WITH_SNAPPY)
TEST_CASE("bytearray_t - decompress_snappy") {
	/* TODO(aki): Implement & Test */
}
#endif /* PANKO_WITH_SNAPPY */

TEST_CASE("bytearray_t - decompress_xz") {
	/* TODO(aki): Implement & Test */
}

TEST_CASE("bytearray_t - decompress_zstd") {
	/* TODO(aki): Implement & Test */
}
