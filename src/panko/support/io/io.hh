// SPDX-License-Identifier: BSD-3-Clause
/* io.hh - File I/O traits */
#pragma once
#if !defined(PANKO_SUPPORT_IO_IO_HH)
#define PANKO_SUPPORT_IO_IO_HH

#include <cstdint>
#include <cstddef>
#include <memory>
#include <array>
#include <type_traits>
#include <string>
#include <string_view>

#include "panko/internal/defs.hh"
#include "panko/core/types.hh"

namespace Panko::support::io {
	using Panko::core::types::ssize_t;

	struct io_t {
		virtual ~io_t() noexcept = default;

		[[nodiscard]]
		virtual bool valid() const noexcept = 0;
		[[nodiscard]]
		virtual bool eof() const noexcept = 0;
		[[nodiscard]]
		virtual operator std::int32_t() const noexcept = 0;
		[[nodiscard]]
		virtual off_t seek(const off_t offset, const std::int32_t whence) const noexcept = 0;
		[[nodiscard]]
		bool seek_rel(const off_t offset) const noexcept {
			const auto curr_pos{tell()};
			if (curr_pos == -1 || curr_pos + offset < 0) {
				return false;
			}
			return seek(offset, SEEK_CUR) == curr_pos + offset;
		}
		[[nodiscard]]
		virtual off_t tell() const noexcept = 0;
		[[nodiscard]]
		virtual off_t length() const noexcept = 0;
		[[nodiscard]]
		bool head() const noexcept {
			return seek(0, SEEK_SET) == 0;
		}
		[[nodiscard]]
		bool tail() const noexcept {
			const auto offset{length()};
			if (offset < 0) {
				return false;
			}
			return seek(offset, SEEK_SET) == offset;
		}

		[[nodiscard]]
		virtual ssize_t write(const void* const buffer, const std::size_t len, std::nullptr_t) const noexcept = 0;
		[[nodiscard]]
		virtual ssize_t read(void* const buffer, const std::size_t len, std::nullptr_t) const noexcept = 0;

		[[nodiscard]]
		bool read(void* const value, const std::size_t len, std::size_t& res_len) const noexcept {
			const ssize_t res{read(value, len, nullptr)};
			if (res < 0) {
				return false;
			}
			res_len = std::size_t(res);
			return res_len == len;
		}

		[[nodiscard]]
		bool read(void* const value, const std::size_t len) const noexcept {
			std::size_t res_len{};
			return read(value, len, res_len);
		}

		template<typename T>
		[[nodiscard]]
		bool read(T& value) const noexcept {
			return read(&value, sizeof(T));
		}

		template<typename T>
		[[nodiscard]]
		bool read(std::unique_ptr<T>& value) const noexcept {
			return read(value.get(), sizeof(T));
		}

		template<typename T>
		[[nodiscard]]
		bool read(const std::unique_ptr<T>& value) const noexcept {
			return read(value.get(), sizeof(T));
		}

		template<typename T>
		[[nodiscard]]
		bool read(const std::unique_ptr<T[]>& value, const std::size_t len) const noexcept {
			return read(value.get(), sizeof(T) * len);
		}

		template<typename T, std::size_t N>
		[[nodiscard]]
		bool read(std::array<T, N>& value) const noexcept {
			return read(value.data(), sizeof(T) * N);
		}

		template<std::size_t len, typename T, std::size_t N>
		[[nodiscard]]
		bool read(std::array<T, N>& value) const noexcept {
			static_assert(len <= N, "Unable to read more than array length!");
			return read(value.data(), sizeof(T) * len);
		}

		[[nodiscard]]
		bool read_le(std::uint16_t& value) const noexcept {
			std::array<std::uint8_t, 2> data{};
			const bool res{read(data)};
			value = std::uint16_t((static_cast<std::uint16_t>(data[1]) << 8U) | data[0]);

			return res;
		}

		[[nodiscard]]
		bool read_le(std::uint32_t& value) const noexcept {
			std::array<std::uint8_t, 4> data{};
			const bool res{read(data)};
			value = (
				(static_cast<std::uint32_t>(data[3]) << 24U) |
				(static_cast<std::uint32_t>(data[2]) << 16U) |
				(static_cast<std::uint32_t>(data[1]) <<  8U) |
				data[0]
			);

			return res;
		}

		[[nodiscard]]
		bool read_le(std::uint64_t& value) const noexcept {
			std::array<std::uint8_t, 8> data{};
			const bool res{read(data)};
			value = (
				(static_cast<std::uint64_t>(data[7]) << 56U) |
				(static_cast<std::uint64_t>(data[6]) << 48U) |
				(static_cast<std::uint64_t>(data[5]) << 40U) |
				(static_cast<std::uint64_t>(data[4]) << 32U) |
				(static_cast<std::uint64_t>(data[3]) << 24U) |
				(static_cast<std::uint64_t>(data[2]) << 16U) |
				(static_cast<std::uint64_t>(data[1]) <<  8U) |
				data[0]
			);
			return res;
		}

		template<typename T, typename = typename std::enable_if_t<
			std::is_integral_v<T> && !std::is_same_v<T, bool> &&
			std::is_signed_v<T> && sizeof(T) >= 2
		>>
		[[nodiscard]]
		bool read_le(T& value) const noexcept {
			std::make_unsigned_t<T> data{};
			const auto res{read_le(data)};
			value = static_cast<T>(data);
			return res;
		}

		[[nodiscard]]
		bool read_be(std::uint16_t& value) const noexcept {
			std::array<std::uint8_t, 2> data{};
			const bool res{read(data)};
			value = std::uint16_t((static_cast<std::uint16_t>(data[0]) << 8U) | data[1]);

			return res;
		}

		[[nodiscard]]
		bool read_be(std::uint32_t& value) const noexcept {
			std::array<std::uint8_t, 4> data{};
			const bool res{read(data)};
			value = (
				(static_cast<std::uint32_t>(data[0]) << 24U) |
				(static_cast<std::uint32_t>(data[1]) << 16U) |
				(static_cast<std::uint32_t>(data[2]) <<  8U) |
				data[3]
			);

			return res;
		}

		[[nodiscard]]
		bool read_be(std::uint64_t& value) const noexcept {
			std::array<std::uint8_t, 8> data{};
			const bool res{read(data)};
			value = (
				(static_cast<std::uint64_t>(data[0]) << 56U) |
				(static_cast<std::uint64_t>(data[1]) << 48U) |
				(static_cast<std::uint64_t>(data[2]) << 40U) |
				(static_cast<std::uint64_t>(data[3]) << 32U) |
				(static_cast<std::uint64_t>(data[4]) << 24U) |
				(static_cast<std::uint64_t>(data[5]) << 16U) |
				(static_cast<std::uint64_t>(data[6]) <<  8U) |
				data[7]
			);
			return res;
		}

		template<typename T, typename = typename std::enable_if_t<
			std::is_integral_v<T> && !std::is_same_v<T, bool> &&
			std::is_signed_v<T> && sizeof(T) >= 2
		>>
		[[nodiscard]]
		bool read_be(T& value) const noexcept {
			std::make_unsigned_t<T> data{};
			const auto res{read_be(data)};
			value = static_cast<T>(data);
			return res;
		}

		[[nodiscard]]
		bool write(const void* const value, const std::size_t len) const noexcept {
			const auto res{write(value, len, nullptr)};
			if (res < 0) {
				return false;
			}
			return std::size_t(res) == len;
		}

		template<typename T>
		[[nodiscard]]
		bool write(const T& value) const noexcept {
			return write(&value, sizeof(T));
		}

		template<typename T>
		[[nodiscard]]
		bool write(const std::unique_ptr<T>& value) const noexcept {
			return write(value.get(), sizeof(T));
		}

		template<typename T>
		[[nodiscard]]
		bool write(const std::unique_ptr<T[]>& value, const std::size_t len) const noexcept {
			return write(value.get(), sizeof(T) * len);
		}

		template<typename T, std::size_t N>
		[[nodiscard]]
		bool write(const std::array<T, N>& value) const noexcept {
			return write(value.data(), sizeof(T) * N);
		}

		template<std::size_t len, typename T, std::size_t N>
		[[nodiscard]]
		bool write(const std::array<T, N>& value) const noexcept {
			static_assert(len <= N, "Unable to write more than the array length!");
			return write(value.data(), sizeof(T) * len);
		}

		[[nodiscard]]
		bool write(const std::string& value) const noexcept {
			return write(value.data(), value.size());
		}

		[[nodiscard]]
		bool write(const std::string_view& value) const noexcept {
			return write(value.data(), value.size());
		}

		[[nodiscard]]
		bool write_le(const std::uint16_t value) const noexcept {
			const std::array<std::uint8_t, 2> data{{
				static_cast<std::uint8_t>(value),
				static_cast<std::uint8_t>(value >> 8U)
			}};
			return write(data);
		}

		[[nodiscard]]
		bool write_le(const std::uint32_t value) const noexcept {
			const std::array<std::uint8_t, 4> data{{
				static_cast<std::uint8_t>(value),
				static_cast<std::uint8_t>(value >>  8U),
				static_cast<std::uint8_t>(value >> 16U),
				static_cast<std::uint8_t>(value >> 24U),
			}};
			return write(data);
		}

		[[nodiscard]]
		bool write_le(const std::uint64_t value) const noexcept {
			const std::array<std::uint8_t, 8> data{{
				static_cast<std::uint8_t>(value),
				static_cast<std::uint8_t>(value >>  8U),
				static_cast<std::uint8_t>(value >> 16U),
				static_cast<std::uint8_t>(value >> 24U),
				static_cast<std::uint8_t>(value >> 32U),
				static_cast<std::uint8_t>(value >> 40U),
				static_cast<std::uint8_t>(value >> 48U),
				static_cast<std::uint8_t>(value >> 56U),
			}};
			return write(data);
		}

		template<typename T, typename = typename std::enable_if_t<
			std::is_integral_v<T> && !std::is_same_v<T, bool> &&
			std::is_signed_v<T> && sizeof(T) >= 2
		>>
		[[nodiscard]]
		bool write_le(const T value) const noexcept {
			auto data{static_cast<std::make_unsigned_t<T>>(value)};
			return write_le(data);
		}

		[[nodiscard]]
		bool write_be(const std::uint16_t value) const noexcept {
			const std::array<std::uint8_t, 2> data{{
				static_cast<std::uint8_t>(value >> 8U),
				static_cast<std::uint8_t>(value)
			}};
			return write(data);
		}

		[[nodiscard]]
		bool write_be(const std::uint32_t value) const noexcept {
			const std::array<std::uint8_t, 4> data{{
				static_cast<std::uint8_t>(value >> 24U),
				static_cast<std::uint8_t>(value >> 16U),
				static_cast<std::uint8_t>(value >>  8U),
				static_cast<std::uint8_t>(value)
			}};
			return write(data);
		}

		[[nodiscard]]
		bool write_be(const std::uint64_t value) const noexcept {
			const std::array<std::uint8_t, 8> data{{
				static_cast<std::uint8_t>(value >> 56U),
				static_cast<std::uint8_t>(value >> 48U),
				static_cast<std::uint8_t>(value >> 40U),
				static_cast<std::uint8_t>(value >> 32U),
				static_cast<std::uint8_t>(value >> 24U),
				static_cast<std::uint8_t>(value >> 16U),
				static_cast<std::uint8_t>(value >>  8U),
				static_cast<std::uint8_t>(value)
			}};
			return write(data);
		}

		template<typename T, typename = typename std::enable_if_t<
			std::is_integral_v<T> && !std::is_same_v<T, bool> &&
			std::is_signed_v<T> && sizeof(T) >= 2
		>>
		[[nodiscard]]
		bool write_be(const T value) const noexcept {
			auto data{static_cast<std::make_unsigned_t<T>>(value)};
			return write_be(data);
		}
	};
}

#endif /* PANKO_SUPPORT_IO_IO_HH */
