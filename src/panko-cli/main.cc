// SPDX-License-Identifier: BSD-3-Clause

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iostream> // Ick :(
#include <print>
#include <string_view>
#include <thread>
#include <unistd.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <cxxopts.hpp>

#include "panko/internal/defs.hh"
#include "panko/support/sys.hh"
#include "panko/support/paths.hh"
#include "panko/config.hh"

namespace fs  = std::filesystem;
namespace cfg = Panko::config;

using namespace std::literals::string_view_literals;

const auto panko_version{std::format(
	"{} ({} {} {}-{})"sv,
	cfg::version_full, cfg::compiler_name, cfg::compiler_version, cfg::target_system, cfg::target_arch
)};


void print_banner() noexcept;
void print_help() noexcept;
void print_version() noexcept;
cxxopts::Options setup_options() noexcept;

auto options{setup_options()};

int main(int argc, char** argv) noexcept {
	/* Ingest CLI Options */
	const auto args{options.parse(argc, argv)};

	if (args.count("help")) {
		print_help();
		return 0;
	} else if (args.count("version")) {
		print_banner();
		return 0;
	}

	const auto verbosity{args.count("verbose")};

	/* Set up the logger */
	auto logger{spdlog::stdout_color_mt("panko")};

	if (verbosity == 1) {
		logger->set_level(spdlog::level::debug);
	} else if (verbosity >= 2) {
		logger->set_level(spdlog::level::trace);
	}

	spdlog::get("panko")->debug("Creating configuration and data directories if needed"sv);
	Panko::support::paths::initialize_dirs();

	print_banner();

	return 0;
}


void print_banner() noexcept {
	print_version();
	std::println("\nPanko and the Panko CLI are licensed under the BSD-3-Clause <https://spdx.org/licenses/BSD-3-Clause.htm>\n");
}

void print_help() noexcept {
	print_banner();
	std::cout << options.positional_help("CAPTURE_FILE").help() << "\n";
}

void print_version() noexcept {
	std::println("Panko v{}", panko_version);
	std::println("  Python Version: {}", cfg::python_version);
	std::println("  Lua Version: {} {}", cfg::lua_backend, cfg::lua_version);
}

cxxopts::Options setup_options() noexcept {
	cxxopts::Options opts{"panko-cli", "Panko packet dissector and analysis engine CLI"};

	opts.add_options()
		("capture", "The capture file to open", cxxopts::value<fs::path>());

	opts.add_options("Common")
		("h,help",    "Display this message and exit")
		("v,verbose", "Enable verbose logging (more v's the more verbose)")
		("V,version", "Show Panko version");

	options.parse_positional({"capture"});

	return opts;
}
