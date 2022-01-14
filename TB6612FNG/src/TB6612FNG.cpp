/*
 * TB6612FNG.cpp
 *
 *  Created on: 11 de jan. de 2022
 *      Author: igor_
 */

#include <TB6612FNG.hpp>
#include "driver/mcpwm.h"
#include "driver/gpio.h"

tb6612_t tb_channel_a;
tb6612_t tb_channel_b;

namespace TB6612FNG {

mcpwm_unit_t pwm_unit;
mcpwm_timer_t pwm_timer;

gpio_num_t stby_pin;

void set_standby_pin(gpio_num_t stby_pin) {
	gpio_config_t conf;

	conf.intr_type = GPIO_INTR_DISABLE;
	conf.mode = GPIO_MODE_OUTPUT;
	conf.pin_bit_mask = 0x1<<stby_pin;
	conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	conf.pull_up_en = GPIO_PULLUP_ENABLE;
	gpio_config(&conf);

	gpio_set_level(stby_pin, 1);

	TB6612FNG::stby_pin = stby_pin;
}

void tb6612_mcpwm_config(tb6612_pwm_config_t *conf) {

	mcpwm_config_t pwm_conf;

	pwm_conf.frequency = conf->pwm_freq;
	pwm_conf.cmpr_a = 0;
	pwm_conf.cmpr_b = 0;
	pwm_conf.counter_mode = MCPWM_UP_COUNTER;
	pwm_conf.duty_mode = MCPWM_DUTY_MODE_0;

	TB6612FNG::pwm_unit = conf->pwm_unit;
	TB6612FNG::pwm_timer = conf->pwm_timer;

	mcpwm_init(conf->pwm_unit, conf->pwm_timer, &pwm_conf);
}

void tb6612::tb6612_gpio_config(tb6612_gpio_config_t *conf) {
	gpio_config_t gpio_conf;

	gpio_conf.intr_type = GPIO_INTR_DISABLE;
	gpio_conf.mode = GPIO_MODE_OUTPUT;
	gpio_conf.pin_bit_mask = 0x1<<conf->in1_pin | 0x1<<conf->in2_pin;
	gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	gpio_conf.pull_up_en = GPIO_PULLUP_ENABLE;
	gpio_config(&gpio_conf); // TODO

	tb6612::in1_pin = conf->in1_pin;
	tb6612::in2_pin = conf->in2_pin;

	tb6612::inverted = conf->inverted;

	mcpwm_io_signals_t pwm_io_signals = static_cast<mcpwm_io_signals_t>(conf->pwm_operator * 2 + conf->pwm_generator);

	tb6612::pwm_generator = static_cast<mcpwm_generator_t>(conf->pwm_generator);

	mcpwm_gpio_init(TB6612FNG::pwm_unit, pwm_io_signals, conf->pwm_pin);
}

void tb6612::set_duty(float duty_cycle) {
	mcpwm_set_duty(TB6612FNG::pwm_unit, TB6612FNG::pwm_timer, tb6612::pwm_generator, duty_cycle);
}

void tb6612::direction_forward() {
	gpio_set_level(in1_pin, !inverted);
	gpio_set_level(in2_pin, inverted);
}

void tb6612::direction_backward() {
	gpio_set_level(in1_pin, inverted);
	gpio_set_level(in2_pin, !inverted);
}

void tb6612::drive(float duty_cycle) {
	(duty_cycle >= 0) ? direction_forward() : direction_backward();
	set_duty(abs(duty_cycle));
}

void tb6612::stop() {
	gpio_set_level(in1_pin, 0);
	gpio_set_level(in2_pin, 0);
}

void tb6612::short_brake() {
	gpio_set_level(in1_pin, 1);
	gpio_set_level(in2_pin, 1);
}

} /* namespace tb6612 */
