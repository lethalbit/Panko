// SPDX-License-Identifier: BSD-3-Clause
/* errcodes.cc - Error codes */

#include <cstdint>
#include <string_view>

#include "panko/core/errcodes.hh"

namespace Panko::core::error_codes {
	using namespace std::literals::string_view_literals;

	const std::string_view file_error_message(file_error_t err) noexcept {
		switch (err) {
			case file_error_t::MagicReadError: {
				return "Unable to read file magic"sv;
			} case file_error_t::InvalidMagic: {
				return "Invalid file magic"sv;
			} case file_error_t::UnknownType: {
				return "Unknown file type"sv;
			} case file_error_t::ReadError: {
				return "File I/O Error"sv;
			} case file_error_t::InputExhausted: {
				return "Input exhausted, but did not complete import"sv;
			} case file_error_t::Unspecified:
			default: {
				return "Unspecified file error"sv;
			}
		}
	}

	const std::string_view strdec_error_message(strdec_error_t err) noexcept {
		switch (err) {
			case strdec_error_t::Invalid: {
				return "String is not valid for the given encoding"sv;
			} case strdec_error_t::Malformed: {
				return "String is malformed"sv;
			} case strdec_error_t::Unspecified:
			default: {
				return "Unspecified string decoding error"sv;
			}
		}
	}

	const std::string_view decomp_error_message(decomp_error_t err) noexcept {
		switch (err) {
			case decomp_error_t::Invalid: {
				return "Input buffer is not valid compressed data"sv;
			} case decomp_error_t::Unspecified:
			default: {
				return "Unspecified string decoding error"sv;
			}
		}
	}
}
