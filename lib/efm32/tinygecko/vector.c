/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>,
 *               2012 chrysn <chrysn@fsfe.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/efm32/vector.h>

#define WEAK __attribute__ ((weak))

/* Symbols exported by the linker script(s). */
extern unsigned _etext, _data, _edata, _ebss, _stack;

void main(void);
void reset_handler(void);
void blocking_handler(void);
void null_handler(void);

void WEAK nmi_handler(void);
void WEAK hard_fault_handler(void);
void WEAK mem_manage_handler(void);
void WEAK bus_fault_handler(void);
void WEAK usage_fault_handler(void);
void WEAK sv_call_handler(void);
void WEAK debug_monitor_handler(void);
void WEAK pend_sv_handler(void);
void WEAK sys_tick_handler(void);


__attribute__ ((section(".vectors")))
efm32_vector_table_t vector_table = {
	.initial_sp_value = &_stack,
	.reset = reset_handler,
	.nmi = nmi_handler,
	.hard_fault = hard_fault_handler,
	.memory_manage_fault = mem_manage_handler,
	.bus_fault = bus_fault_handler,
	.usage_fault = usage_fault_handler,
	.sv_call = sv_call_handler,
	.pend_sv = pend_sv_handler,
	.systick = sys_tick_handler,
};

void reset_handler(void)
{
	volatile unsigned *src, *dest;

	__asm__("MSR msp, %0" : : "r"(&_stack));

	for (src = &_etext, dest = &_data; dest < &_edata; src++, dest++)
		*dest = *src;

	while (dest < &_ebss)
		*dest++ = 0;

	/* Call the application's entry point. */
	main();
}

void blocking_handler(void)
{
	while (1) ;
}

void null_handler(void)
{
	/* Do nothing. */
}

#pragma weak nmi_handler = null_handler
#pragma weak hard_fault_handler = blocking_handler
#pragma weak mem_manage_handler = blocking_handler
#pragma weak bus_fault_handler = blocking_handler
#pragma weak usage_fault_handler = blocking_handler
#pragma weak sv_call_handler = null_handler
#pragma weak debug_monitor_handler = null_handler
#pragma weak pend_sv_handler = null_handler
#pragma weak sys_tick_handler = null_handler
