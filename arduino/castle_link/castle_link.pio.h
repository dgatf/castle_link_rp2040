/*
 * Copyright (c) 2022, Daniel Gorbea
 * All rights reserved.
 *
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree. 
 */

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

#define CASTLE_LINK_IRQ_NUM 2

// ----- //
// pulse //
// ----- //

#define pulse_wrap_target 0
#define pulse_wrap 4

static const uint16_t pulse_program_instructions[] = {
            //     .wrap_target
    0x20a0, //  0: wait   1 pin, 0                   
    0xff81, //  1: set    pindirs, 1             [31]
    0x2020, //  2: wait   0 pin, 0                   
    0xff80, //  3: set    pindirs, 0             [31]
    0xc004, //  4: irq    nowait 4                   
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program pulse_program = {
    .instructions = pulse_program_instructions,
    .length = 5,
    .origin = -1,
};

static inline pio_sm_config pulse_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + pulse_wrap_target, offset + pulse_wrap);
    return c;
}
#endif

// ------- //
// counter //
// ------- //

#define counter_wrap_target 3
#define counter_wrap 9

#define counter_offset_start 3u

static const uint16_t counter_program_instructions[] = {
    0xa0ca, //  0: mov    isr, !y                    
    0x8000, //  1: push   noblock                    
    0xc002, //  2: irq    nowait 2                   
            //     .wrap_target
    0xa04b, //  3: mov    y, !null                   
    0x20c4, //  4: wait   1 irq, 4                   
    0x4002, //  5: in     pins, 2                    
    0x407f, //  6: in     null, 31                   
    0xa026, //  7: mov    x, isr                     
    0x0020, //  8: jmp    !x, 0                      
    0x0085, //  9: jmp    y--, 5                     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program counter_program = {
    .instructions = counter_program_instructions,
    .length = 10,
    .origin = -1,
};

static inline pio_sm_config counter_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + counter_wrap_target, offset + counter_wrap);
    return c;
}
#endif

