# Joystick Controlled Fan

## Overview
This project uses an Arduino UNO, a joystick module, and an L293DNE motor driver to control a DC motor and a stepper motor. The joystick module allows for manual control of the motors, which can be used in applications such as a fan in this case. Moving the joystick up and down changes the speed of the fan (4 levels including OFF) while moving the fan left and right changes the direction. The OFF mode will be indicated by the red LED. Pressing the switch on the joystick toggles between an automatic (fan oscillates on its own) and manual mode.

## Components
All external parts that are used are provided with their appropirate part number (from Digikey). 
- Joystick Module (1568-1526-ND)
- L293DNE Motor Driver (296-9518-5-ND)
- DC Motor (1568-1660-ND)
- Stepper Motor (1597-1699-ND)
- ULN2003/X113647 Stepper Motor Driver (1597-1699-ND)
- Breadboard Power Supply Module (377-2647-ND)

## Schematic Rev1 (Scheme-It)
![schematic-joystickfan](https://github.com/user-attachments/assets/6b394b59-8135-4fd5-b1d7-b04940891dd0)

Made on Scheme-it (Digikey's Schematic Tool). An external link to the schematic can be found [here](https://www.digikey.ca/schemeit/project/joystick-b921c0219b6d4c529892d8f982140288). 

## Schematic Rev2 (KiCad)
![image](https://github.com/user-attachments/assets/d4a5b6f6-f8ec-41bf-b6ec-4e239f14f40a)

A full file of all relevant attachments is provided under "KiCad Documents" within the repository. 

## Connections
Power supply module should be attached to the breadboard, do not power the motors using the Arduino board.

### Joystick Module to UNO
- VCC -> 5V
- GND -> GND
- VRx -> A0
- VRy -> A1
- SW -> D2

### Stepper Motor Driver to UNO
Attach the Stepper Motor Coil to the Stepper Motor Driver. View aforementioned schematic for orientation.
- IN1 -> D11
- IN2 -> D10
- IN3 -> D9
- IN4 -> D8
- GND -> GND

### DC Motor to L293DNE
- Connect the DC motor (+) to 1Y (out 1)
- Connect the DC motor (-) to 2Y (out 2)
- 1 (Enable 1) -> D5
- 1A (In 1) -> D4
- GND -> GND
- 2A (In 2) - > D3 PWM
- VCC2 (Motor) -> 5V

### LED to Arduino UNO
- Anode (long leg) -> 220Ω Resistor -> D7 (Power from Arduino) 
- Cathode (short leg) -> GND

## Demo Video
[![Joystick-Fan Demo Video](https://img.youtube.com/vi/cgZLRQtYCaY/0.jpg)](https://www.youtube.com/watch?v=cgZLRQtYCaY)

A physical link is provided [here](https://youtu.be/cgZLRQtYCaY)

## Next Steps
- Creating a proper 3D printed design for housing the motors and fan
- PCB Design using KiCAD
