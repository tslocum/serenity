/*
 * Copyright (c) 2024, the SerenityOS developers.
 * Copyright (c) 2024, Trevor Slocum <trevor@rocket9labs.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "BackgammonWidget.h"
#include <LibGUI/Dialog.h>

class PromotionDialog final : public GUI::Dialog {
    C_OBJECT(PromotionDialog)
public:
    Chess::Type selected_piece() const { return m_selected_piece; }

private:
    explicit PromotionDialog(BackgammonWidget& bkgm_widget);
    virtual void event(Core::Event&) override;

    Chess::Type m_selected_piece;
};
