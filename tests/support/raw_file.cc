// SPDX-License-Identifier: BSD-3-Clause
/* raw_file.cc - raw file handling, test harness */

#include <cstring>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <array>
#include <string>
#include <string_view>
#include <filesystem>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "panko/support/io/raw_file.hh"

using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

namespace fs = std::filesystem;

constexpr static std::array<char, 4> test_array{{'t', 'E', 'S', 't'}};
constexpr static char test_char{'.'};
const static auto test_string{"fileDescriptor"s};

constexpr static auto u8{std::uint8_t(0x5A)};
constexpr static auto i8{std::int8_t(0xA5U)};
constexpr static auto u16{std::uint16_t(0x125A)};
constexpr static auto i16{std::int16_t(0x12A5)};
constexpr static auto u32{std::uint32_t(UINT32_C(0x1234565A))};
constexpr static auto i32{std::int32_t(INT32_C(0x123456A5))};
constexpr static auto u64{std::uint64_t(UINT64_C(0x123456789ABCDE5A))};
constexpr static auto i64{std::int64_t(INT64_C(0x123456789ABCDEA5))};

using Panko::support::io::raw_file_t;

static std::unique_ptr<char[]> to_unique(const std::string& value) {
	auto res{std::make_unique<char[]>(value.size())};
	::memcpy(res.get(), value.data(), value.size());
	return res;
}

static std::unique_ptr<char> to_unique(const char value) {
	auto res{std::make_unique<char>()};
	*res = value;
	return res;
}

static void read_unique(const raw_file_t& file, const std::string& expected) {
	auto result{std::make_unique<char[]>(expected.size())};
	CHECK(result != nullptr);
	CHECK(file.read(result, expected.size()));
	CHECK(memcmp(result.get(), expected.data(), expected.size()) == 0);
	CHECK_FALSE(file.eof());
}

static void read_unique(const raw_file_t& file, const char expected) {
	auto result{std::make_unique<char>()};
	CHECK(result != nullptr);
	CHECK(file.read(result));
	CHECK(*result == expected);
	CHECK_FALSE(file.eof());
}

template<typename T>
static void read(const raw_file_t& file, const T& expected) {
	T result{};
	CHECK(file.read(result));
	CHECK(result == expected);
	CHECK_FALSE(file.eof());
}

template<typename T>
static void read_le(const raw_file_t& file, const T expected) {
	T result{};
	CHECK(file.read_le(result));
	CHECK(result == expected);
	CHECK_FALSE(file.eof());
}

template<typename T>
static void read_be(const raw_file_t& file, const T expected) {
	T result{};
	CHECK(file.read_be(result));
	CHECK(result == expected);
	CHECK_FALSE(file.eof());
}


TEST_CASE("raw_file_t - bad file descriptor") {
	raw_file_t file{};
	std::size_t res{};
	CHECK(file == -1);
	CHECK_FALSE(file.valid());
	CHECK_FALSE(file.eof());

	CHECK(file.read(nullptr, 0, nullptr) == -1);
	CHECK_FALSE(file.read(nullptr, 0, res));
	CHECK(file.write(nullptr, 0, nullptr) == -1);
	CHECK_FALSE(file.write(nullptr, 0));
	CHECK_FALSE(file.tail());
}

TEST_CASE("raw_file_t bad open - c string") {
	raw_file_t file("invalid.file", O_RDONLY);
	CHECK(file == -1);
	CHECK_FALSE(file.valid());
	CHECK_FALSE(file.eof());
}

TEST_CASE("raw_file_t bad open - std::string") {
	raw_file_t file("invalid.file"s, O_RDONLY);
	CHECK(file == -1);
	CHECK_FALSE(file.valid());
	CHECK_FALSE(file.eof());
}

TEST_CASE("raw_file_t bad open - std::string_view") {
	raw_file_t file("invalid.file"sv, O_RDONLY);
	CHECK(file == -1);
	CHECK_FALSE(file.valid());
	CHECK_FALSE(file.eof());
}

TEST_CASE("raw_file_t bad open - std::filesystem::path") {
	fs::path file_name{"invalid.file"sv};

	raw_file_t file(file_name, O_RDONLY);
	CHECK(file == -1);
	CHECK_FALSE(file.valid());
	CHECK_FALSE(file.eof());
}

TEST_CASE("raw_file_t - write") {
	raw_file_t file{"file.test", O_WRONLY | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH};

	CHECK(file >= 0);
	CHECK(file.valid());
	CHECK_FALSE(file.eof());

	CHECK(file.write(test_array));
	CHECK(file.write(test_char));
	auto arrptr{to_unique(test_string)};
	CHECK(file.write(arrptr, test_string.size()));
	auto objptr{to_unique(test_char)};
	CHECK(file.write(objptr));
	CHECK(file.write(u8));
	CHECK(file.write(i8));
	CHECK(file.write_le(u16));
	CHECK(file.write_le(i16));
	CHECK(file.write_le(u32));
	CHECK(file.write_le(i32));
	CHECK(file.write_le(u64));
	CHECK(file.write_le(i64));
	CHECK(file.write_be(u16));
	CHECK(file.write_be(i16));
	CHECK(file.write_be(u32));
	CHECK(file.write_be(i32));
	CHECK(file.write_be(u64));
	CHECK(file.write_be(i64));
}

TEST_CASE("raw_file_t - seek") {
	raw_file_t file{"file.test", O_RDONLY};

	CHECK(file >= 0);
	CHECK(file.valid());
	CHECK_FALSE(file.eof());
	CHECK(file.tell() == 0);

	const auto len{file.length()};
	CHECK(len == 78);
	CHECK(file.tail());
	CHECK(file.eof());
	CHECK(file.tell() == len);

	CHECK(file.seek(-(len / 2), SEEK_CUR) == len / 2);
	CHECK_FALSE(file.eof());
	CHECK(file.seek(0, SEEK_END) == len);
	CHECK(file.eof());
	CHECK(file.head());
	CHECK_FALSE(file.eof());

	raw_file_t other{std::move(file)};

	// NOLINTNEXTLINE(bugprone-use-after-move,clang-analyzer-cplusplus.Move)
	CHECK(!file.valid());
	CHECK(other.valid());
}

TEST_CASE("raw_file_t - read") {
	raw_file_t file{"file.test", O_RDONLY};

	CHECK(file >= 0);
	CHECK(file.valid());
	CHECK_FALSE(file.eof());
	CHECK(file.tell() == 0);

	read(file, test_array);
	read(file, test_char);
	read_unique(file, test_string);
	read_unique(file, test_char);
	read(file, u8);
	read(file, i8);
	read_le(file, u16);
	read_le(file, i16);
	read_le(file, u32);
	read_le(file, i32);
	read_le(file, u64);
	read_le(file, i64);
	read_be(file, u16);
	read_be(file, i16);
	read_be(file, u32);
	read_be(file, i32);
	read_be(file, u64);
	read_be(file, i64);

	char junk{};
	CHECK_FALSE(file.read(junk));
	CHECK(file.eof());
	CHECK_FALSE(file.read(junk));
	CHECK(file.eof());
}

// Cleanup
TEST_CASE("raw_file_t tests cleanup") {
	::unlink("file.test");
	CHECK(true);
}
