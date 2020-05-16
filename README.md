# Zumo 32U4 SBUS Receiver

Using SBUS to communicate with the Zumo board. 
You will also need a signal inverter between SBUS receiver and Zumo board [Build your own inverter](https://www.ernstc.dk/arduino/sbus.html).

## Why use SBUS

When using pwm we are restricted to use more input pins with SBUS we only need one.

## Wiring

<a href="https://user-images.githubusercontent.com/16755871/82107092-64dcd680-9725-11ea-99ec-610cd2e3c9df.png
" target="_blank"><img src="https://user-images.githubusercontent.com/16755871/82107092-64dcd680-9725-11ea-99ec-610cd2e3c9df.png" 
alt="IMAGE ALT TEXT HERE" width="278" height="190" /></a>

## Channels 

| Stick    | Channel | Constant         | Description                                                     |
| -------- | ------- | ---------------- | --------------------------------------------------------------- |
| Throttle | 2       | THROTTLE_CHANNEL | Gas & break, Mode 2 Stick on the left side move UP or DOWN      |
| Turning  | 0       | TURNING_CHANNEL  | Left & Right, Mode 2 stick on the right side move LEFT or RIGHT |
| Reverse  | 8       | REVERSE_CHANNEL  | Make zumo go in reverse (Switch B on my Radiolink)              |

## Status leds

On the Zumo 32U4 board you got three leds yellow green red.

| Led    | Mode      | Description                    |
| ------ | --------- | ------------------------------ |
| Red    | failSafe  | No connection to transmitter   |
| Yellow | lostFrame | SBUS lost a frame              |
| Green  | Ok        | We have connection to receiver |


## Debug

If you wich to se the exact mesurements from SBUS channels in terminal you have to hook it up to USB and connect with putty to it.
This mode should not be use more than debug purpose, you will miss packages from SBUS and that is made intentially because i wish to delay the output so merasurements can be read from console.

* First you need to uncomment in main.cpp

    //#define MY_DEBUG 

* Upload the scetch

* Open putty with these settings
   
    Select Serial
    Select your COM port
    Select Speed: 115200
    Window->Uncheck 'Push erased text to scrollback'
