// SPDX-License-Identifier: BSD-3-Clause
/* raw_file.hh - Raw file I/O wrapper */
#pragma once
#if !defined(PANKO_SUPPORT_IO_RAW_FILE_HH)
#define PANKO_SUPPORT_IO_RAW_FILE_HH

#include <cstdint>
#include <cstddef>
#include <filesystem>
#include <string_view>
#include <string>
#include <optional>

#include <sys/stat.h>
#include <fcntl.h>

#include "panko/internal/defs.hh"

#if !defined(_WIN32)
#	include <unistd.h>
#else
#	define O_NOCTTY _O_BINARY
#	if defined(__MINGW32__) || defined(__MINGW64__)
#		include <unistd.h>
#		include <share.h>
#		undef fstat
#	elif defined(_MSC_VER)
#		include <io.h>
#	endif
#endif

#include "panko/core/types.hh"
#include "panko/core/mmap.hh"
#include "panko/support/io/io.hh"

namespace Panko::support::io {
	using Panko::core::types::ssize_t;
	using Panko::core::types::mode_t;
	using Panko::core::types::off_t;
	using Panko::core::mmap_t;

	// NOTE(aki): Eventually we might want windows support, so this should let us abstract away most of the icky windows differences.
	namespace _compat {
		#if !defined(_WIN32)
			using stat_t = struct ::stat;

			[[nodiscard]]
			inline std::int32_t fdopen(const char* const filename, const std::int32_t flags, const mode_t mode) noexcept {
				// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
				return ::open(filename, flags, mode);
			}

			[[nodiscard]]
			inline std::int32_t fdclose(const std::int32_t fd) noexcept {
				return ::close(fd);
			}

			[[nodiscard]]
			inline ssize_t fdread(const std::int32_t fd, void* const buffer, const std::size_t len) noexcept {
				return ::read(fd, buffer, len);
			}

			[[nodiscard]]
			inline ssize_t fdwrite(const std::int32_t fd, const void* const buffer, const std::size_t len) noexcept {
				return ::write(fd, buffer, len);
			}

			[[nodiscard]]
			inline off_t fdseek(const std::int32_t fd, const off_t offset, const std::int32_t whence) noexcept {
				return ::lseek(fd, offset, whence);
			}

			[[nodiscard]]
			inline off_t fdtell(const std::int32_t fd) noexcept {
				return ::lseek(fd, 0, SEEK_CUR);
			}

			[[nodiscard]]
			inline std::int32_t fdtruncate(const std::int32_t fd, const off_t size) noexcept {
				return ::ftruncate(fd, size);
			}

			[[nodiscard]]
			inline std::int32_t fdup(const std::int32_t fd) noexcept {
				return ::dup(fd);
			}

			[[nodiscard]]
			inline std::int32_t fdstat(std::int32_t fd, stat_t* stat) noexcept {
				return ::fstat(fd, stat);
			}
		#else /* !_WIN32 */
			using stat_t = struct ::_stat64;

			[[nodiscard]]
			inline std::int32_t fdopen(const char* const filename, const std::int32_t flags, const mode_t mode) noexcept {
				std::int32_t fd{};
				_sopen_s(&fd, filename, flags, _SH_DENYNO, mode);
				return fd;
			}

			[[nodiscard]]
			inline std::int32_t fdopen(const wchar_t* const filename, const std::int32_t flags, const mode_t mode) noexcept {
				std::int32_t fd{};
				::_wsopen_s(&fd, filename, flags, _SH_DENYNO, mode);
				return fd;
			}

			[[nodiscard]]
			inline std::int32_t fdclose(const std::int32_t fd) noexcept {
				return ::_close(fd);
			}

			[[nodiscard]]
			inline ssize_t fdread(const std::int32_t fd, void* const buffer, const std::size_t len) noexcept {
				return ::_read(fd, buffer, static_cast<std::uint32_t>(std::min<std::size_t>(len, INT_MAX)));
			}

			[[nodiscard]]
			inline ssize_t fdwrite(const std::int32_t fd, const void* const buffer, const std::size_t len) noexcept {
				return ::_write(fd, buffer, static_cast<std::uint32_t>(std::min<std::size_t>(len, INT_MAX)));
			}

			[[nodiscard]]
			inline off_t fdseek(const std::int32_t fd, const off_t offset, const std::int32_t whence) noexcept {
				return ::_lseeki64(fd, offset, whence);
			}

			[[nodiscard]]
			inline off_t fdtell(const std::int32_t fd) noexcept {
				return ::_telli64(fd);
			}

			[[nodiscard]]
			inline std::int32_t fdtruncate(const std::int32_t fd, const off_t size) noexcept {
				return ::_chsize_s(fd, size);
			}

			[[nodiscard]]
			inline std::int32_t fdup(const std::int32_t fd) noexcept {
				return ::_dup(fd);
			}

			[[nodiscard]]
			inline std::int32_t fdstat(std::int32_t fd, stat_t* stat) noexcept {
				return ::_fstat64(fd, stat);
			}
		#endif
	}

	namespace fs = std::filesystem;

	struct raw_file_t final : public io_t {
		private:
			std::int32_t  _fd{-1};
			mutable bool  _eof{false};
			mutable off_t _len{-1};

			void invalidate() noexcept {
				_fd = -1;
			}
		public:
			constexpr raw_file_t() noexcept = default;
			constexpr raw_file_t(std::int32_t fd) noexcept : _fd{fd} { }
			raw_file_t(const char* const fname, const std::int32_t flags, const ::mode_t mode = 0) noexcept :
				_fd{_compat::fdopen(fname, flags, mode)} {}
			raw_file_t(const std::string& fname, const std::int32_t flags, const ::mode_t mode = 0) noexcept :
				_fd{_compat::fdopen(fname.c_str(), flags, mode)} {}
			raw_file_t(const std::string_view& fname, const std::int32_t flags, const ::mode_t mode = 0) noexcept :
				_fd{_compat::fdopen(fname.data(), flags, mode)} {}
			raw_file_t(const fs::path& fname, const std::int32_t flags, const ::mode_t mode = 0) noexcept :
				_fd{_compat::fdopen(fname.c_str(), flags, mode)} {}

			raw_file_t(const raw_file_t&) = delete;

			raw_file_t(raw_file_t&& other) noexcept : raw_file_t{} {
				*this = std::move(other);
			}

			~raw_file_t() noexcept override {
				if (_fd != -1) {
					static_cast<void>(_compat::fdclose(_fd));
				}
			}


			raw_file_t& operator=(raw_file_t&& dest) noexcept {
				std::swap(_fd, dest._fd);
				std::swap(_eof, dest._eof);
				std::swap(_len, dest._len);
				return *this;
			}
			raw_file_t& operator=(const raw_file_t&) = delete;

			[[nodiscard]]
			bool operator==(const std::int32_t fd) const noexcept { return _fd == fd; }

			[[nodiscard]]
			bool valid() const noexcept override {
				return _fd != -1;
			}

			[[nodiscard]]
			bool eof() const noexcept override {
				return _eof;
			}

			[[nodiscard]]
			operator std::int32_t() const noexcept override {
				return _fd;
			}

			[[nodiscard]]
			off_t seek(const off_t offset, const std::int32_t whence) const noexcept override {
				const auto res{_compat::fdseek(_fd, offset, whence)};
				_eof = res == length();
				return res;
			}

			[[nodiscard]]
			off_t tell() const noexcept override {
				return _compat::fdtell(_fd);
			}

			[[nodiscard]]
			off_t length() const noexcept override {
				if (_len != -1) {
					return _len;
				}
				struct stat file_stat{};
				const auto res{_compat::fdstat(_fd, &file_stat)};
				_len = res ? - 1 : file_stat.st_size;
				return _len;
			}

			[[nodiscard]]
			ssize_t write(const void* const buffer, const std::size_t len, std::nullptr_t) const noexcept override {
				return _compat::fdwrite(_fd, buffer, len);
			}

			[[nodiscard]]
			ssize_t read(void* const buffer, const std::size_t len, std::nullptr_t) const noexcept override {
				const auto res{_compat::fdread(_fd, buffer, len)};
				if (!res && len) {
					_eof = true;
				}
				return res;
			}

			[[nodiscard]]
			std::optional<struct stat> stat() const noexcept {
				struct stat file_stat{};
				if (!_compat::fdstat(_fd, &file_stat)) {
					return file_stat;
				}
				return std::nullopt;
			}

			[[nodiscard]]
			raw_file_t dup() const noexcept {
				return _compat::fdup(_fd);
			}

			[[nodiscard]]
			mmap_t map(const std::int32_t prot, const std::int32_t flags = MAP_SHARED) noexcept {
				const auto len{length()};
				if (len <= 0) {
					return {};
				}
				return map(prot, static_cast<std::size_t>(len), flags);
			}

			[[nodiscard]]
			mmap_t map(
				const std::int32_t prot, const std::size_t len, const std::int32_t flags, void* map_addr = nullptr
			) noexcept {
				if (!valid()) {
					return {};
				}
				const std::int32_t file{_fd};
				invalidate();

				return {file, len, prot, flags, map_addr};
			}

			using io_t::read;
			using io_t::write;
			using io_t::seek;
	};
}

#endif /* PANKO_SUPPORT_IO_RAW_FILE_HH */
