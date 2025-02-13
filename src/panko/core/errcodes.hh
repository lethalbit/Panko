// SPDX-License-Identifier: BSD-3-Clause
/* errcodes.hh - Error codes */

#pragma once
#if !defined(PANKO_CORE_ERRCODES_HH)
#define PANKO_CORE_ERRCODES_HH

#include <cstdint>
#include <string_view>
#include <limits>

#include "panko/internal/defs.hh"

namespace Panko::core::error_codes {
	enum struct file_error_t : std::uint16_t {
		MagicReadError = 0x0000'0000, // Unable to read file magic
		InvalidMagic   = 0x0000'0001, // Invalid file magic
		UnknownType    = 0x0000'0002, // Unknown file type
		ReadError      = 0x0000'0003, // Generic file I/O read Error
		InputExhausted = 0x0000'0004, // Read stream exhausted but ingestion not complete
		/* Ensure that the Unspecified error is always the last one, no matter the type */
		// NOLINTNEXTLINE(cert-int09-c)
		Unspecified = std::numeric_limits<std::underlying_type_t<file_error_t>>::max()
	};

	[[nodiscard]]
	PANKO_API const std::string_view file_error_message(file_error_t err) noexcept;
}

#endif /* PANKO_CORE_ERRCODES_HH */
