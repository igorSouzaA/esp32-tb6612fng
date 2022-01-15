/*
 * TB6612FNG.cpp
 *
 *  Created on: 11 de jan. de 2022
 *      Author: igor_
 */

#include "TB6612FNG.hpp"
#include "driver/mcpwm.h"
#include "driver/gpio.h"

namespace TB6612FNG {

// tb6612 class methods

void tb6612::set_standby_pin(gpio_num_t stby_pin) {
	// GPIO configuration structure
	gpio_config_t conf;

	conf.intr_type = GPIO_INTR_DISABLE;
	conf.mode = GPIO_MODE_OUTPUT;
	conf.pin_bit_mask = 0x1<<stby_pin;
	conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	conf.pull_up_en = GPIO_PULLUP_ENABLE;

	// COnfigures the GPIOs
	gpio_config(&conf);

	// Stand-by pin should be set to one always
	gpio_set_level(stby_pin, 1);

	this->stby_pin = stby_pin;
}

void tb6612::tb6612_mcpwm_config(tb6612_pwm_config_t *conf) {
	// MCPWM configuration structure
	mcpwm_config_t pwm_conf;

	pwm_conf.frequency = conf->pwm_freq;
	pwm_conf.cmpr_a = 0;
	pwm_conf.cmpr_b = 0;
	pwm_conf.counter_mode = MCPWM_UP_COUNTER;
	pwm_conf.duty_mode = MCPWM_DUTY_MODE_0;

	// Configures the first requirements for the MCPWM
	mcpwm_init(conf->pwm_unit, conf->pwm_timer, &pwm_conf);

	this->pwm_unit = conf->pwm_unit;
	this->pwm_timer = conf->pwm_timer;
	this->pwm_operator = conf->pwm_operator;
}

	// channel class methods

	void tb6612::channel::set_parent_pointer(tb6612* p) {
		this->ptr_tb6612 = p;
	}

	void tb6612::channel::channel_gpio_config(tb6612_channel_gpio_config_t *conf) {
		// GPIO configuration structure
		gpio_config_t gpio_conf;

		gpio_conf.intr_type = GPIO_INTR_DISABLE;
		gpio_conf.mode = GPIO_MODE_OUTPUT;
		gpio_conf.pin_bit_mask = 0x1<<conf->in1_pin | 0x1<<conf->in2_pin;
		gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
		gpio_conf.pull_up_en = GPIO_PULLUP_ENABLE;
		gpio_config(&gpio_conf);

		this->in1_pin = conf->in1_pin;
		this->in2_pin = conf->in2_pin;

		this->inverted = conf->inverted;

		mcpwm_io_signals_t pwm_io_signals = static_cast<mcpwm_io_signals_t>(ptr_tb6612->pwm_operator * 2 + conf->pwm_generator);

		this->pwm_generator = static_cast<mcpwm_generator_t>(conf->pwm_generator);

		// Configures the channel requirements for the MCPWM
		mcpwm_gpio_init(ptr_tb6612->pwm_unit, pwm_io_signals, conf->pwm_pin);
	}

	void tb6612::channel::set_duty(float duty_cycle) {
		// Changes the MCPWM duty cycle for the channel
		mcpwm_set_duty(ptr_tb6612->pwm_unit, ptr_tb6612->pwm_timer, this->pwm_generator, duty_cycle);
	}

	void tb6612::channel::direction_forward() {
		// Changes the GPIOs level as to change the direction the motor will spin, so it goes forwards
		gpio_set_level(this->in1_pin, !this->inverted);
		gpio_set_level(this->in2_pin, this->inverted);
	}

	void tb6612::channel::direction_backward() {
		// Changes the GPIOs level as to change the direction the motor will spin, so it goes backwards
		gpio_set_level(this->in1_pin, this->inverted);
		gpio_set_level(this->in2_pin, !this->inverted);
	}

	void tb6612::channel::drive(float duty_cycle) {
		// Decides if the motor should go forwards or backwards according to the duty_cycle sign
		(duty_cycle >= 0) ? this->direction_forward() : this->direction_backward();

		// And then updates its duty cycle
		this->set_duty(abs(duty_cycle));
	}

	void tb6612::channel::stop() {
		// Changes both input pins to a low level as to stop the motor (High impedance)
		gpio_set_level(this->in1_pin, 0);
		gpio_set_level(this->in2_pin, 0);
	}

	void tb6612::channel::short_brake() {
		// Changes both input pins to a high level as to brake the motor
		gpio_set_level(this->in1_pin, 1);
		gpio_set_level(this->in2_pin, 1);
	}
	/* channel class methods */
/* tb6612 class methods */
} /* namespace tb6612 */
