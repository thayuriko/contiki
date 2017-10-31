/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

#include "contiki.h"
#include "../platform/srf06-cc26xx/launchpad/cc2650/board.h"
#include "ti-lib.h"
#include "hwsensors.c"
#include "../cpu/cc26xx-cc13xx/lpm.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
//PROCESS(hello_world_process, "Hello world process");
//AUTOSTART_PROCESSES(&hello_world_process);

PROCESS(gpio_process, "Hello world process");
PROCESS(pwm_process, "PWM world process");
AUTOSTART_PROCESSES(&gpio_process, &pwm_process);

/*---------------------------------------------------------------------------
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  printf("Hello, world\n");

  PROCESS_END();
}
---------------------------------------------------------------------------*/
int16_t current_duty = 0;

PROCESS_THREAD(gpio_process, ev, data)
{
    static struct etimer et;
    static int8_t counter = 0;
    PROCESS_BEGIN();
    etimer_set(&et, CLOCK_SECOND * 1);

    /* initialization of GPIO output */
    ti_lib_rom_ioc_pin_type_gpio_output(IOID_27);
    ti_lib_gpio_clear_multi_dio(1<<IOID_27);

    ti_lib_rom_ioc_pin_type_gpio_output(IOID_30);
    ti_lib_gpio_clear_multi_dio(1<<IOID_30);

    while(1) {

        PROCESS_WAIT_EVENT();
        counter=(counter+1)%2;

        if(counter)
        {
            ti_lib_gpio_set_dio(IOID_27);
            ti_lib_gpio_clear_dio(IOID_30);
        }
        else
        {
            ti_lib_gpio_set_dio(IOID_30);
            ti_lib_gpio_clear_dio(IOID_27);
        }
        etimer_reset(&et);
    }

    PROCESS_END();
}

PROCESS_THREAD(pwm_process,ev,data)
{
    static int16_t loadvalue, ticks;
    static uint32_t bot0, bot1;
    static struct etimer asd;
    etimer_set(&asd, CLOCK_SECOND * 1);

    PROCESS_BEGIN();

    loadvalue = pwminit(5000);

    while(1)
    {
        PROCESS_WAIT_EVENT();
        if(ev == PROCESS_EVENT_TIMER){

            if((!bot0) && (current_duty < 100)){
                current_duty = current_duty + 10;
                delay(1000);
            }

            if((!bot1) && (current_duty > 10)){
                current_duty = current_duty - 10;
            }

            ti_lib_timer_match_set(GPT0_BASE, TIMER_A, loadvalue - ticks);
            ticks = (current_duty * loadvalue/100);

            printf("%d %d %d %d\n", bot0, bot1, current_duty, ticks);

            etimer_reset(&asd);
        }
    }

    PROCESS_END();
}
