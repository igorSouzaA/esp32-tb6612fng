/*
 * TB6612FNG.hpp
 *
 *  Created on: 11 de jan. de 2022
 *      Author: igor_
 */

#ifndef _TB6612FNG_HPP_
#define _TB6612FNG_HPP_

#include "driver/mcpwm.h"
#include "driver/gpio.h"

/**
 * @brief Configuration parameters of TB6612FNG's MCPWM for tb6612_mcpwm_config function
 *
 * @attention MCPWM doesn't support by default high and low frequencies. Check [this topic](https://www.esp32.com/viewtopic.php?f=14&t=5170)
 */
typedef struct {
	uint32_t pwm_freq = 1000;   /*!< PWM frequency: The frequency at which the PWM will operate	*/
	mcpwm_unit_t pwm_unit;      /*!< MCPWM Unit: One of two available units	*/
	mcpwm_timer_t pwm_timer;    /*!< MCPWM Timer: One of three available timers per unit */
	uint8_t pwm_operator;   	/*!< MCPWM operator: One of three available operators per Unit. The x in PWMxA/B */
} tb6612_pwm_config_t;

/**
 * @brief Configuration parameters of GPIOs for tb_channel_gpio_config function
 */
typedef struct {
	gpio_num_t in1_pin;     	/*!< GPIO pin: First input pin for one channel of the TB6612FNG */
	gpio_num_t in2_pin;     	/*!< GPIO pin: Second input pin for one channel of the TB6612FNG */
	gpio_num_t pwm_pin;     	/*!< GPIO pin: Pin designated to the MCPWM output */
	bool pwm_generator;      	/*!< MCPWM generator:  One of two available generators per Operator. The A or B in PWMxA/B */
	bool inverted = 0;			/*!< Config: Change the inputs' level to correct for swapped pins */
} tb6612_channel_gpio_config_t;

/* ^^^^^^^^^
 * TB6612FNG
 * ^^^^^^^^^ */
namespace TB6612FNG {
class tb6612;
} /* namespace TB6612FNG */

// TB6612FNG type
using tb6612_t = TB6612FNG::tb6612;

namespace TB6612FNG {
// tb6612 class definition
class tb6612 {
private:

	mcpwm_unit_t pwm_unit;		/*!< MCPWM Unit: One of two available units	*/
	mcpwm_timer_t pwm_timer;	/*!< MCPWM Timer: One of three available timers per unit */
	uint8_t pwm_operator; 		/*!< MCPWM operator: One of three available operators per Unit. The x in PWMxA/B */
	gpio_num_t stby_pin;		/*!< GPIO pin: Stand-by pin for */

public:

	/**
	 * @brief  Configure the stand-by GPIO pad
	 * @param  stby_pin   [Stand-by pin]
	 */
	void set_standby_pin(gpio_num_t stby_pin);

	/**
	 * @brief  Configure the MCPWM operator
	 * @param  conf   [MCPWM configurations]
	 */
	void tb6612_mcpwm_config(tb6612_pwm_config_t *conf);

	// channel class definition
	class channel {
	private:

		tb6612* ptr_tb6612;				 /*!< tb6612 pointer: Allows channel to use tb6612 variables */
		mcpwm_generator_t pwm_generator; /*!< MCPWM generator:  One of two available generators per Operator. The A or B in PWMxA/B 	*/
		gpio_num_t in1_pin;				 /*!< GPIO pin: First input pin for given port  */
		gpio_num_t in2_pin;				 /*!< GPIO pin: Second input pin for given port */
		bool inverted;					 /*!< Config: Change the inputs' level to correct for swapped pins */

		/**
		 * @brief  Changes the PWM duty cycle (Motor speed).
		 * @param  duty_cycle   [I2C slave device register]
		 */
		void set_duty(float duty_cycle);

		/**
		 * @brief  Changes the input pins value (Motor spin).
		 */
		void direction_forward();
		void direction_backward();

	public:

		/**
		 * @brief  Links the channel class objects with their parent tb6612 class object through a parent pointer
		 * @param  p   [tb6612 class pointer]
		 */
		void set_parent_pointer(tb6612* p);

		/**
		 * @brief  Configure the GPIO pins needed for the channel
		 * @param  conf   [GPIO configurations]
		 */
		void channel_gpio_config(tb6612_channel_gpio_config_t *conf);

		/**
		 * @brief  Changes the input pins value (Motor spin) and PWM duty cycle (Motor speed).
		 * @param  duty_cycle   [Desired duty cycle and spin according to the magnitude and sign]
		 */
		void drive(float duty_cycle);

		/**
		 * @brief  Sets both input pins to low, stopping the motor.
		 */
		void stop();

		/**
		 * @brief  Sets both input pins to high, braking the motor.
		 */
		void short_brake();

	}; /* class channel */

	channel channel_a, channel_b; /*!< TB6612FNG Channels: A and B */

}; /* class tb6612 */
} /* namespace TB6612FNG */

#endif /* COMPONENTS_TB6612FNG_TB6612FNG_H_ */
