// SPDX-License-Identifier: BSD-3-Clause
/* gzip_file.cc - gzip file handling, test harness */

#include <filesystem>
#include <variant>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "panko/support/file.hh"
#include "panko/support/io/compressed/gzip_file.hh"

namespace fs = std::filesystem;

using Panko::support::io::compressed::gzip_file_t;

const static auto TEST_DATA_PATH{fs::path(PANKO_TEST_DATA_DIR)};
const static auto GZ_PCAP{TEST_DATA_PATH / "test0.pcapng.gz"};
