// SPDX-License-Identifier: BSD-3-Clause

#include "panko/internal/defs.hh"
#include "panko/config.hh"

#include "panko-gui/windows/main.hh"

#include "panko-gui/dialogs/about.hh"

PANKO_DIAGNOSTICS_PUSH()
PANKO_DIAGNOSTICS_IGNORE("-Wconversion")
PANKO_DIAGNOSTICS_IGNORE("-Warith-conversion")
PANKO_DIAGNOSTICS_IGNORE("-Wsign-conversion")
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
PANKO_DIAGNOSTICS_POP()

namespace cfg = Panko::config;

namespace Panko::gui::windows {
	Main::Main(QWidget* parent) noexcept : QMainWindow{parent} {
		setAttribute(Qt::WA_NativeWindow);

		setMinimumSize(640, 480);
		resize(1040, 600);

		/* Setup the main layout */
		_setup_layout();
		/* Setup the menus/status bar */
		_setup_actions();
		_setup_menus();
		// _setup_toolbar();
		_setup_statusbar();
	}

	void Main::_setup_actions() noexcept {
		_setup_actions_file();
		_setup_actions_edit();
		_setup_actions_view();
		_setup_actions_go();
		_setup_actions_analysis();
		_setup_actions_tools();
		_setup_actions_help();
	}

	void Main::_setup_actions_file() noexcept {
		_action_file_open = std::make_unique<QAction>(tr("Open"), this);
		_action_file_open->setIcon(QIcon::fromTheme(""));
		_action_file_open->setShortcut(QKeySequence::Open);
		_action_file_open->setStatusTip(tr("Open Capture File"));
		connect(_action_file_open.get(), &QAction::triggered, this, [this](){

		});

		_action_file_recent_clear = std::make_unique<QAction>(tr("Clear Recent"), this);
		_action_file_recent_clear->setIcon(QIcon::fromTheme("edit-clear-history"));
		// _action_file_recent_clear->setShortcut(...);
		_action_file_recent_clear->setStatusTip(tr("Clear all recently opened files"));
		connect(_action_file_recent_clear.get(), &QAction::triggered, this, [this](){

		});

		_action_file_close = std::make_unique<QAction>(tr("Close"), this);
		_action_file_close->setIcon(QIcon::fromTheme("document-close"));
		_action_file_close->setShortcut(QKeySequence::Close);
		_action_file_close->setStatusTip(tr("Close open capture"));
		connect(_action_file_close.get(), &QAction::triggered, this, [this](){

		});
//
// 		_action_file_export = std::make_unique<QAction>(tr(""), this);
// 		_action_file_export->setIcon(QIcon::fromTheme(""));
// 		// _action_file_export->setShortcut(...);
// 		_action_file_export->setStatusTip(tr(""));
// 		connect(_action_file_export.get(), &QAction::triggered, this, [this](){
//
// 		});

		_action_file_exit = std::make_unique<QAction>(tr("Exit"), this);
		_action_file_exit->setIcon(QIcon::fromTheme("application-exit"));
		_action_file_exit->setShortcut(QKeySequence::Quit);
		_action_file_exit->setStatusTip(tr("Exit Panko"));
		connect(_action_file_exit.get(), &QAction::triggered, this, &Main::close);
	}

	void Main::_setup_actions_edit() noexcept {

	}

	void Main::_setup_actions_view() noexcept {

	}

	void Main::_setup_actions_go() noexcept {

	}

	void Main::_setup_actions_analysis() noexcept {

	}

	void Main::_setup_actions_tools() noexcept {

	}

	void Main::_setup_actions_help() noexcept {
		_action_help_about = std::make_unique<QAction>(tr("About"), this);
		_action_help_about->setIcon(QIcon::fromTheme("help-about"));
		// _action_help_about->setShortcut(...);
		_action_help_about->setStatusTip(tr("About Panko"));
		connect(_action_help_about.get(), &QAction::triggered, this, [this](){
			dialogs::About about{this};
			about.exec();
		});

		_action_help_website = std::make_unique<QAction>(tr("Website"), this);
		_action_help_website->setIcon(QIcon::fromTheme("globe"));
		// _action_help_website->setShortcut(...);
		_action_help_website->setStatusTip(tr("Go to the Panko Website"));
		connect(_action_help_website.get(), &QAction::triggered, this, [](){
			QDesktopServices::openUrl(QUrl("https://github.com/lethalbit/Panko"));
		});

		_action_help_documentation = std::make_unique<QAction>(tr("Documentation"), this);
		_action_help_documentation->setIcon(QIcon::fromTheme("documentation"));
		// _action_help_documentation->setShortcut(...);
		_action_help_documentation->setStatusTip(tr("View the Panko documentation"));
		connect(_action_help_documentation.get(), &QAction::triggered, this, [](){
			QDesktopServices::openUrl(QUrl("https://github.com/lethalbit/Panko"));
		});

		_action_help_report_bug = std::make_unique<QAction>(tr("Report A Bug"), this);
		_action_help_report_bug->setIcon(QIcon::fromTheme("tools-report-bug"));
		// _action_help_report_bug->setShortcut(...);
		_action_help_report_bug->setStatusTip(tr("Report a bug with Panko"));
		connect(_action_help_report_bug.get(), &QAction::triggered, this, [](){
			QDesktopServices::openUrl(QUrl(cfg::bugreport_url.data()));
		});
	}


	void Main::_setup_menus() noexcept {
		_main_menu = new QMenuBar(this);
		setMenuBar(_main_menu);

		_setup_menus_file();
		_setup_menus_edit();
		_setup_menus_view();
		_setup_menus_go();
		_setup_menus_analysis();
		_setup_menus_tools();
		_setup_menus_help();
	}


	void Main::_setup_menus_file() noexcept {
		_menu_file = std::make_unique<QMenu>(tr("&File"), _main_menu);
		_main_menu->addAction(_menu_file->menuAction());


		_menu_file_recent_open = std::make_unique<QMenu>(tr("Open Recent"), _main_menu);
		/* TODO(aki): Populate/Handle recent files list */
		_menu_file_recent_open->setIcon(QIcon::fromTheme("document-open-recent"));
		_menu_file_recent_open->addSeparator();
		_menu_file_recent_open->addAction(_action_file_recent_clear.get());

		_menu_file_export_as = std::make_unique<QMenu>(tr("Export As..."), _main_menu);
		_menu_file_export_as->setIcon(QIcon::fromTheme("document-export"));

		_menu_file->addAction(_action_file_open.get());
		_menu_file->addAction(_menu_file_recent_open->menuAction());
		_menu_file->addAction(_action_file_close.get());
		_menu_file->addSeparator();
		_menu_file->addAction(_menu_file_export_as->menuAction());
		_menu_file->addSeparator();
		_menu_file->addAction(_action_file_exit.get());
	}

	void Main::_setup_menus_edit() noexcept {
		_menu_edit = std::make_unique<QMenu>(tr("&Edit"), _main_menu);
		_main_menu->addAction(_menu_edit->menuAction());

	}

	void Main::_setup_menus_view() noexcept {
		_menu_view = std::make_unique<QMenu>(tr("&View"), _main_menu);
		_main_menu->addAction(_menu_view->menuAction());

	}

	void Main::_setup_menus_go() noexcept {
		_menu_go = std::make_unique<QMenu>(tr("&Go"), _main_menu);
		_main_menu->addAction(_menu_go->menuAction());

	}

	void Main::_setup_menus_analysis() noexcept {
		_menu_analysis = std::make_unique<QMenu>(tr("&Analysis"), _main_menu);
		_main_menu->addAction(_menu_analysis->menuAction());

	}

	void Main::_setup_menus_tools() noexcept {
		_menu_tools = std::make_unique<QMenu>(tr("&Tools"), _main_menu);
		_main_menu->addAction(_menu_tools->menuAction());

	}

	void Main::_setup_menus_help() noexcept {
		_menu_help = std::make_unique<QMenu>(tr("&Help"), _main_menu);
		_main_menu->addAction(_menu_help->menuAction());

		_menu_help->addAction(_action_help_website.get());
		_menu_help->addAction(_action_help_documentation.get());
		_menu_help->addAction(_action_help_report_bug.get());
		_menu_help->addSeparator();
		_menu_help->addAction(_action_help_about.get());

	}

	void Main::_setup_toolbar() noexcept {
		_tool_bar = new QToolBar(this);
		addToolBar(Qt::ToolBarArea::AllToolBarAreas, _tool_bar);

		_tool_bar->addAction(_action_help_about.get());
	}

	void Main::_setup_statusbar() noexcept {
		_status_bar = new QStatusBar(this);

		_status_bar->setSizeGripEnabled(true);

		setStatusBar(_status_bar);
	}

	void Main::_setup_layout() noexcept {

	}

}
