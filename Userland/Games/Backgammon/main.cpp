/*
 * Copyright (c) 2024, the SerenityOS developers.
 * Copyright (c) 2024, Trevor Slocum <trevor@rocket9labs.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "BackgammonWidget.h"
#include <LibConfig/Client.h>
#include <LibCore/System.h>
#include <LibDesktop/Launcher.h>
#include <LibFileSystemAccessClient/Client.h>
#include <LibGUI/ActionGroup.h>
#include <LibGUI/Application.h>
#include <LibGUI/Clipboard.h>
#include <LibGUI/Icon.h>
#include <LibGUI/Menu.h>
#include <LibGUI/Menubar.h>
#include <LibGUI/MessageBox.h>
#include <LibGUI/Process.h>
#include <LibGUI/Window.h>
#include <LibMain/Main.h>

ErrorOr<int> serenity_main(Main::Arguments arguments)
{
    TRY(Core::System::pledge("stdio rpath recvfd sendfd thread proc exec unix"));

    auto app = TRY(GUI::Application::create(arguments));

    Config::pledge_domain("Games");
    Config::monitor_domain("Games");

    TRY(Desktop::Launcher::add_allowed_handler_with_only_specific_urls("/bin/Help", { URL::create_with_file_scheme("/usr/share/man/man6/Backgammon.md") }));
    TRY(Desktop::Launcher::seal_allowlist());

    auto app_icon = TRY(GUI::Icon::try_create_default_icon("app-backgammon"sv));

    auto window = GUI::Window::construct();
    auto widget = TRY(BackgammonWidget::try_create());
    widget->fps_timer.start();
    window->set_main_widget(widget);

    TRY(Core::System::unveil("/etc/passwd", "r"));
    TRY(Core::System::unveil("/res", "r"));
    TRY(Core::System::unveil("/bin/GamesSettings", "x"));
    TRY(Core::System::unveil("/tmp/session/%sid/portal/launch", "rw"));
    TRY(Core::System::unveil("/tmp/session/%sid/portal/filesystemaccess", "rw"));
    TRY(Core::System::unveil(nullptr, nullptr));

    window->set_title("Backgammon");
    window->set_base_size({ 4, 4 });
    window->set_size_increment({ 8, 8 });
    window->resize(508, 508);

    window->set_icon(app_icon.bitmap_for_size(16));

    auto game_menu = window->add_menu("&Game"_string);

    game_menu->add_action(GUI::Action::create("&New Game", { Mod_None, Key_F2 }, TRY(Gfx::Bitmap::load_from_file("/res/icons/16x16/reload.png"sv)), [&](auto&) {
        widget->reset();
    }));
    game_menu->add_separator();

    game_menu->add_action(GUI::CommonActions::make_quit_action([](auto&) {
        GUI::Application::the()->quit();
    }));

    auto view_menu = window->add_menu("&View"_string);
    view_menu->add_action(GUI::CommonActions::make_fullscreen_action([&](auto&) {
        window->set_fullscreen(!window->is_fullscreen());
    }));

    auto help_menu = window->add_menu("&Help"_string);
    help_menu->add_action(GUI::CommonActions::make_command_palette_action(window));
    help_menu->add_action(GUI::CommonActions::make_help_action([](auto&) {
        Desktop::Launcher::open(URL::create_with_file_scheme("/usr/share/man/man6/Backgammon.md"), "/bin/Help");
    }));
    help_menu->add_action(GUI::CommonActions::make_about_action("Backgammon"_string, app_icon, window));

    window->show();
    widget->reset();

    return app->exec();
}
