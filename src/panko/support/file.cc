// SPDX-License-Identifier: BSD-3-Clause
/* file.cc - File Interface */

#include <filesystem>
#include <variant>
#include <optional>
#include <expected>

#include "panko/core/errcodes.hh"
#include "panko/core/units.hh"
#include "panko/core/types.hh"

#include "panko/support/io/io.hh"
#include "panko/support/io/raw_file.hh"
#include "panko/support/io/compressed/bz2_file.hh"
#include "panko/support/io/compressed/gzip_file.hh"
#include "panko/support/io/compressed/lz4_file.hh"
#include "panko/support/io/compressed/lzma_file.hh"
#include "panko/support/io/compressed/xz_file.hh"
#include "panko/support/io/compressed/zstd_file.hh"

#include "panko/support/file.hh"

namespace Panko::support {
	using namespace Panko::core::units::IEC;

	using Panko::core::error_codes::file_error_t;
	using Panko::core::types::match_t;

	using Panko::support::io::io_t;
	using Panko::support::io::raw_file_t;
	using Panko::support::io::compressed::bz2_file_t;
	using Panko::support::io::compressed::gzip_file_t;
	using Panko::support::io::compressed::lz4_file_t;
	using Panko::support::io::compressed::lzma_file_t;
	using Panko::support::io::compressed::xz_file_t;
	using Panko::support::io::compressed::zstd_file_t;


	[[nodiscard]]
	const io::io_t& decompose_file_variant(const file_t& file_var) noexcept {
		const auto &file{std::visit(match_t{
			[](const auto &entry) -> const io::io_t & {
				return entry;
			}
		}, file_var)};

		return file;
	}

	[[nodiscard]]
	std::optional<file_t> open(const fs::path& filename) noexcept {
		raw_file_t file{filename, O_RDONLY | O_NOCTTY};

		if (!file.valid()) {
			return std::nullopt;
		}

		/* Read the first handful of bytes for magic identification */
		std::uint64_t buff{};
		const auto res{file.read_be(buff)};

		/* Reset seek */
		if (!file.head()) {
			return std::nullopt;
		}

		/* If we read only 0's then we likely failed to read */
		if (!res) {
			return std::nullopt;
		}

		/* Try to match compressed file magics */
		if (bz2_file_t::valid_magic(buff)) {
			return bz2_file_t{std::move(file)};
		}

		if (gzip_file_t::valid_magic(buff)) {
			return gzip_file_t{std::move(file)};
		}

		if (lz4_file_t::valid_magic(buff)) {
			return lz4_file_t{std::move(file)};
		}

		if (lzma_file_t::valid_magic(buff)) {
			return lzma_file_t{std::move(file)};
		}

		if (xz_file_t::valid_magic(buff)) {
			return xz_file_t{std::move(file)};
		}

		if (zstd_file_t::valid_magic(buff)) {
			return zstd_file_t{std::move(file)};
		}

		/* If none of the compressed file magics match assume raw */
		return file;
	}

	/* NOTE(aki): Currently this is only support on Linux */
	[[nodiscard]]
	bool atomic_move(const fs::path& src, const fs::path& dest, bool clobber) noexcept {
		/* Check to see if the src file exists an if the dest exists but we don't clobber */
		if (!fs::exists(src) || (fs::exists(dest) && !clobber)) {
			return false;
		}

		/* Ensure the paths are absolute, otherwise renameat2 will complain */
		const auto abs_src{fs::absolute(src)};
		const auto abs_dst{fs::absolute(dest)};

		/* Atomically rename the file */
		const auto res{::renameat2(0, abs_src.c_str(), 0, abs_dst.c_str(), RENAME_EXCHANGE)};

		return res == 0;
	}
}
