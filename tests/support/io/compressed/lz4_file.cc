// SPDX-License-Identifier: BSD-3-Clause
/* lz4_file.cc - lz4 file handling, test harness */

#include <filesystem>
#include <variant>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "panko/support/file.hh"
#include "panko/support/io/compressed/lz4_file.hh"

namespace fs = std::filesystem;

using Panko::support::io::compressed::lz4_file_t;

const static auto TEST_DATA_PATH{fs::path(PANKO_TEST_DATA_DIR)};
const static auto LZ4_PCAP{TEST_DATA_PATH / "test0.pcapng.lz4"};
