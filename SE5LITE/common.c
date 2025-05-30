#include <stdint.h>
#include <project.h>
#include <libopencm3/stm32/gpio.h>
#include <common.h>
#include <pin.h>
#include <tick.h>
#include <power.h>
#include <chip.h>

#define MCU_SW_VER	8
#define DDR_TYPE	DDR_TYPE_LPDDR4X

static uint8_t board_type;

static int board_temp, soc_temp;

int get_board_temp(void)
{
	return board_temp;
}

int get_soc_temp(void)
{
	return soc_temp;
}

void set_board_temp(int temp)
{
	board_temp = temp;
}

void set_soc_temp(int temp)
{
	soc_temp = temp;
}

uint8_t get_ddr_type(void)
{
	return DDR_TYPE;
}

char *get_board_type_name()
{
	return "SE5";
}

void set_board_type(uint8_t type)
{
	board_type = type;
}

uint8_t get_board_type(void)
{
	return board_type;
}

uint8_t get_firmware_version(void)
{
	return MCU_SW_VER;
}

uint8_t get_declared_board_type(void)
{
	return *(uint8_t*)EEPROM_BOARD_TYPE_ADDR;
}

static inline void led_on(void)
{
#ifndef DEBUG
	gpio_clear(POWER_LED_PORT, POWER_LED_PIN);
#endif
}

static inline void led_off(void)
{
#ifndef DEBUG
	gpio_set(POWER_LED_PORT, POWER_LED_PIN);
#endif
}

int led_tick_handle;
int led_status;

void led_isr(void);

void led_init(void)
{
	/* donnot start now */
	led_tick_handle = tick_register_task(led_isr, 0);
}

/* if freq > 500, turn on led
 * if freq == 0, turn off led
 */
void led_set_frequency(unsigned long freq)
{
	if (freq > 500) {
		/* turn off tick task */
		tick_set_task_interval(led_tick_handle, 0);
		led_on();
	} else if (freq == 0) {
		tick_set_task_interval(led_tick_handle, 0);
		led_off();
	} else {
		tick_set_task_interval(led_tick_handle, 1000 / freq / 2);
	}
}

void led_isr(void)
{
	led_status = !led_status;
	if (led_status)
		led_on();
	else
		led_off();
}

int reset_key_status(void)
{
	return !gpio_get(RESET_KEY_PORT, RESET_KEY_PIN);
}
