// Include for task delay
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Include for printf
#include <stdio.h>

// Include for types
#include "driver/mcpwm.h"
#include "driver/gpio.h"

// Library include
#include "TB6612FNG.hpp"

extern "C" void app_main(void)
{
	printf("----Initializing TB6612FNG object----\n");

	tb6612_t tb6612; 							  // Instantiates the tb6612 object
	tb6612.channel_a.set_parent_pointer(&tb6612); // Links the channel objects with
	tb6612.channel_b.set_parent_pointer(&tb6612); //  their corresponding tb6612 object

	printf("----Initializing MCPWM----\n");

	tb6612_pwm_config_t pwm_conf;			// Creates a TB6612FNG MCPWM configurations struct
	pwm_conf.pwm_freq = 1000;				// Set the MCPWM frequency
	pwm_conf.pwm_timer = MCPWM_TIMER_1;		// Set the MCPWM timer
	pwm_conf.pwm_unit = MCPWM_UNIT_0;		// Set the MCPWM unit
	pwm_conf.pwm_operator = 1;				// Set the MCPWM operator PWM1x
	tb6612.tb6612_mcpwm_config(&pwm_conf);	// Configures the MCPWM

	printf("----Initializing GPIOs----\n");

	tb6612.set_standby_pin(GPIO_NUM_25); 		 // Set the Stand-by pin

	tb6612_channel_gpio_config_t conf;			 // Creates a TB6612FNG GPIO configurations struct

	conf.in1_pin = GPIO_NUM_14;					 // Set the IN1 pin
	conf.in2_pin = GPIO_NUM_27;					 // Set the IN2 pin
	conf.inverted = 0;							 // Determines if the pins are swapped
	conf.pwm_generator = 0;						 // Set the MCPWM generator PWM1A
	conf.pwm_pin = GPIO_NUM_12;				 	 // Set the MCPWM pin
	tb6612.channel_a.channel_gpio_config(&conf); // Configures the GPIOs for TB6612FNG's Channel A

	conf.in1_pin = GPIO_NUM_4;
	conf.in2_pin = GPIO_NUM_15;
	conf.inverted = 1;
	conf.pwm_generator = 1;					 	 // Set the MCPWM generator PWM1B
	conf.pwm_pin = GPIO_NUM_16;
	tb6612.channel_b.channel_gpio_config(&conf); // Configures the GPIOs for TB6612FNG's Channel B

	// Variables for the loop
	int duty = 0;
	bool counter_updown = 0;

    while (true) {
    	printf("Duty A: %d\t\tDuty B: %d\n", duty, -duty);

		tb6612.channel_a.drive(duty);
		tb6612.channel_b.drive(-duty);

		// Go back and forth from maximum speed in one direction to the other.
    	// The duty_cycle value is of type float, but for the sake of simplicity, here is used an int.
		if(duty == -100){
			duty++;
			counter_updown = true;
		}
		else if(duty == 100){
			duty--;
			counter_updown = false;
		}
		else counter_updown ? duty++ : duty-- ;

		vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

