// SPDX-License-Identifier: BSD-3-Clause
/* bytearray.cc - Flexble and subdividable byte array */

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <expected>
#include <limits>
#include <memory>
#include <optional>
#include <span>
#include <stdexcept>
#include <stdfloat>
#include <string_view>
#include <vector>

#include <spdlog/spdlog.h>

#include "panko/internal/defs.hh"
#include "panko/core/types.hh"
#include "panko/core/bytearray.hh"
#include "panko/core/errcodes.hh"
#include "panko/core/integers.hh"
#include "panko/core/strutils.hh"

namespace Panko::core {
	using Panko::core::error_codes::strdec_error_t;
	using Panko::core::error_codes::decomp_error_t;

	/* 8-bit ASCII */

	/* string_ascii - Fixed length 8-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii_zp - Fixed length null-padded 8-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii_zp(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii_zt - Zero/Null terminated 8-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii_zt(const std::size_t idx, const std::size_t max_len) {
		(void)idx;
		(void)max_len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii_lp8 - 8bit length prefixed 8-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii_lp8(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii_lp16 - 16bit length prefixed 8-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii_lp16(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii_lp24 - 24bit length prefixed 8-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii_lp24(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii_lp32 - 32bit length prefixed 8-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii_lp32(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii_lp64 - 64bit length prefixed 8-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii_lp64(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii_leb128 - LEB128 length prefixed 8-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii_leb128(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}


	/* 7-bit ASCII */

	/* string_ascii7 - Fixed length 7-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii7(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii7_zt - Zero/Null terminated 7-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii7_zp(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii7_zp - Fixed length null-padded 7-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii7_zt(const std::size_t idx, const std::size_t max_len) {
		(void)idx;
		(void)max_len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii7_lp8 - 8bit length prefixed 7-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii7_lp8(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii7_lp16 - 16bit length prefixed 7-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii7_lp16(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii7_lp24 - 24bit length prefixed 7-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii7_lp24(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii7_lp32 - 32bit length prefixed 7-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii7_lp32(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii7_lp64 - 64bit length prefixed 7-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii7_lp64(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ascii7_leb128 - LEB128 length prefixed 7-bit ASCII */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ascii7_leb128(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}


	/* EBCDIC */

	/* string_ebcdic - Fixed length terminated EBCDIC */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ebcdic(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ebcdic_zt - Zero/Null terminated EBCDIC */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ebcdic_zp(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ebcdic_zp - Fixed length null-padded EBCDIC */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ebcdic_zt(const std::size_t idx, const std::size_t max_len) {
		(void)idx;
		(void)max_len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ebcdic_lp8 - 8bit length prefixed EBCDIC */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ebcdic_lp8(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ebcdic_lp16 - 16bit length prefixed EBCDIC */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ebcdic_lp16(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ebcdic_lp24 - 24bit length prefixed EBCDIC */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ebcdic_lp24(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ebcdic_lp32 - 32bit length prefixed EBCDIC */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ebcdic_lp32(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ebcdic_lp64 - 64bit length prefixed EBCDIC */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ebcdic_lp64(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_ebcdic_leb128 - LEB128 length prefixed EBCDIC */
	std::expected<std::string_view, strdec_error_t> bytearray_t::string_ebcdic_leb128(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}


	/* UTF-8 */

	/* string_utf8 - Fixed length terminated UTF-8 */
	std::expected<std::u8string_view, strdec_error_t> bytearray_t::string_utf8(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf8_zt - Zero/Null terminated UTF-8 */
	std::expected<std::u8string_view, strdec_error_t> bytearray_t::string_utf8_zp(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf8_zp - Fixed length null-padded UTF-8 */
	std::expected<std::u8string_view, strdec_error_t> bytearray_t::string_utf8_zt(const std::size_t idx, const std::size_t max_len) {
		(void)idx;
		(void)max_len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf8_lp8 - 8bit length prefixed UTF-8 */
	std::expected<std::u8string_view, strdec_error_t> bytearray_t::string_utf8_lp8(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf8_lp16 - 16bit length prefixed UTF-8 */
	std::expected<std::u8string_view, strdec_error_t> bytearray_t::string_utf8_lp16(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf8_lp24 - 24bit length prefixed UTF-8 */
	std::expected<std::u8string_view, strdec_error_t> bytearray_t::string_utf8_lp24(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf8_lp32 - 32bit length prefixed UTF-8 */
	std::expected<std::u8string_view, strdec_error_t> bytearray_t::string_utf8_lp32(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf8_lp64 - 64bit length prefixed UTF-8 */
	std::expected<std::u8string_view, strdec_error_t> bytearray_t::string_utf8_lp64(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf8_leb128 - LEB128 length prefixed UTF-8 */
	std::expected<std::u8string_view, strdec_error_t> bytearray_t::string_utf8_leb128(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}


	/* UTF-16 w/ Byte-Order-Mark */

	/* string_utf16_bom - Fixed length UTF-16 w/ Byte-Order-Mark */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16_bom(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16_bom_zt - Zero/Null terminated UTF-16 w/ Byte-Order-Mark */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16_bom_zp(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16_zp - Fixed length null-padded UTF-16 w/ Byte-Order-Mark */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16_bom_zt(const std::size_t idx, const std::size_t max_len) {
		(void)idx;
		(void)max_len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16_bom_lp8 - 8bit length prefixed UTF-16 w/ Byte-Order-Mark */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16_bom_lp8(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16_bom_lp16 - 16bit length prefixed UTF-16 w/ Byte-Order-Mark */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16_bom_lp16(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16_bom_lp24 - 24bit length prefixed UTF-16 w/ Byte-Order-Mark */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16_bom_lp24(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16_bom_lp32 - 32bit length prefixed UTF-16 w/ Byte-Order-Mark */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16_bom_lp32(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16_bom_lp64 - 64bit length prefixed UTF-16 w/ Byte-Order-Mark */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16_bom_lp64(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16_bom_leb128 - LEB128 length prefixed UTF-16 w/ Byte-Order-Mark */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16_bom_leb128(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}


	/* UTF-16 Little Endian */

	/* string_utf16le - Fixed length terminated UTF-16 Little-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16le(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16le_zt - Zero/Null terminated UTF-16 Little-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16le_zp(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16le_zp - Fixed length null-padded UTF-16 Little-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16le_zt(const std::size_t idx, const std::size_t max_len) {
		(void)idx;
		(void)max_len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16le_lp8 - 8bit length prefixed UTF-16 Little-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16le_lp8(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16le_lp16 - 16bit length prefixed UTF-16 Little-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16le_lp16(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16le_lp24 - 24bit length prefixed UTF-16 Little-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16le_lp24(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16le_lp32 - 32bit length prefixed UTF-16 Little-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16le_lp32(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16le_lp64 - 64bit length prefixed UTF-16 Little-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16le_lp64(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16le_leb128 - LEB128 length prefixed UTF-16 Little-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16le_leb128(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}


	/* UTF-16 Big Endian */

	/* string_utf16be - Fixed length terminated UTF-16 Big-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16be(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16be_zt - Zero/Null terminated UTF-16 Big-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16be_zp(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16be_zp - Fixed length null-padded UTF-16 Big-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16be_zt(const std::size_t idx, const std::size_t max_len) {
		(void)idx;
		(void)max_len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16be_lp8 - 8bit length prefixed UTF-16 Big-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16be_lp8(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16be_lp16 - 16bit length prefixed UTF-16 Big-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16be_lp16(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16be_lp24 - 24bit length prefixed UTF-16 Big-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16be_lp24(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16be_lp32 - 32bit length prefixed UTF-16 Big-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16be_lp32(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16be_lp64 - 64bit length prefixed UTF-16 Big-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16be_lp64(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf16be_leb128 - LEB128 length prefixed UTF-16 Big-Endian */
	std::expected<std::u16string_view, strdec_error_t> bytearray_t::string_utf16be_leb128(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}


	/* UTF-32 w/ Byte-Order-Mark */

	/* string_utf32_bom - Fixed length UTF-32 w/ Byte-Order-Mark */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32_bom(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32_bom_zt - Zero/Null terminated UTF-32 w/ Byte-Order-Mark */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32_bom_zp(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32_bom_zp - Fixed length null-padded UTF-32 w/ Byte-Order-Mark */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32_bom_zt(const std::size_t idx, const std::size_t max_len) {
		(void)idx;
		(void)max_len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32_bom_lp8 - 8bit length prefixed UTF-32 w/ Byte-Order-Mark */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32_bom_lp8(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32_bom_lp16 - 16bit length prefixed UTF-32 w/ Byte-Order-Mark */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32_bom_lp16(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32_bom_lp24 - 24bit length prefixed UTF-32 w/ Byte-Order-Mark */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32_bom_lp24(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32_bom_lp32 - 32bit length prefixed UTF-32 w/ Byte-Order-Mark */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32_bom_lp32(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32_bom_lp64 - 64bit length prefixed UTF-32 w/ Byte-Order-Mark */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32_bom_lp64(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32_bom_leb128 - LEB128 length prefixed UTF-32 w/ Byte-Order-Mark */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32_bom_leb128(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}


	/* UTF-32 Little Endian */

	/* string_utf32le - Fixed length terminated UTF-32 Little-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32le(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32le_zt - Zero/Null terminated UTF-32 Little-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32le_zp(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32le_zp - Fixed length null-padded UTF-32 Little-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32le_zt(const std::size_t idx, const std::size_t max_len) {
		(void)idx;
		(void)max_len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32le_lp8 - 8bit length prefixed UTF-32 Little-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32le_lp8(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32le_lp16 - 16bit length prefixed UTF-32 Little-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32le_lp16(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32le_lp24 - 24bit length prefixed UTF-32 Little-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32le_lp24(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32le_lp32 - 32bit length prefixed UTF-32 Little-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32le_lp32(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32le_lp64 - 64bit length prefixed UTF-32 Little-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32le_lp64(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32le_leb128 - LEB128 length prefixed UTF-32 Little-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32le_leb128(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}


	/* UTF-32 Big Endian */

	/* string_utf32be - Fixed length terminated UTF-32 Big-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32be(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32be_zt - Zero/Null terminated UTF-32 Big-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32be_zp(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32le_zp - Fixed length null-padded UTF-32 Big-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32be_zt(const std::size_t idx, const std::size_t max_len) {
		(void)idx;
		(void)max_len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32be_lp8 - 8bit length prefixed UTF-32 Big-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32be_lp8(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32be_lp16 - 16bit length prefixed UTF-32 Big-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32be_lp16(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32be_lp24 - 24bit length prefixed UTF-32 Big-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32be_lp24(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32be_lp32 - 32bit length prefixed UTF-32 Big-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32be_lp32(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32be_lp64 - 64bit length prefixed UTF-32 Big-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32be_lp64(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_utf32be_leb128 - LEB128 length prefixed UTF-32 Big-Endian */
	std::expected<std::u32string_view, strdec_error_t> bytearray_t::string_utf32be_leb128(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}


	/* SHIFT-JIS */

	/* string_shiftjis - Fixed length terminated SHIFT-JIS */
	std::expected<std::wstring_view, strdec_error_t> bytearray_t::string_shiftjis(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_shiftjis_zt - Zero/Null terminated SHIFT-JIS */
	std::expected<std::wstring_view, strdec_error_t> bytearray_t::string_shiftjis_zt(const std::size_t idx, const std::size_t max_len) {
		(void)idx;
		(void)max_len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}
	/* string_shiftjis_zp - Fixed length null-padded terminated SHIFT-JIS */
	std::expected<std::wstring_view, strdec_error_t> bytearray_t::string_shiftjis_zp(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* string_shiftjis_lp8 - 8bit length prefixed SHIFT-JIS */
	std::expected<std::wstring_view, strdec_error_t> bytearray_t::string_shiftjis_lp8(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}
	/* string_shiftjis_lp16 - 16bit length prefixed SHIFT-JIS */
	std::expected<std::wstring_view, strdec_error_t> bytearray_t::string_shiftjis_lp16(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}
	/* string_shiftjis_lp24 - 24bit length prefixed SHIFT-JIS */
	std::expected<std::wstring_view, strdec_error_t> bytearray_t::string_shiftjis_lp24(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}
	/* string_shiftjis_lp32 - 32bit length prefixed SHIFT-JIS */
	std::expected<std::wstring_view, strdec_error_t> bytearray_t::string_shiftjis_lp32(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}
	/* string_shiftjis_lp64 - 64bit length prefixed SHIFT-JIS */
	std::expected<std::wstring_view, strdec_error_t> bytearray_t::string_shiftjis_lp64(const std::size_t idx, const std::endian endian) {
		(void)idx;
		(void)endian;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}
	/* string_shiftjis_leb128 - LEB128 length prefixed SHIFT-JIS */
	std::expected<std::wstring_view, strdec_error_t> bytearray_t::string_shiftjis_leb128(const std::size_t idx) {
		(void)idx;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	#if defined(PANKO_WITH_BROTLI)
	/* decompress_brotli - Decompress bytearray slice with brotli */
	std::expected<bytearray_t, decomp_error_t> bytearray_t::decompress_brotli(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}
	#endif /* PANKO_WITH_BROTLI */

	/* decompress_bz2 - Decompress bytearray slice with bz2 */
	std::expected<bytearray_t, decomp_error_t> bytearray_t::decompress_bz2(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* decompress_deflate - Decompress bytearray slice with zlib deflate */
	std::expected<bytearray_t, decomp_error_t> bytearray_t::decompress_deflate(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* decompress_lz4 - Decompress bytearray slice with lz4 */
	std::expected<bytearray_t, decomp_error_t> bytearray_t::decompress_lz4(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* decompress_lz77 - Decompress bytearray slice with lz77 */
	std::expected<bytearray_t, decomp_error_t> bytearray_t::decompress_lz77(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* decompress_lz77huff - Decompress bytearray slice with lz77+huffman */
	std::expected<bytearray_t, decomp_error_t> bytearray_t::decompress_lz77huff(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* decompress_lzma - Decompress bytearray slice with lzma */
	std::expected<bytearray_t, decomp_error_t> bytearray_t::decompress_lzma(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* decompress_lznt1 - Decompress bytearray slice with lznt1 */
	std::expected<bytearray_t, decomp_error_t> bytearray_t::decompress_lznt1(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	#if defined(PANKO_WITH_SNAPPY)
	/* decompress_snappy - Decompress bytearray slice with snappy */
	std::expected<bytearray_t, decomp_error_t> bytearray_t::decompress_snappy(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}
	#endif /* PANKO_WITH_SNAPPY */

	/* decompress_xz - Decompress bytearray slice with xz */
	std::expected<bytearray_t, decomp_error_t> bytearray_t::decompress_xz(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

	/* decompress_zstd - Decompress bytearray slice with zstd */
	std::expected<bytearray_t, decomp_error_t> bytearray_t::decompress_zstd(const std::size_t idx, const std::size_t len) {
		(void)idx;
		(void)len;
		/* TODO(aki): Implement */
		spdlog::get("panko")->warn("bytearray_t::{} is unimplemented!", __FUNCTION__);
		return {};
	}

}
