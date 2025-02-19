// SPDX-License-Identifier: BSD-3-Clause
#pragma once
#if !defined(PANKO_GUI_DIALOGS_ABOUT_HH)
#define PANKO_GUI_DIALOGS_ABOUT_HH

#include "panko/internal/defs.hh"

#include <memory>

PANKO_DIAGNOSTICS_PUSH()
PANKO_DIAGNOSTICS_IGNORE("-Wsign-conversion")
PANKO_DIAGNOSTICS_IGNORE("-Warith-conversion")
PANKO_DIAGNOSTICS_IGNORE("-Wconversion")
#include <QDialog>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QSpacerItem>
PANKO_DIAGNOSTICS_POP()

namespace Panko::gui::dialogs {
	struct About final : public QDialog {
		Q_OBJECT
	private:
		std::unique_ptr<QHBoxLayout> _horizontal_layout;
		std::unique_ptr<QVBoxLayout> _sidebar_layout;
		std::unique_ptr<QLabel> _sidebar_img;
		std::unique_ptr<QSpacerItem> _sidebar_spacer;
		std::unique_ptr<QLabel> _sidebar_version;
		std::unique_ptr<QLabel> _sidebar_py_version;
		std::unique_ptr<QLabel> _sidebar_lua_version;
		std::unique_ptr<QLabel> _sidebar_platform;
		std::unique_ptr<QLabel> _sidebar_compiler;

		std::unique_ptr<QTabWidget> _about_tabs;

		std::unique_ptr<QWidget> _tab_about;
		std::unique_ptr<QVBoxLayout> _about_layout;
		std::unique_ptr<QLabel> _about_text;
		std::unique_ptr<QHBoxLayout> _flag_layout;
		std::unique_ptr<QLabel> _flag_trans;
		std::unique_ptr<QLabel> _flag_lesbian;

		std::unique_ptr<QWidget> _tab_components;
		std::unique_ptr<QVBoxLayout> _components_layout;
		std::unique_ptr<QTextEdit> _components_text;

		std::unique_ptr<QWidget> _tab_contributors;
		std::unique_ptr<QVBoxLayout> _contributors_layout;
		std::unique_ptr<QTextEdit> _contributors_text;

		std::unique_ptr<QWidget> _tab_license;
		std::unique_ptr<QVBoxLayout> _license_layout;
		std::unique_ptr<QTextEdit> _license_text;

		void _add_about_text();
		void _add_components_text();
		void _add_contributors_text();
		void _add_license_text();
	public:
		About(QWidget* parent = nullptr) noexcept;
		~About() = default;
	};
}

#endif /* PANKO_GUI_DIALOGS_ABOUT_HH */
