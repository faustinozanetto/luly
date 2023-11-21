#pragma once

#include "shared_api.h"
#include <cstdint>

namespace luly::input
{
    using mouse_code = uint16_t;

    enum class LULY_SHARED_API mouse_button : mouse_code
    {
        button0 = 0,
        button1 = 1,
        button2 = 2,
        button3 = 3,
        button4 = 4,
        button5 = 5,
        button6 = 6,
        button7 = 7,

        button_last = button7,
        button_left = button0,
        button_right = button1,
        button_middle = button2
    };
}
