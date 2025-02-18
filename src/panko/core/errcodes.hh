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
	/*! \enum Panko::core::error_codes::file_error_t
		\brief Possible File I/O errors
	*/
	enum struct file_error_t : std::uint16_t {
		MagicReadError = 0x0000'0000U, /*!<  Unable to read file magic */
		InvalidMagic   = 0x0000'0001U, /*!<  Invalid file magic */
		UnknownType    = 0x0000'0002U, /*!<  Unknown file type */
		ReadError      = 0x0000'0003U, /*!<  Generic file I/O read Error */
		InputExhausted = 0x0000'0004U, /*!<  Read stream exhausted but ingestion not complete */
		/* Ensure that the Unspecified error is always the last one, no matter the type */
		// NOLINTNEXTLINE(cert-int09-c)
		Unspecified = std::numeric_limits<std::underlying_type_t<file_error_t>>::max() /*!< Unspecified Error */
	};

	/*! \enum Panko::core::error_codes::strdec_error_t
		\brief Possible string decoding errors
	*/
	enum struct strdec_error_t : std::uint8_t {
		Invalid   = 0x00U, /*!< The string is invalid. */
		Malformed = 0x01U, /*!< The string is malformed. (same as Invalid?) */
		Overrun   = 0x02U, /*!< The string length overruns the buffer */
		/* Ensure that the Unspecified error is always the last one, no matter the type */
		// NOLINTNEXTLINE(cert-int09-c)
		Unspecified = std::numeric_limits<std::underlying_type_t<strdec_error_t>>::max() /*!< Unspecified Error */
	};

	/*! \enum Panko::core::error_codes::decomp_error_t
		\brief Possible data decompression errors
	*/
	enum struct decomp_error_t : std::uint8_t {
		Invalid = 0x00U, /*!< The input buffer is invalid. */
		/* Ensure that the Unspecified error is always the last one, no matter the type */
		// NOLINTNEXTLINE(cert-int09-c)
		Unspecified = std::numeric_limits<std::underlying_type_t<decomp_error_t>>::max() /*!< Unspecified Error */
	};

	/*! \brief Return a human readable string describing the File I/O error.

		This returns a non-localized string describing the meaning of the `file_error_t` if possible.

		For localization, this should be fed into the translation engine for the front-end.

		\param err The error to describe.
	*/
	[[nodiscard]]
	PANKO_API const std::string_view file_error_message(file_error_t err) noexcept;

	/*! \brief Return a human readable string describing the string decoding error.

		This returns a non-localized string describing the meaning of the `strdec_error_t` if possible.

		For localization, this should be fed into the translation engine for the front-end.

		\param err The error to describe.
	*/
	[[nodiscard]]
	PANKO_API const std::string_view strdec_error_message(strdec_error_t err) noexcept;

	/*! \brief Return a human readable string describing the decompression error.

		This returns a non-localized string describing the meaning of the `decomp_error_t` if possible.

		For localization, this should be fed into the translation engine for the front-end.

		\param err The error to describe.
	*/
	[[nodiscard]]
	PANKO_API const std::string_view decomp_error_message(decomp_error_t err) noexcept;
}

#endif /* PANKO_CORE_ERRCODES_HH */
