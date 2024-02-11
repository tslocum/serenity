/*
 * Copyright (c) 2024, the SerenityOS developers.
 * Copyright (c) 2024, Trevor Slocum <trevor@rocket9labs.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "PromotionDialog.h"
#include <LibGUI/BoxLayout.h>
#include <LibGUI/Button.h>
#include <LibGUI/Frame.h>

PromotionDialog::PromotionDialog(BackgammonWidget& bkgm_widget)
    : Dialog(bkgm_widget.window())
    , m_selected_piece(Chess::Type::None)
{
    set_title("Choose piece to promote to");
    set_icon(bkgm_widget.window()->icon());
    resize(70 * 4, 70);

    auto main_widget = set_main_widget<GUI::Frame>();
    main_widget->set_frame_style(Gfx::FrameStyle::SunkenContainer);
    main_widget->set_fill_with_background_color(true);
    main_widget->set_layout<GUI::HorizontalBoxLayout>();

    for (auto const& type : { Chess::Type::Queen, Chess::Type::Knight, Chess::Type::Rook, Chess::Type::Bishop }) {
        auto& button = main_widget->add<GUI::Button>();
        button.set_fixed_height(70);
        button.set_icon(bkgm_widget.get_piece_graphic({ bkgm_widget.board().turn(), type }));
        button.on_click = [this, type](auto) {
            m_selected_piece = type;
            done(ExecResult::OK);
        };
    }
}

void PromotionDialog::event(Core::Event& event)
{
    Dialog::event(event);
}
