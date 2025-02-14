// SPDX-License-Identifier: BSD-3-Clause
/* sys.cc - System helpers */

#include <cstdlib>
#include <string>
#include <string_view>
#include <optional>

#include "panko/support/sys.hh"

using namespace std::literals::string_view_literals;

namespace Panko::support::sys {

	[[nodiscard]]
	std::optional<std::string> getenv(const std::string_view& name) {
		/* Not ideal, but needed to ensure we don't have getenv run off into the sunset */
		if (name.back() != '\0') {
			return std::nullopt;
		}
		// NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage, concurrency-mt-unsafe)
		const auto* const env{std::getenv(name.data())};
		if (env != nullptr) {
			return std::make_optional(std::string(env));
		}
		return std::nullopt;
	}

	[[nodiscard]]
	fs::path get_home() noexcept {
		fs::path tmp{};
		#if defined(_WIN32)
			constexpr auto HOMEDIR_VAR{"USERPROFILE"sv};
			constexpr auto USERNAME_VAR{"USERNAME"sv};
			constexpr auto HOMEPATH{"C:\\Users\\"sv};
		#elif defined(__APPLE__)
			constexpr auto HOMEDIR_VAR{"HOME"sv};
			constexpr auto USERNAME_VAR{"USER"sv};
			constexpr auto HOMEPATH{"/Users/"sv};
		#else
			constexpr auto HOMEDIR_VAR{"HOME"sv};
			constexpr auto USERNAME_VAR{"USER"sv};
			constexpr auto HOMEPATH{"/home/"sv};
		#endif

		if (auto homedir = Panko::support::sys::getenv(HOMEDIR_VAR)) {
			tmp.concat(*homedir);
		} else if (auto username = Panko::support::sys::getenv(USERNAME_VAR)) {
			tmp.concat(HOMEPATH);
			tmp.concat(*username);
		} else {
			tmp.concat("~"sv);
		}

		return tmp;
	}

	[[nodiscard]]
	fs::path expand_user(const std::string_view& path) noexcept {
		if (path.size() < 2) {
			return { path };
		}

		if (!path.empty() && path[0] == '~' && path[1] == '/') {
			fs::path tmp{get_home()};
			tmp.concat(path.substr(1, path.length()));
			return tmp;
		}
		return { path };
	}
}
