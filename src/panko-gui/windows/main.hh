// SPDX-License-Identifier: BSD-3-Clause
#pragma once
#if !defined(PANKO_GUI_WINDOWS_MAIN_HH)
#define PANKO_GUI_WINDOWS_MAIN_HH

#include <memory>

#include "panko/internal/defs.hh"

PANKO_DIAGNOSTICS_PUSH()
PANKO_DIAGNOSTICS_IGNORE("-Wconversion")
PANKO_DIAGNOSTICS_IGNORE("-Warith-conversion")
PANKO_DIAGNOSTICS_IGNORE("-Wsign-conversion")
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMenu>
#include <QAction>

PANKO_DIAGNOSTICS_POP()

namespace Panko::gui::windows {
	struct Main final : public QMainWindow {
		Q_OBJECT
	private:
		void _setup_layout() noexcept;

		/* Menus */
		void _setup_menus() noexcept;

		void _setup_menus_file() noexcept;
		void _setup_menus_edit() noexcept;
		void _setup_menus_view() noexcept;
		void _setup_menus_go() noexcept;
		void _setup_menus_analysis() noexcept;
		void _setup_menus_tools() noexcept;
		void _setup_menus_help() noexcept;

		QMenuBar* _main_menu{nullptr};

		std::unique_ptr<QMenu> _menu_file;
		std::unique_ptr<QMenu> _menu_file_recent_open;
		std::unique_ptr<QMenu> _menu_file_export_as;
		std::unique_ptr<QMenu> _menu_edit;
		std::unique_ptr<QMenu> _menu_view;
		std::unique_ptr<QMenu> _menu_go;
		std::unique_ptr<QMenu> _menu_analysis;
		std::unique_ptr<QMenu> _menu_tools;
		std::unique_ptr<QMenu> _menu_help;

		/* Status Bar */
		void _setup_statusbar() noexcept;
		QStatusBar* _status_bar{nullptr};

		/* Toolbar */
		void _setup_toolbar() noexcept;
		QToolBar* _tool_bar{nullptr};

		/* Actions */
		void _setup_actions() noexcept;

		void _setup_actions_file() noexcept;
		void _setup_actions_edit() noexcept;
		void _setup_actions_view() noexcept;
		void _setup_actions_go() noexcept;
		void _setup_actions_analysis() noexcept;
		void _setup_actions_tools() noexcept;
		void _setup_actions_help() noexcept;

		/* Actions: File */
		std::unique_ptr<QAction> _action_file_open;
		std::unique_ptr<QAction> _action_file_recent_clear;
		std::unique_ptr<QAction> _action_file_close;
		// std::unique_ptr<QAction> _action_file_export_;
		std::unique_ptr<QAction> _action_file_exit;

		/* Actions: Edit */

		/* Actions: View */

		/* Actions: Go */

		/* Actions: Analysis */

		/* Actions: Tools */

		/* Actions: Help */
		std::unique_ptr<QAction> _action_help_website;
		std::unique_ptr<QAction> _action_help_documentation;
		std::unique_ptr<QAction> _action_help_report_bug;
		std::unique_ptr<QAction> _action_help_about;

	public:
		Main(QWidget* parent = nullptr) noexcept;
		~Main() = default;
	};
}

#endif /* PANKO_GUI_WINDOWS_MAIN_HH */
