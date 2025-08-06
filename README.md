# Incremental Rotary Encoder Door System

This project simulates an automatic door system using an Arduino UNO, incremental rotary encoder, PIR motion sensor, OLED display, and motor driver. It detects motion, displays door status, and rotates a motor to simulate door opening and closing.

## Features

- Detects motion using PIR sensor
- Rotates motor based on encoder feedback
- Displays messages on OLED (Opening, Closing, etc.)
- Uses state machine (STOPPED, OPENING, WAIT, CLOSING)
- LEDs for door status indicators (green for open, yellow for close)
- Blinking heartbeat LED for system activity

## Hardware Used

- Arduino UNO
- Incremental rotary encoder
- PIR motion sensor
- Adafruit SSD1306 OLED display
- L298N or compatible motor driver
- 2 LEDs (green and yellow)
- DC motor
- Jumper wires, breadboard
- External battery pack (for motor)

## Pin Connections

| Component       | Pin         |
|------------------|-------------|
| PIR Sensor       | D2          |
| Encoder A/B      | D3 / D4     |
| Motor Enable     | D9          |
| Motor Input 1/2  | D8 / D7     |
| Green LED        | D11         |
| Yellow LED       | D13         |
| Heartbeat LED    | D12         |
| OLED SDA / SCL   | A4 / A5     |

## How It Works

1. **Idle**: System waits for motion.
2. **Motion Detected**: Motor rotates counterclockwise to "open" the door.
3. **Wait State**: Door stays open for 5 seconds.
4. **Auto Close**: Motor rotates clockwise to close door.
5. **Status Displayed**: OLED shows status throughout.

## Libraries Used

- `Encoder.h`
- `Adafruit_GFX.h`
- `Adafruit_SSD1306.h`
- `Wire.h`

## Author

DiaaEldin Elabsy  
City Tech, CUNY

