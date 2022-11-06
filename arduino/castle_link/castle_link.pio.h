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
    0xe081, //  1: set    pindirs, 1                 
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

#define counter_wrap_target 6
#define counter_wrap 12

#define counter_offset_start 6u

static const uint16_t counter_program_instructions[] = {
    0x4001, //  0: in     pins, 1                    
    0x407f, //  1: in     null, 31                   
    0x8000, //  2: push   noblock                    
    0xa0ca, //  3: mov    isr, !y                    
    0x8000, //  4: push   noblock                    
    0xc002, //  5: irq    nowait 2                   
            //     .wrap_target
    0xa04b, //  6: mov    y, !null                   
    0x20c4, //  7: wait   1 irq, 4                   
    0x4002, //  8: in     pins, 2                    
    0x407f, //  9: in     null, 31                   
    0xa026, // 10: mov    x, isr                     
    0x0020, // 11: jmp    !x, 0                      
    0x0088, // 12: jmp    y--, 8                     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program counter_program = {
    .instructions = counter_program_instructions,
    .length = 13,
    .origin = -1,
};

static inline pio_sm_config counter_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + counter_wrap_target, offset + counter_wrap);
    return c;
}
#endif

