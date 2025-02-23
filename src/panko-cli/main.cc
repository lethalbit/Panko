// SPDX-License-Identifier: BSD-3-Clause
#include <string_view>
#include <cstdint>
#include <filesystem>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <print>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "panko/internal/defs.hh"
#include "panko/support/sys.hh"
#include "panko/support/paths.hh"
#include "panko/config.hh"

namespace fs  = std::filesystem;
namespace cfg = Panko::config;

const auto panko_version{std::format(
	"{} ({} {} {}-{})"sv,
	cfg::version_full, cfg::compiler_name, cfg::compiler_version, cfg::target_system, cfg::target_arch
)};


void print_banner() noexcept;
void print_help() noexcept;
void print_version() noexcept;

int main(int argc, char** argv) noexcept {
	/* Set up the logger */
	auto logger{spdlog::stdout_color_mt("panko")};

	spdlog::get("panko")->debug("Creating configuration and data directories if needed"sv);
	Panko::support::paths::initialize_dirs();

	print_banner();
}


void print_banner() noexcept {
	print_version();
	std::println("\nPanko and the Panko CLI are licensed under the BSD-3-Clause <https://spdx.org/licenses/BSD-3-Clause.htm>");
}

void print_help() noexcept {
	print_banner();

}

void print_version() noexcept {
	std::println("Panko v{}", panko_version);
	std::println("  Python Version: {}", cfg::python_version);
	std::println("  Lua Version: {} {}", cfg::lua_backend, cfg::lua_version);
}
