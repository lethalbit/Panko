// SPDX-License-Identifier: BSD-3-Clause

#include <string_view>
#include <cstdint>
#include <filesystem>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <print>

#include <spdlog/spdlog.h>

#include "panko/internal/defs.hh"
#include "panko/internal/frontend.hh"
#include "panko/support/sys.hh"
#include "panko/core/paths.hh"
#include "panko/config.hh"

PANKO_DIAGNOSTICS_PUSH()
PANKO_DIAGNOSTICS_IGNORE("-Wconversion")
PANKO_DIAGNOSTICS_IGNORE("-Warith-conversion")
PANKO_DIAGNOSTICS_IGNORE("-Wsign-conversion")
#include <QApplication>
#include <QTranslator>

#define NDEBUG
PANKO_DIAGNOSTICS_IGNORE("-Wredundant-decls")
#if defined(KDE_INTEGRATION)
#	if defined(NDEBUG)
#		include <KCrash>
# 	endif
#	include <KAboutData>
#endif
PANKO_DIAGNOSTICS_POP()

#include "panko-gui/windows/main.hh"

using namespace std::literals::string_view_literals;
using namespace std::literals::chrono_literals;

namespace fs  = std::filesystem;
namespace cfg = Panko::config;

using Panko::internal::python_version;
using Panko::internal::lua_version;

const auto panko_version{std::format(
	"{} ({} {} {}-{})"sv,
	cfg::version_full, cfg::compiler_name, cfg::compiler_version, cfg::target_system, cfg::target_arch
)};

void print_banner() noexcept;
void print_help() noexcept;
void print_version() noexcept;

int main(int argc, char** argv) noexcept {

	/* Initialize Qt Application */
	QApplication panko{argc, argv};
	QApplication::setApplicationDisplayName("Panko");
	QApplication::setDesktopFileName("net.lethalbit.Panko.desktop");
	QApplication::setApplicationVersion(QString::fromStdString(panko_version));

	/* Hook up the KDE integration */
	#if defined(KDE_INTEGRATION)
	const KAboutData kabout{
		"Panko", "Panko",
		QString::fromStdString(panko_version),
		"Crispy and Fried packet dissection",
		KAboutLicense::BSD_3_Clause,
		"(c) 2025 Panko Contributors",
		"",
		/* NOTE(aki): This is fine because it's being implicitly converted to a QString */
		// NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage)
		cfg::bugreport_url.data()
	};
	KAboutData::setApplicationData(kabout);
	#	if defined(NDEBUG)
		KCrash::initialize();
		KCrash::setCrashHandler();
		KCrash::setErrorMessage(QString::fromStdString(
			std::format("Please report bugs at {}"sv, cfg::bugreport_url)
		));
	#	endif
	#endif

	/* Setup Localization / i18n */
	QTranslator panko_localizer{};
	if (panko_localizer.load(QLocale(), "", "", ":/gui/i18n")) {
		QCoreApplication::installTranslator(&panko_localizer);
	}

	print_banner();

	/* Start application */
	Panko::gui::windows::Main panko_main{};

	panko_main.show();
	const auto ret{panko.exec()};


	return ret;
}

void print_banner() noexcept {
	print_version();
	std::print("\nPanko and the Panko GUI are licensed under the BSD-3-Clause <https://spdx.org/licenses/BSD-3-Clause.htm>\n");
}

void print_help() noexcept {
	print_banner();

}

void print_version() noexcept {
	std::print("Panko v{}\n", panko_version);
	std::print("  Python Version: {}\n", python_version());
	std::print("  Lua Version: {}\n", lua_version());
}
