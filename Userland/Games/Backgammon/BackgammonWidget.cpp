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
    GUI::Frame::paint_event(event);

    GUI::Painter painter(*this);
    painter.add_clip_rect(event.rect());

    int w = width();
    int h = height();
    
    Color borderColor = Color(0, 0, 0);
    Color frameColor = Color(65, 40, 14);
    Color faceColor = Color(120, 63, 25);
    Color triangleA = Color(225, 188, 125);
    Color triangleB = Color(120, 17, 0);

    int hBorder = 10;
    int vBorder = 20;

    painter.fill_rect(rect(), frameColor);
    painter.fill_rect(Gfx::Rect(hBorder, vBorder, w-hBorder, h-vBorder*2), faceColor);

    int sw = (w-hBorder*2) / 14;
    int sh = (h-vBorder*2) / 2;
    for (int i = 0;i < 13; i++) {
        if (i == 6) {
            painter.fill_rect(Gfx::Rect(hBorder+i*sw, vBorder, sw, h-vBorder*2), frameColor);
            Gfx::Path result_path;
            result_path.move_to({ hBorder+i*sw+sw/2, 0 });
            result_path.line_to({ hBorder+i*sw+sw/2, h });
            painter.stroke_path(result_path, borderColor, 2);
            continue;
        }

        Color cTop = triangleA;
        Color cBottom = triangleB;
        if ((i >= 6) != (i % 2 != 0)) {
            cTop = triangleB;
            cBottom = triangleA;
        }
        {
            Gfx::Path result_path;
            result_path.move_to({ hBorder+i*sw, vBorder });
            result_path.line_to({ hBorder+i*sw+sw, vBorder });
            result_path.line_to({ hBorder+i*sw+sw/2, sh });
            result_path.line_to({ hBorder+i*sw, vBorder });
            painter.fill_path(result_path, cTop, Gfx::Painter::WindingRule::Nonzero);
        }
        {
            Gfx::Path result_path;
            result_path.move_to({ hBorder+i*sw, h-vBorder });
            result_path.line_to({ hBorder+i*sw+sw, h-vBorder });
            result_path.line_to({ hBorder+i*sw+sw/2, sh });
            result_path.line_to({ hBorder+i*sw, h-vBorder });
            painter.fill_path(result_path, cBottom, Gfx::Painter::WindingRule::Nonzero);
        }
    }
    int hx = hBorder+13*sw;
    painter.fill_rect(Gfx::Rect(hx, vBorder, hBorder, h-vBorder*2), frameColor);
            
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
    if (!frame_inner_rect().contains(event.position()))
        return;

    update();
}

void BackgammonWidget::reset()
{
    update();
}
