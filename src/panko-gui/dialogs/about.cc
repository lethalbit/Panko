// SPDX-License-Identifier: BSD-3-Clause

#include <memory>
#include <format>

#include "panko/config.hh"

#include "panko-gui/dialogs/about.hh"

PANKO_DIAGNOSTICS_PUSH()
PANKO_DIAGNOSTICS_IGNORE("-Wsign-conversion")
PANKO_DIAGNOSTICS_IGNORE("-Warith-conversion")
PANKO_DIAGNOSTICS_IGNORE("-Wconversion")
#include <QIcon>
#include <QFile>
PANKO_DIAGNOSTICS_POP()

namespace Panko::gui::dialogs {
	namespace cfg = Panko::config;

	About::About(QWidget* parent) noexcept : QDialog{parent} {
		setWindowTitle(tr("About"));

		setMinimumSize(900, 550);
		setMaximumSize(900, 550);
		resize(900, 550);

		_horizontal_layout = std::make_unique<QHBoxLayout>();
		_sidebar_layout = std::make_unique<QVBoxLayout>();

		_sidebar_img = std::make_unique<QLabel>();
		_sidebar_spacer = std::make_unique<QSpacerItem>(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
		_sidebar_version = std::make_unique<QLabel>();
		_sidebar_py_version = std::make_unique<QLabel>();
		_sidebar_lua_version = std::make_unique<QLabel>();
		_sidebar_platform = std::make_unique<QLabel>();
		_sidebar_compiler = std::make_unique<QLabel>();

		/* TODO(aki): PLACEHOLDER */
		_sidebar_img->setPixmap(QIcon::fromTheme("cpu").pixmap(256, 256));
		_sidebar_img->setAlignment(Qt::AlignTop | Qt::AlignLeft);

		_sidebar_version->setText(QString::fromStdString(
			std::format("Version: {}"sv, cfg::version_full)
		));
		_sidebar_py_version->setText(QString::fromStdString(
			std::format("Python: {}"sv, cfg::python_version)
		));
		_sidebar_lua_version->setText(QString::fromStdString(
			std::format("Lua: {} {}"sv, cfg::lua_backend, cfg::lua_version)
		));
		_sidebar_platform->setText(QString::fromStdString(
			std::format("Platform: {}-{}"sv, cfg::target_system, cfg::target_arch)
		));
		_sidebar_compiler->setText(QString::fromStdString(
			std::format("Compiler: {} {}"sv, cfg::compiler_name, cfg::compiler_version)
		));

		_sidebar_layout->addWidget(_sidebar_img.get());
		_sidebar_layout->addItem(_sidebar_spacer.release());
		_sidebar_layout->addWidget(_sidebar_version.get());
		_sidebar_layout->addWidget(_sidebar_py_version.get());
		_sidebar_layout->addWidget(_sidebar_lua_version.get());
		_sidebar_layout->addWidget(_sidebar_platform.get());
		_sidebar_layout->addWidget(_sidebar_compiler.get());

		_horizontal_layout->addLayout(_sidebar_layout.get());

		_about_tabs = std::make_unique<QTabWidget>();
		_about_tabs->setIconSize(QSize(32, 32));

		_tab_about = std::make_unique<QWidget>();
		_tab_components = std::make_unique<QWidget>();
		_tab_contributors = std::make_unique<QWidget>();
		_tab_license = std::make_unique<QWidget>();

		_about_layout = std::make_unique<QVBoxLayout>();
		_flag_layout = std::make_unique<QHBoxLayout>();
		_components_layout = std::make_unique<QVBoxLayout>();
		_contributors_layout = std::make_unique<QVBoxLayout>();
		_license_layout = std::make_unique<QVBoxLayout>();

		_tab_about->setLayout(_about_layout.get());
		_tab_components->setLayout(_components_layout.get());
		_tab_contributors->setLayout(_contributors_layout.get());
		_tab_license->setLayout(_license_layout.get());

		_about_text = std::make_unique<QLabel>();
		_flag_trans = std::make_unique<QLabel>();
		_flag_trans->setPixmap(QIcon(":/img/trans-flag.svg").pixmap(256, 512));
		_flag_trans->setAlignment(Qt::AlignCenter);
		_flag_lesbian = std::make_unique<QLabel>();
		_flag_lesbian->setPixmap(QIcon(":/img/lesbian-flag.svg").pixmap(256, 512));
		_flag_lesbian->setAlignment(Qt::AlignCenter);
		_components_text = std::make_unique<QTextEdit>();
		_contributors_text = std::make_unique<QTextEdit>();
		_license_text = std::make_unique<QTextEdit>();

		_about_layout->addWidget(_about_text.get());
		_about_layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
		_flag_layout->addWidget(_flag_trans.get());
		_flag_layout->addWidget(_flag_lesbian.get());
		_about_layout->addLayout(_flag_layout.get());
		_components_layout->addWidget(_components_text.get());
		_contributors_layout->addWidget(_contributors_text.get());
		_license_layout->addWidget(_license_text.get());

		/* TODO(aki): PLACEHOLDERS */
		_about_tabs->addTab(_tab_about.release(), QIcon::fromTheme("help-about"), tr("About"));
		_about_tabs->addTab(_tab_components.release(), QIcon::fromTheme("media-floppy"), tr("Components"));
		_about_tabs->addTab(_tab_contributors.release(), QIcon::fromTheme("im-user"), tr("Contributors"));
		_about_tabs->addTab(_tab_license.release(), QIcon::fromTheme("license"), tr("License"));

		_horizontal_layout->addWidget(_about_tabs.get());

		_add_about_text();
		_add_components_text();
		_add_contributors_text();
		_add_license_text();

		setLayout(_horizontal_layout.get());
	}

	void About::_add_about_text() {
		QFile about{":/txt/about"};
		about.open(QIODevice::ReadOnly);

		_about_text->setText(QTextStream(&about).readAll());
		about.close();

		_about_text->setAlignment(Qt::AlignTop | Qt::AlignLeft);
		_about_text->setWordWrap(true);

	}

	void About::_add_components_text() {

		QFile components{":/txt/components"};
		components.open(QIODevice::ReadOnly);

		_components_text->setText(QTextStream(&components).readAll());
		components.close();

		_components_text->setAlignment(Qt::AlignTop | Qt::AlignLeft);
		_components_text->setReadOnly(true);
	}

	void About::_add_contributors_text() {
		QFile contributors{":/txt/contributors"};
		contributors.open(QIODevice::ReadOnly);

		_contributors_text->setText(QTextStream(&contributors).readAll());
		contributors.close();

		_contributors_text->setAlignment(Qt::AlignTop | Qt::AlignLeft);
		_contributors_text->setReadOnly(true);
	}

	void About::_add_license_text() {
		QFile license{":/txt/license"};
		license.open(QIODevice::ReadOnly);

		_license_text->setPlainText(QTextStream(&license).readAll());
		license.close();

		_license_text->setReadOnly(true);
	}
}
