# Mixed Protocol Environmental Monitor

A bare-metal STM32 firmware application that monitors environmental parameters using a BMP180 sensor and displays the telemetry on a 1.3-inch SH1106 OLED display. This project demonstrates handling multiple communication protocols (I2C and SPI) on a single microcontroller.

## 🛠️ Hardware Components
* **Microcontroller:** STM32F401RE (Nucleo-F401RE Development Board)
* **Sensor:** BMP180 (Barometric Pressure & Temperature Sensor)
* **Display:** 1.3" SH1106 OLED (128x64 pixels)

## 📡 Protocol & Peripherals Implementation
* **I2C1 (Standard Mode @ 100kHz):** Interfaced with the BMP180 sensor using standard 7-bit addressing to sample raw temperature, pressure, and calculate altitude.
* **SPI1 (Master Mode, Tx-Only Simplex):** Interfaced with the SH1106 OLED display via a high-speed SPI bus with custom GPIO controls for Reset (`OLED_RES`) and Data/Command (`OLED_DC`) selection to maximize screen refresh rates.
* **HAL Library Integration:** Built upon the STM32 HAL framework leveraging automated code generation via `Mixed_Protocol_Environmental_Monitor.ioc` for clock trees and basic register mapping.

## 📺 Project Demo
<video src="media/demo.mp4" width="100%" controls autoplay loop muted></video>

## 📂 Project Structure
* `Core/Src/main.c`: Core application logic, peripheral initializations, and the main telemetry reading loop.
* `Drivers/`: STM32F4xx HAL and CMSIS peripheral drivers.
* `Schematic_Mixed_Protocol_Environmental_Monitor.png`: Visual wiring diagram of the circuit layout.