// SPDX-License-Identifier: BSD-3-Clause
/* lzma_file-fuzz.cc - LZMA file handling, fuzzing harness */

#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t *data, std::size_t size) {

	return 0;
}
