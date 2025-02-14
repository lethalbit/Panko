// SPDX-License-Identifier: BSD-3-Clause
/* file.cc - file handling, test harness */

#include <filesystem>
#include <variant>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "panko/support/file.hh"
#include "panko/support/io/raw_file.hh"
#include "panko/support/io/compressed/bz2_file.hh"
#include "panko/support/io/compressed/gzip_file.hh"
#include "panko/support/io/compressed/lz4_file.hh"
#include "panko/support/io/compressed/lzma_file.hh"
#include "panko/support/io/compressed/xz_file.hh"
#include "panko/support/io/compressed/zstd_file.hh"

namespace fs = std::filesystem;

using Panko::support::io::raw_file_t;
using Panko::support::io::compressed::bz2_file_t;
using Panko::support::io::compressed::gzip_file_t;
using Panko::support::io::compressed::lz4_file_t;
using Panko::support::io::compressed::lzma_file_t;
using Panko::support::io::compressed::xz_file_t;
using Panko::support::io::compressed::zstd_file_t;

const static auto TEST_DATA_PATH{fs::path(PANKO_TEST_DATA_DIR)};

const static auto RAW_PCAP{TEST_DATA_PATH / "test0.pcapng"};
const static auto BZ2_PCAP{TEST_DATA_PATH / "test0.pcapng.bz2"};
const static auto GZ_PCAP{TEST_DATA_PATH / "test0.pcapng.gz"};
const static auto LZ4_PCAP{TEST_DATA_PATH / "test0.pcapng.lz4"};
const static auto LZMA_PCAP{TEST_DATA_PATH / "test0.pcapng.lzma"};
const static auto XZ_PCAP{TEST_DATA_PATH / "test0.pcapng.xz"};
const static auto ZST_PCAP{TEST_DATA_PATH / "test0.pcapng.zst"};


TEST_CASE("Raw File") {
	auto res{Panko::support::open(RAW_PCAP)};
	CHECK(res.has_value());
	CHECK(std::holds_alternative<raw_file_t>(*res));
	CHECK(!std::holds_alternative<bz2_file_t>(*res));
	CHECK(!std::holds_alternative<gzip_file_t>(*res));
	CHECK(!std::holds_alternative<lz4_file_t>(*res));
	CHECK(!std::holds_alternative<lzma_file_t>(*res));
	CHECK(!std::holds_alternative<xz_file_t>(*res));
	CHECK(!std::holds_alternative<zstd_file_t>(*res));

	auto file{std::get<raw_file_t>(std::move(*res))};
	CHECK(file.valid());
	CHECK(!file.eof());
	CHECK(file.tell() == 0UL);
}

TEST_CASE("bzip2 Compressed File") {
	auto res{Panko::support::open(BZ2_PCAP)};
	CHECK(res.has_value());
	CHECK(!std::holds_alternative<raw_file_t>(*res));
	CHECK(std::holds_alternative<bz2_file_t>(*res));
	CHECK(!std::holds_alternative<gzip_file_t>(*res));
	CHECK(!std::holds_alternative<lz4_file_t>(*res));
	CHECK(!std::holds_alternative<lzma_file_t>(*res));
	CHECK(!std::holds_alternative<xz_file_t>(*res));
	CHECK(!std::holds_alternative<zstd_file_t>(*res));

	auto file{std::get<bz2_file_t>(std::move(*res))};
	CHECK(file.valid());
	CHECK(!file.eof());
	CHECK(file.tell() == 0UL);
}

TEST_CASE("gzip Compressed File") {
	auto res{Panko::support::open(GZ_PCAP)};
	CHECK(res.has_value());
	CHECK(!std::holds_alternative<raw_file_t>(*res));
	CHECK(!std::holds_alternative<bz2_file_t>(*res));
	CHECK(std::holds_alternative<gzip_file_t>(*res));
	CHECK(!std::holds_alternative<lz4_file_t>(*res));
	CHECK(!std::holds_alternative<lzma_file_t>(*res));
	CHECK(!std::holds_alternative<xz_file_t>(*res));
	CHECK(!std::holds_alternative<zstd_file_t>(*res));

	auto file{std::get<gzip_file_t>(std::move(*res))};
	CHECK(file.valid());
	CHECK(!file.eof());
	CHECK(file.tell() == 0UL);
}

TEST_CASE("lz4 Compressed File") {
	auto res{Panko::support::open(LZ4_PCAP)};
	CHECK(res.has_value());
	CHECK(!std::holds_alternative<raw_file_t>(*res));
	CHECK(!std::holds_alternative<bz2_file_t>(*res));
	CHECK(!std::holds_alternative<gzip_file_t>(*res));
	CHECK(std::holds_alternative<lz4_file_t>(*res));
	CHECK(!std::holds_alternative<lzma_file_t>(*res));
	CHECK(!std::holds_alternative<xz_file_t>(*res));
	CHECK(!std::holds_alternative<zstd_file_t>(*res));

	auto file{std::get<lz4_file_t>(std::move(*res))};
	CHECK(file.valid());
	CHECK(!file.eof());
	CHECK(file.tell() == 0UL);
}

TEST_CASE("lzma Compressed File") {
	auto res{Panko::support::open(LZMA_PCAP)};
	CHECK(res.has_value());
	CHECK(!std::holds_alternative<raw_file_t>(*res));
	CHECK(!std::holds_alternative<bz2_file_t>(*res));
	CHECK(!std::holds_alternative<gzip_file_t>(*res));
	CHECK(!std::holds_alternative<lz4_file_t>(*res));
	CHECK(std::holds_alternative<lzma_file_t>(*res));
	CHECK(!std::holds_alternative<xz_file_t>(*res));
	CHECK(!std::holds_alternative<zstd_file_t>(*res));

	auto file{std::get<lzma_file_t>(std::move(*res))};
	CHECK(file.valid());
	CHECK(!file.eof());
	CHECK(file.tell() == 0UL);
}

TEST_CASE("XZ Compressed File") {
	auto res{Panko::support::open(XZ_PCAP)};
	CHECK(res.has_value());
	CHECK(!std::holds_alternative<raw_file_t>(*res));
	CHECK(!std::holds_alternative<bz2_file_t>(*res));
	CHECK(!std::holds_alternative<gzip_file_t>(*res));
	CHECK(!std::holds_alternative<lz4_file_t>(*res));
	CHECK(!std::holds_alternative<lzma_file_t>(*res));
	CHECK(std::holds_alternative<xz_file_t>(*res));
	CHECK(!std::holds_alternative<zstd_file_t>(*res));

	auto file{std::get<xz_file_t>(std::move(*res))};
	CHECK(file.valid());
	CHECK(!file.eof());
	CHECK(file.tell() == 0UL);
}

TEST_CASE("zstd Compressed File") {
	auto res{Panko::support::open(ZST_PCAP)};
	CHECK(res.has_value());
	CHECK(!std::holds_alternative<raw_file_t>(*res));
	CHECK(!std::holds_alternative<bz2_file_t>(*res));
	CHECK(!std::holds_alternative<gzip_file_t>(*res));
	CHECK(!std::holds_alternative<lz4_file_t>(*res));
	CHECK(!std::holds_alternative<lzma_file_t>(*res));
	CHECK(!std::holds_alternative<xz_file_t>(*res));
	CHECK(std::holds_alternative<zstd_file_t>(*res));

	auto file{std::get<zstd_file_t>(std::move(*res))};
	CHECK(file.valid());
	CHECK(!file.eof());
	CHECK(file.tell() == 0UL);
}
