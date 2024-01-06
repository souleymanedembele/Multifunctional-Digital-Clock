# Multifunctional-Digital-Clock
This repository contains the design and implementation details of a multifunctional digital clock, utilizing the TM4C microcontroller as its core processing unit. This project aims to blend traditional timekeeping with modern technological enhancements.

# TimeSync Project Overview

The goal of the TimeSync project was to design and implement a multifunctional digital clock using the TM4C123GH6PM microcontroller. This clock includes a timer, display, temperature sensor, and an alarm feature that triggers an mp3 module using UART. Users interact through a modern OLED display and 3 push buttons for functionalities such as switching between the clock and temperature display.

![TimeSync Project Demo](./Docs/working_demo.gif)  
*Figure: Represents a demo of the working project.*

### Full Demo Videos
- [TimeSync Demo Video 1](https://uw.hosted.panopto.com/Panopto/Pages/Viewer.aspx?id=b79e97a9-7308-4104-b7cb-b0db0072c394)
- [TimeSync Demo Video 2](https://uw.hosted.panopto.com/Panopto/Pages/Viewer.aspx?id=3ecc4f04-095f-4224-bf3a-b0db006e97bb)

## Project Objectives
- Develop a precise timer for accurate timekeeping.
- Integrate a temperature sensor for ambient temperature readings.
- Implement an intuitive user interface using an OLED display through I2C.
- Embed an alarm function that activates an mp3 module via UART.

## Components and Techniques
- Tiva EK-TM4C123GXL LaunchPad.
- Temperature sensor with ADC conversion.
- I2C OLED display.
- UART module for mp3 alarm.
- Multiple interrupts and 1 timer and a real-time clock for timekeeping and system management.

## Progress and Challenges
- Successfully designed and implemented the timer and display.
- Precise timekeeping using a hardware timer and multiple interrupts.
- Overcame UART module challenges by setting the clock rate at 50 MHz using PLL.
- Realized the integration of the temperature sensor with ADC conversion.
- Implemented the UART module for the mp3 alarm feature.
- Addressed ADC conversion accuracy for the temperature sensor through calibration tests.
- Anticipated complexity in integrating the alarm feature with the timer.

## Conclusion
The TimeSync project is a comprehensive digital clock implementation that demonstrates advanced skills in system design, embedded programming, and hardware integration. It successfully achieves its objectives, providing an accurate and functional timekeeping device with the added benefits of temperature reading and alarm notifications.


