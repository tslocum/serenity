/*
 * Copyright (c) 2024, the SerenityOS developers.
 * Copyright (c) 2024, Trevor Slocum <trevor@rocket9labs.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/HashMap.h>
#include <AK/NonnullRefPtr.h>
#include <AK/Optional.h>
#include <LibConfig/Listener.h>
#include <LibGUI/Frame.h>
#include <LibGfx/Bitmap.h>

class BackgammonWidget final
    : public GUI::Frame
    , public Config::Listener {
    C_OBJECT_ABSTRACT(BackgammonWidget);

public:
    static ErrorOr<NonnullRefPtr<BackgammonWidget>> try_create();

    virtual ~BackgammonWidget() override = default;

    virtual void paint_event(GUI::PaintEvent&) override;
    virtual void mousedown_event(GUI::MouseEvent&) override;
    virtual void mouseup_event(GUI::MouseEvent&) override;
    virtual void mousemove_event(GUI::MouseEvent&) override;

    virtual void draw_checkers(GUI::Painter painter, int x, int y, int count, Gfx::Color c, bool bottom);

    void reset();

private:
    enum class ClaimDrawBehavior {
        Always,
        Prompt
    };

    BackgammonWidget() = default;
};
