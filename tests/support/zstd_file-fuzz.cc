// SPDX-License-Identifier: BSD-3-Clause
/* zstd_file-fuzz.cc - zstd file handling, fuzzing harness */

#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t *data, std::size_t size) {

	return 0;
}
