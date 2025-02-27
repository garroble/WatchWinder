# WatchWinder
## About the project
3D Printed Watch Winder controlled through Arduino IoT Cloud.

A watch winder is a machine that rotates an automatic mechanical watch to keep it on time and date.

Depending on the watch, it needs to rotate several hundreds times a day. Some of them need to rotate clockwise and some counterclockwise. Others can rotate in either direction.

With this watch winder, the watch is always on time, keeping it running and preserving it from breakdowns during prolonged shutdowns.

In the project you can find everything needed to build your own watch winder.

## Mechanics
On the mechanics folder you will find the 3D model designed in FreeCAD so you can make any changes needed for your watch.
The watch winder has been designed considering watch bracelets up to 26mm width.
The FreeCAD model also includes an assembly to check how everything fits together. (Under development)

You can also find the STL files en m3f files to directly import the parts in 3D printing slicing programs.
The models have been proved on Bambu Lab A1 mini printer successfully.

The 3D model is also published on:
* Maker World (https://makerworld.com/es/models/1157015-watch-winder#profileId-1162305)

Other parts that need to be purchased to get the full mechanical assembly are:

mBOM (Mechanical Parts):

| ID | Part name     | Part number             | Quantity |
|----|---------------|-------------------------|----------|
| S1 | Spring        | 0.5x5x25                | 2        |
| M1 | Stepper Motor | 28BYJ-48                | 1        |
| I1 | Insert M3     | M3x0.5 5.74mm length    | 2        |
| I2 | Insert M4     | M4x0.7 8.15mm length    | 2        |
| S2 | Screw         | M3x10                   | 2        |
| S3 | Screw         | M4x20                   | 2        |
| B1 | Bearing       | 608zz                   | 2        |
| C1 | Cable F-F     | Dupont Cable FF         | 10       |
| DC | Barrel Plug   | DC Barrel Plug to cable | 1        |


eBOM (Electronic Parts):

| ID   | Part name         | Part number                                 | Quantity |
|------|-------------------|---------------------------------------------|----------|
| PCB1 | Arduino           | Nano RP2040                                 | 1        |
| PCB2 | Motro Driver      | ULN2003                                     | 1        |
| PCB3 | Voltage Step Down | MP1584EN                                    | 1        |
| DC   | DC Wall Adapater  | 12V 500mA adapter compatible with DC barrel | 1        |

### Assembly
1. Place the M3 inserts on the Body with a soldering iron for the motor.
2. Place the M4 inserts on the Body with a soldering iron for the bearrings.
3. Place the 608zz and bearring stopper on the hole and fix it with the M4x20 screw.
4. Place the drum inside of the body.
5. Place the motor on the rear part of the body fitting the shaft inside of the drum.
6. Fix the motor to the body with M3x10 screws.
7. Place all PCB boards in place.
8. Cut 4 Dupont Cable in one end, two for DC power supply and two for DC ground.
9. Connect the DC power supply cables to barrel + signal.
10. Connect the DC ground cables to barrel - signal.
11. Ensure the polarity by connecting the DC Wall adapater and measuring the voltage wiht a multimeter.
12. Connect one DC power supply cable and one DC ground cable to Arduino Nano Vin and GND signals.
13. Connect the other DC power supply cable and the other DC ground cable to MP1584EN board in V+ and V-.
14. Use two dupont cables to connect the MP1584EN output Vout+ and Vout- to ULN2003 power input Vin and GND.
15. Connect Arduino Nano pins 7, 8, 9, 10 to IN1, IN2, IN3, IN4 on ULN2003 board.
16. Connect the Stepper Motor to the ULN2003 board.
17. Install front and rear covers on the body.
18. Place the springs on Watch Press front shafts.
19. Insert Watch Press Front shafts on Watch Press Rear.
20. Place the wath on Wath Press and insert it on the Watch Winder drum.

<img src="https://github.com/garroble/WatchWinder/blob/main/images/electronic_connections.png" width=200>


## Firmware
The firmware has been developed on Arduino IDE 2.3.4 (https://www.arduino.cc/en/software) and Arduino Cloud (https://app.arduino.cc/) 

On Arduino Cloud:
1. Include the Arduino Nano board as a new Device.
2. Under Things, include the following variables to the Device:
   1. cl_winder_status as Boolean, Read Only and update on change.
   2. cl_winder_turns as Counter, Read Only and update on change.
   3. ctrl_start_time as CloudSchedule, Read & Write and update on change.
   4. ctrl_switch as Boolean, Read & Write and update on change.
3. This will generate the basic Sketch with all the variables and callbacks ready to be used. The thingProperties.h shall be very similar to the one published here.
4. Follow the published code to get your Watch Winder ready and running.
5. Create a Dashboard in Arduino Cloud to control and get the status of your winder:
   1. Switch linked to ctrl_switch to turn it on/off any time.
   2. LED linked to cl_winder_status to check if it is running.
   3. Scheduler linked to ctrl_start_time to schedule the start time, duration and repeat every.
   4. Graph linked to cl_winder_turns to keep track of the number of rotations.

### Firmware tuning
Based on your watch, you may need to change the number of rotations a day needed.
You can set it on the WatchWinder.ino file:
> #define WATCH_TURNS_PER_DAY 750

The program continuously rotates clockwise as it is. If you need to make it rotate counterclockwise, just remove the minus signal on:
> int motor_target = -MOTOR_MAX_STEPS;


## License
Licensed under GNU General Public License V3.0.

## Acknowledgements
Thanks to the following sites and their community for providing the 3D models for all the purchased parts.
* 3D Content Central. https://www.3dcontentcentral.com/
* GrabCAD. https://grabcad.com/

Thanks for the amazing AccelStepper library for Arduino. It is extremely easy to make it work and it has some really cool features.
* AccelStepper: https://www.airspayce.com/mikem/arduino/AccelStepper/

