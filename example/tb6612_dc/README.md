# _TB6612_DC Example_

This example shows how to actuate 2 DC motors using the TB6612FNG API.

## How to use example

### Hardware Required

* A development board with ESP32 SoC (e.g., ESP32-DevKitC, ESP-WROVER-KIT, etc.)
* A USB cable for power supply and programming
* A TB6612FNG H-Bridge chip
* 1 or 2 DC motors

Connect the following TB6612FNG pins to the respective GPIOs:

|TB6612 Pin|GPIO|
|:---:|:---:|
|PWMA|GPIO|
|AIN2|GPIO|
|AIN1|GPIO|
|STBY|GPIO|
|BIN1|GPIO|
|BIN2|GPIO|
|PWMB|GPIO|

Connect the following TB6612FNG pins to the motor(s):

|TB6612 Pin|Motor Pin|
|:---:|:---:|
|A1|M1+|
|A2|M1-|
|B2|M2-|
|B1|M2+|

### Configure the project

```
idf.py menuconfig
```

### Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py -p PORT flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)

See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.

## Example Output

Running this example, you will see each motor rotation spin to opposite direction but with the same speed

* Motor 1: Spin CW/CCW / increase then decrease speed
* Motor 2: Spin CCW/CW / increase then decrease speed

you can also see the following output log on the serial monitor:

```
...
Duty A: 49              Duty B: -49
Duty A: 50              Duty B: -50
Duty A: 51              Duty B: -51
...
```