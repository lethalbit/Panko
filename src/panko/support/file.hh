// SPDX-License-Identifier: BSD-3-Clause
/* file.hh - File Interface */
#pragma once
#if !defined(PANKO_SUPPORT_FILE_HH)
#define PANKO_SUPPORT_FILE_HH

#include <filesystem>
#include <variant>
#include <optional>
#include <expected>

#include "panko/internal/defs.hh"
#include "panko/core/errcodes.hh"

#include "panko/support/io/io.hh"
#include "panko/support/io/raw_file.hh"
#include "panko/support/io/compressed/bz2_file.hh"
#include "panko/support/io/compressed/gzip_file.hh"
#include "panko/support/io/compressed/lz4_file.hh"
#include "panko/support/io/compressed/lzma_file.hh"
#include "panko/support/io/compressed/xz_file.hh"
#include "panko/support/io/compressed/zstd_file.hh"

namespace Panko::support {
	namespace fs = std::filesystem;

	using Panko::core::error_codes::file_error_t;

	using file_t = std::variant<
		Panko::support::io::raw_file_t,
		Panko::support::io::compressed::bz2_file_t,
		Panko::support::io::compressed::gzip_file_t,
		Panko::support::io::compressed::lz4_file_t,
		Panko::support::io::compressed::lzma_file_t,
		Panko::support::io::compressed::xz_file_t,
		Panko::support::io::compressed::zstd_file_t
	>;

	[[nodiscard]]
	std::optional<file_t> open(const fs::path& filename) noexcept;

	[[nodiscard]]
	bool atomic_move(const fs::path& src, const fs::path& dest, bool clobber = false) noexcept;
}

#endif /* PANKO_SUPPORT_FILE_HH */
