// SPDX-License-Identifier: BSD-3-Clause
/* bz2_file.hh - bzip2 RAII Wrapper */
#pragma once
#if !defined(PANKO_SUPPORT_IO_COMPRESSED_BZ2_FILE_HH)
#define PANKO_SUPPORT_IO_COMPRESSED_BZ2_FILE_HH

#include <cstdint>

#include "panko/internal/defs.hh"
#include "panko/support/io/io.hh"
#include "panko/support/io/raw_file.hh"

namespace Panko::support::io::compressed {
	using Panko::support::io::raw_file_t;

	struct bz2_file_t final : public io_t {
		constexpr static std::uint64_t MAGIC_MASK{UINT64_C(0xFFFFFF0000000000)};
		constexpr static std::uint64_t MAGIC_VALUE{UINT64_C(0x425A680000000000)};

		[[nodiscard]]
		constexpr static bool valid_magic(const std::uint64_t& magic) noexcept {
			return (magic & MAGIC_MASK) == MAGIC_VALUE;
		}

	private:
		raw_file_t _backing_file{};
	public:
		constexpr bz2_file_t() noexcept = default;
		bz2_file_t(raw_file_t&& backing) noexcept : _backing_file{std::move(backing)}
		{ }

		bz2_file_t(const bz2_file_t&) = delete;
		bz2_file_t(bz2_file_t&& other) noexcept : bz2_file_t{} {
			*this = std::move(other);
		}

		bz2_file_t& operator=(const bz2_file_t&) = delete;
		bz2_file_t& operator=(bz2_file_t&& dest) noexcept {
			std::swap(_backing_file, dest._backing_file);
			return *this;
		}

		[[nodiscard]]
		bool valid() const noexcept override {
			return _backing_file.valid();
		}

		[[nodiscard]]
		bool eof() const noexcept override {
			return _backing_file.eof();
		}

		[[nodiscard]]
		operator std::int32_t() const noexcept override {
			return _backing_file;
		}

		// TODO(aki): Remove [[maybe_unused]] when implemented
		[[nodiscard]]
		off_t seek([[maybe_unused]] const off_t offset,[[maybe_unused]]  const std::int32_t whence) const noexcept override {
			return {};
		}

		[[nodiscard]]
		off_t tell() const noexcept override {
			return {};
		}

		[[nodiscard]]
		off_t length() const noexcept override {
			return {};
		}

		// TODO(aki): Remove [[maybe_unused]] when implemented
		[[nodiscard]]
		ssize_t write([[maybe_unused]] const void* const buffer,[[maybe_unused]] const std::size_t len, std::nullptr_t) const noexcept override {
			return {};
		}

		// TODO(aki): Remove [[maybe_unused]] when implemented
		[[nodiscard]]
		ssize_t read([[maybe_unused]] void* const buffer,[[maybe_unused]] const std::size_t len, std::nullptr_t) const noexcept override {
			return {};
		}
	};
}

#endif /* PANKO_SUPPORT_IO_COMPRESSED_BZ2_FILE_HH */
