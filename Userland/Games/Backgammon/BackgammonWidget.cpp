/*
 * Copyright (c) 2024, the SerenityOS developers.
 * Copyright (c) 2024, Trevor Slocum <trevor@rocket9labs.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "BackgammonWidget.h"
#include <AK/Random.h>
#include <AK/String.h>
#include <LibCore/Account.h>
#include <LibCore/DateTime.h>
#include <LibCore/File.h>
#include <LibGUI/MessageBox.h>
#include <LibGUI/Painter.h>
#include <LibGfx/AntiAliasingPainter.h>
#include <LibGfx/Font/Font.h>
#include <LibGfx/Font/FontDatabase.h>
#include <LibGfx/Palette.h>
#include <LibGfx/Path.h>
#include <unistd.h>

ErrorOr<NonnullRefPtr<BackgammonWidget>> BackgammonWidget::try_create()
{
    auto widget = TRY(AK::adopt_nonnull_ref_or_enomem(new (nothrow) BackgammonWidget));

    return widget;
}

void BackgammonWidget::paint_event(GUI::PaintEvent& event)
{
    //int const min_size = min(width(), height());
    //int const widget_offset_x = (window()->width() - min_size) / 2;
    //int const widget_offset_y = (window()->height() - min_size) / 2;

    GUI::Frame::paint_event(event);

    GUI::Painter painter(*this);
    painter.add_clip_rect(event.rect());
    
    painter.fill_rect(frame_inner_rect(), Color::Black);
    Gfx::Path result_path;
    result_path.move_to({ 0, 0 });
    result_path.line_to({ 10, 10 });
    result_path.line_to({ 0, 20 });
    result_path.line_to({ 0, 0 });
    painter.fill_path(result_path, Color::White, Gfx::Painter::WindingRule::Nonzero);

    update();
}

void BackgammonWidget::mousedown_event(GUI::MouseEvent& event)
{
    if (!frame_inner_rect().contains(event.position()))
        return;

    update();
}

void BackgammonWidget::mouseup_event(GUI::MouseEvent& event)
{
    if (!frame_inner_rect().contains(event.position()))
        return;

    update();
}

void BackgammonWidget::mousemove_event(GUI::MouseEvent& event)
{
    //int const min_size = min(width(), height());
    //int const widget_offset_x = (window()->width() - min_size) / 2;
    //int const widget_offset_y = (window()->height() - min_size) / 2;

    if (!frame_inner_rect().contains(event.position()))
        return;

    update();
}

void BackgammonWidget::reset()
{
    update();
}
