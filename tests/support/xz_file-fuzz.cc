// SPDX-License-Identifier: BSD-3-Clause
/* xz_file-fuzz.cc - XZ file handling, fuzzing harness */

#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t *data, std::size_t size) {

	return 0;
}
