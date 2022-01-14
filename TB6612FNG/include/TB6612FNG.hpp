/*
 * TB6612FNG.h
 *
 *  Created on: 11 de jan. de 2022
 *      Author: igor_
 */

// TODO Create the tb6612fng class which encompasses the tb_channel class as to allow for multiple TB6612FNG ICs
//Classe tb6612fng que possui a classe tb_channel caso houver mais de 1 CI

#ifndef _TB6612FNG_HPP_
#define _TB6612FNG_HPP_

#include "driver/mcpwm.h"

/**
 * @brief Configuration parameters of TB6612FNG's MCPWM for tb6612_mcpwm_config function
 */
typedef struct {
	uint32_t pwm_freq = 1000;   /*!< PWM frequency: MCPWM doesn't support by default high frequencies. Check [this topic](https://www.esp32.com/viewtopic.php?f=14&t=5170) */
	mcpwm_unit_t pwm_unit;      /*!< MCPWM Unit: One of two available units							  */
	mcpwm_timer_t pwm_timer;    /*!< MCPWM Timer: One of three available timers per unit 			  */
} tb6612_pwm_config_t;

/**
 * @brief Configuration parameters of GPIOs for tb6612_gpio_config function
 */
typedef struct { // TODO operator não ser da classe tb6612 pois deve ser o mesmo para ambos canais
	gpio_num_t in1_pin;     	/*!< GPIO pin: First input pin for one channel of the TB6612FNG 							*/
	gpio_num_t in2_pin;     	/*!< GPIO pin: Second input pin for one channel of the TB6612FNG 							*/
	gpio_num_t pwm_pin;     	/*!< GPIO pin: Pin designated to the MCPWM output 											*/
	uint8_t pwm_operator;   	/*!< MCPWM operator: One of three available operators per Unit. The x in PWMxA/B 			*/
	bool pwm_generator;      	/*!< MCPWM generator:  One of two available generators per Operator. The A or B in PWMxA/B 	*/
	bool inverted = 0;			/*!< Config: Change the inputs' level to correct for swapped pins 							*/
} tb6612_gpio_config_t;

/* ^^^^^^^^^
 * TB6612FNG
 * ^^^^^^^^^ */
namespace TB6612FNG {
class tb6612;
} /* namespace TB6612FNG */

// TB6612FNG type
using tb6612_t = TB6612FNG::tb6612;

// Default Objects
extern tb6612_t tb_channel_a;		/*!< TB6612FNG Channel: A */
extern tb6612_t tb_channel_b;		/*!< TB6612FNG Channel: B */

namespace TB6612FNG {
extern mcpwm_unit_t pwm_unit;		/*!< MCPWM Unit: One of two available units				 */
extern mcpwm_timer_t pwm_timer;		/*!< MCPWM Timer: One of three available timers per unit */

extern gpio_num_t stby_pin;			/*!< GPIO pin: Stand-by pin for */

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

// tb6612 class definition
class tb6612 {
private:
	/**
	 * @brief  Changes the PWM duty cycle (Motor speed).
	 * @param  duty_cycle   [I2C slave device register]
	 */
	void set_duty(float duty_cycle);

	/**@{*/
	/**
	 * @brief  Changes the input pins value (Motor spin).
	 */
	void direction_forward();
	void direction_backward();
	/**@}*/

public:
	mcpwm_generator_t pwm_generator;	/*!< MCPWM generator:  One of two available generators per Operator. The A or B in PWMxA/B 	*/

	gpio_num_t in1_pin;			/*!< GPIO pin: First input pin for given port  */
	gpio_num_t in2_pin;			/*!< GPIO pin: Second input pin for given port */

	bool inverted;				/*!< Config: Change the inputs' level to correct for swapped pins */

	/**
	 * @brief  Configure the GPIO pins needed for the channel
	 * @param  conf   [GPIO configurations]
	 */
	void tb6612_gpio_config(tb6612_gpio_config_t *conf);

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

};

} /* namespace tb6612 */

#endif /* COMPONENTS_TB6612FNG_TB6612FNG_H_ */
