# RobotCar2

I was gifted an Elegoo Smart robot car v4 and started to experiment by writing some new software to provide a better experience than the one provided out of the box.
This code is for the Arduino part of the project.
It works with these 2 other projects :

1. The "client" part : [https://github.com/Thibdumont/RobotCarUI](https://github.com/Thibdumont/RobotCarUI)
2. The ESP32 part : [https://github.com/Thibdumont/espCam2](https://github.com/Thibdumont/espCam2)

- [RobotCar2](#robotcar2)
  - [Requirements](#requirements)
  - [Key features](#key-features)
  - [Installation instructions](#installation-instructions)
    - [Configuration](#configuration)
    - [Uploading](#uploading)
  - [Work in progress](#work-in-progress)

## Requirements

- You need to have a `Elegoo Smart robot car v4` kit or build some equivalent.

## Key features

- Handle communication with ESP32 through JSON based messages
- Can switch from LAN/SoftAP mode by pressing the Arduino shield button
- Synchronize state with ESP32 when booting/rebooting
- Allow interaction with any part of the build (no usage for MPU and IR receiver yet)
- See the client part project for the full list of user features : [https://github.com/Thibdumont/RobotCarUI](https://github.com/Thibdumont/RobotCarUI)

## Installation instructions

### Configuration

If you have a `Elegoo Smart robot car v4` kit, you shouldn't have anything to adapt. I advise you to first test "as is" before changing anything, unless you don't have the required kit and want to adapt the program to your setup of course.

### Uploading

1. Install `VSCode`
2. Install `PlatformIO IDE` extension
3. Clone this repository and go to the `RobotCar2` folder
4. Plug an USB cable to your arduino and set the arduino switch to `upload` mode
5. Click the `upload` button of PlatformIO IDE extension to start the upload
6. Unplug the USB cable and switch back to `com` mode to make the communication between ESP and Arduino possible

## Work in progress

This project is still work in progress. Any suggestion, advice, pull request is welcome.
