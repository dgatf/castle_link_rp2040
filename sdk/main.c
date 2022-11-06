/*
 * -----------------------------------------------------------------------------------------
 *
 * Copyright (c) 2022, Daniel Gorbea
 * All rights reserved.
 *
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * -----------------------------------------------------------------------------------------
 *
 *  Library for Castle link protocol for RP2040:
 *
 *  Base pin (gpio) is 4. Connect receiver signal throttle to pin 4 and esc signal to pin 5 with pull up 1k - 6k
 *
 * -----------------------------------------------------------------------------------------
 */

#include "castle_link.h"

static void castle_link_handler(castle_link_telemetry_t packet)
{
    printf("\nVolt(V): %.2f Ripple volt(V): %.2f Curr(A): %.2f Thr: %.0f Out: %.0f Rpm: %.0f Bec volt(V): %.2f Bec curr(A): %.2f Temp(C): %.0f", packet.voltage, packet.ripple_voltage, packet.current, packet.thr, packet.output, packet.rpm, packet.voltage_bec, packet.current_bec, packet.temperature);
    if (packet.is_temp_ntc)
        printf(" NTC");
    else
        printf(" Linear");
}

int main()
{
    PIO pio = pio0;        // values: pio0, pio1
    uint pin_base = 4;          // gpio 4 = receiver signal. gpio 5 = esc signal. Any gpio is valid
    uint irq = PIO0_IRQ_0; // values for pio0: PIO0_IRQ_0, PIO0_IRQ_1. values for pio1: PIO1_IRQ_0, PIO1_IRQ_1

    stdio_init_all();

    castle_link_init(pio, pin_base, irq);
    castle_link_set_handler(castle_link_handler);

    while (1)
    {
    }
}
