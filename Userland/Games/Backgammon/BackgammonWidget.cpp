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
#include <LibCore/ElapsedTimer.h>
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
    widget->fps_timer = Core::ElapsedTimer::start_new();
    return widget;
}

void BackgammonWidget::paint_event(GUI::PaintEvent& event)
{
    auto elapsed = fps_timer.elapsed_milliseconds();
    long int delay = 10000;
    if (elapsed < delay) {
        usleep(delay-elapsed);
    }
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
    Color spaceLabelColor = Color(121, 96, 60);

    int hBorder = 8;
    int vBorder = 16;

    painter.fill_rect(rect(), frameColor);
    painter.fill_rect(Gfx::Rect(hBorder, vBorder, w-hBorder, h-vBorder*2), faceColor);

    int sw = (w-hBorder*2) / 14;
    int sh = (h-vBorder*2) / 2;
    if (sh > sw * 5) {
        sh = sw * 5;
    }
    if (sh > h/2-vBorder-30) {
        sh = h/2-vBorder-30;
    }
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
            result_path.line_to({ hBorder+i*sw+sw/2, vBorder+sh });
            result_path.line_to({ hBorder+i*sw, vBorder });
            painter.fill_path(result_path, cTop, Gfx::Painter::WindingRule::Nonzero);
        }
        {
            Gfx::Path result_path;
            result_path.move_to({ hBorder+i*sw, h-vBorder });
            result_path.line_to({ hBorder+i*sw+sw, h-vBorder });
            result_path.line_to({ hBorder+i*sw+sw/2, h-vBorder-sh });
            result_path.line_to({ hBorder+i*sw, h-vBorder });
            painter.fill_path(result_path, cBottom, Gfx::Painter::WindingRule::Nonzero);
        }

        int spaceTop = 13+i;
        int spaceBottom = 12-i;
        if (i >= 6) {
            spaceTop--;
            spaceBottom++;
        }

        painter.draw_text(Gfx::IntRect{hBorder+i*sw, 0, sw, vBorder}, MUST(String::formatted("{}", spaceTop)), Gfx::TextAlignment::Center, spaceLabelColor, Gfx::TextElision::None, Gfx::TextWrapping::DontWrap);
        painter.draw_text(Gfx::IntRect{hBorder+i*sw, h-vBorder, sw, vBorder}, MUST(String::formatted("{}", spaceBottom)), Gfx::TextAlignment::Center, spaceLabelColor, Gfx::TextElision::None, Gfx::TextWrapping::DontWrap);
    }
    int hx = hBorder+13*sw;
    painter.fill_rect(Gfx::Rect(hx, vBorder, hBorder, h-vBorder*2), frameColor);
    painter.fill_rect(Gfx::Rect(hx, vBorder+5*sw, w-hx, h-vBorder*2-10*sw), frameColor);

    for (int i = 0;i < 13; i++) {
        if (i == 0) {
            BackgammonWidget::draw_checkers(painter, hBorder+i*sw, vBorder, 5, Color::Black, false);
            BackgammonWidget::draw_checkers(painter, hBorder+i*sw, h-vBorder, 5, Color::White, true);
        } else if (i == 4) {
            BackgammonWidget::draw_checkers(painter, hBorder+i*sw, vBorder, 3, Color::White, false);
            BackgammonWidget::draw_checkers(painter, hBorder+i*sw, h-vBorder, 3, Color::Black, true);
        } else if (i == 7) {
            BackgammonWidget::draw_checkers(painter, hBorder+i*sw, vBorder, 5, Color::White, false);
            BackgammonWidget::draw_checkers(painter, hBorder+i*sw, h-vBorder, 5, Color::Black, true);
        } else if (i == 12) {
            BackgammonWidget::draw_checkers(painter, hBorder+i*sw, vBorder, 2, Color::Black, false);
            BackgammonWidget::draw_checkers(painter, hBorder+i*sw, h-vBorder, 2, Color::White, true);
        }
    }
            
    update();
    fps_timer.reset();
    fps_timer.start();
}

void BackgammonWidget::draw_checkers(GUI::Painter p, int x, int y, int count, Gfx::Color c, bool bottom)
{
    Gfx::AntiAliasingPainter painter { p };
    int hBorder = 8;
    int sw = (width()-hBorder*2) / 14;
    for (int i = 0; i < count; i++) {
        int cy = y+i*(sw-1);
        if (bottom) {
            cy = y-i*(sw-1)-(sw-1);
        }
        painter.fill_ellipse(Gfx::IntRect{x, cy, sw, sw-1}, c);
    }
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
