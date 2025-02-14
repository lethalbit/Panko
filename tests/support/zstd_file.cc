// SPDX-License-Identifier: BSD-3-Clause
/* zstd_file.cc - zstd file handling, test harness */

#include <filesystem>
#include <variant>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "panko/support/file.hh"
#include "panko/support/io/compressed/zstd_file.hh"

namespace fs = std::filesystem;

using Panko::support::io::compressed::zstd_file_t;

const static auto TEST_DATA_PATH{fs::path(PANKO_TEST_DATA_DIR)};
const static auto ZST_PCAP{TEST_DATA_PATH / "test0.pcapng.zst"};
