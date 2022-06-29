/*
 * Copyright (c) 2022, Idan Horowitz <idan.horowitz@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibJS/Runtime/Intl/DurationFormat.h>
#include <LibJS/Runtime/PrototypeObject.h>

namespace JS::Intl {

class DurationFormatPrototype final : public PrototypeObject<DurationFormatPrototype, DurationFormat> {
    JS_PROTOTYPE_OBJECT(DurationFormatPrototype, DurationFormat, Intl.DurationFormat);

public:
    explicit DurationFormatPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~DurationFormatPrototype() override = default;
};

}
