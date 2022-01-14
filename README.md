# TB6612FNG driver
 A library for TB6612FNG chips.
 It is written in C++ and designed for working with **[ESP32]** microcontroller _[esp-idf]_ framework.
 C++ library implementation of the TB6612FNG IC to be used on the ESP-IDF environment with an ESP-32 micro-controller.
 
 This library currently only supports DC motors connected to the TB6612FNG chip.

## Installation

 To add this component to your project, you can either:
 
### Clone to your project
 
 Clone it right into your project components directory with the following command.
 
 ```
 git clone https://github.com/igorSouzaA/esp32-tb6612fng.git
 ```
 
 And add the following line to your project's 'main' CmakeLists.txt:
 
 ```CMake
 set(COMPONENT_REQUIRES TB6612FNG)
 ```
 
 This way you can easily update the library with `git pull` whenever a update is available.
 
### Clone outside of your project
 
 Clone it anywhere you want in your system with the following command.
 
 ```
 git clone https://github.com/igorSouzaA/esp32-tb6612fng.git
 ```
 
 And include the following line to your project's top-level CMakeLists.txt:
 
 ```CMake
 set(EXTRA_COMPONENT_DIRS "Insert/Your/Full/Path/Here")
 ```
 
 Remember to change the path to where you cloned this repository.
 
 And also add the following line to your project's 'main' CmakeLists.txt:
 
 ```CMake
 set(COMPONENT_REQUIRES TB6612FNG)
 ```
 
 This way, instead of updating each instance of this repository amongst your projects, you can simply update one main instance that will trickle down to every project that uses it.
 
## Usage
 
 First of all, make sure TB6612FNG driver is a component in your project.
 
 Now, in your source code, include the TB6612FNG main header `TB6612FNG.hpp`
 
 ```C++
 #include "TB6612FNG.hpp"
 ```
 
 Configure the GPIOs that will be used by the TB6612FNG chip.
 
 ```C++
 TB6612FNG::set_standby_pin(GPIO_NUM_25); // Set the Stand-by pin

 tb6612_gpio_config_t conf;		  // Creates a TB6612FNG GPIO configurations struct

 conf.in1_pin = GPIO_NUM_14;		  // Set the IN1 pin
 conf.in2_pin = GPIO_NUM_27;		  // Set the IN2 pin
 conf.inverted = 0;			  // Determines if the pins are swapped
 conf.pwm_operator = 1;			  // Set the MCPWM operator PWM1x
 conf.pwm_generator = 0;		  // Set the MCPWM generator PWM1A
 conf.pwm_pin = GPIO_NUM_12;		  // Set the MCPWM pin
 tb_channel_a.tb6612_gpio_config(&conf);  // Configures the GPIOs for TB6612FNG's Channel A

 conf.in1_pin = GPIO_NUM_4;
 conf.in2_pin = GPIO_NUM_15;
 conf.inverted = 1;
 conf.pwm_operator = 1;			  // Both Channels must have the same operator
 conf.pwm_generator = 1;		  // Set the MCPWM generator PWM1B
 conf.pwm_pin = GPIO_NUM_16;
 tb_channel_b.tb6612_gpio_config(&conf);  // Configures the GPIOs for TB6612FNG's Channel B
 ```
 
 Configure the MCPWM that will be used by the TB6612FNG chip.
 
 ```C++
 tb6612_pwm_config_t pwm_conf;			// Creates a TB6612FNG MCPWM configurations struct
 pwm_conf.pwm_freq = 1000;			// Set the MCPWM frequency
 pwm_conf.pwm_timer = MCPWM_TIMER_1;		// Set the MCPWM timer
 pwm_conf.pwm_unit = MCPWM_UNIT_0;		// Set the MCPWM unit
 TB6612FNG::tb6612_mcpwm_config(&pwm_conf);	// Configures the MCPWM
 ```
 
 You are now ready to go.
 
 ```C++
 float duty = 53.2;		// Creates a duty cycle variable
		
 tb_channel_a.drive(duty);	// Drives the motor connected to the TB6612FNG's Channel A forwards
 tb_channel_b.drive(-duty);	// Drives the motor connected to the TB6612FNG's Channel A backwards
 ```
 
 The API provides a few other methods to operate the motors. See the source code.
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 