# SmartKey — ESP32 Vibration Alert System with Blynk Control

An ESP32-based device that reminds you not to forget your keys. It detects vibration/movement, asks you to confirm you've grabbed your keys via an OLED countdown, and triggers escalating alerts (buzzer + LED) if you don't respond — all controllable and monitorable remotely through the Blynk IoT app.

Built for **IIB20604 - Project Sem 3, Individual Project #2** (UniKL MIIT).

## The Problem

Rushing out the door and forgetting your keys is a small but frustrating everyday problem so it wastes time and can lead to being locked out or missing appointments. SmartKey solves this by sitting next to your keys and actively reminding you to grab them before you leave.

## How It Works

1. Vibration sensor detects movement (e.g. keys being picked up, or the device being touched)
2. A Blynk notification is sent, and the OLED starts a 5-second confirmation countdown
3. If you confirm via the Blynk app or physical button, the system returns to idle
4. If you don't confirm in time, it triggers a repeating alert (buzzer + LED) until acknowledged
5. Two alert modes, switchable via the app:
   - **Gentle Mode** — soft tones, subtle LED flash
   - **Aggressive Mode** — loud pulses, rapid LED flash — for hectic mornings

## Hardware

| Component | Function |
|---|---|
| ESP32 | Main microcontroller, WiFi + Blynk connectivity |
| SW-420 Vibration Sensor | Detects movement/vibration |
| OLED Display (0.96" I2C, SSD1306) | Shows system status, countdown, alerts |
| Active Buzzer | Audio alerts |
| 2x LEDs (Gentle / Aggressive) | Visual alerts |
| Push Button | Physical confirm button |

## Wiring

| Component | Pin |
|---|---|
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| Vibration Sensor (DO) | GPIO 27 |
| Buzzer | GPIO 14 |
| Gentle Mode LED | GPIO 33 (220Ω resistor) |
| Aggressive Mode LED | GPIO 32 (220Ω resistor) |
| Physical Confirm Button | GPIO 26 |

Mode switch and app-side confirm button are controlled via Blynk virtual pins (V1 for mode, V4 for confirm, V3 for alert indicator).

## Tech Stack

- **Hardware:** ESP32, SW-420 vibration sensor, SSD1306 OLED
- **Firmware:** Arduino (C++), Adafruit_GFX / Adafruit_SSD1306 libraries
- **Connectivity:** WiFi, Blynk IoT platform (BlynkSimpleEsp32)

## Operation Flow

Power on → Connect WiFi → Idle (read mode from Blynk)
→ Vibration detected → Blynk notification sent → Confirmation countdown starts
→ Confirmed in time → back to Idle
→ Not confirmed → Alert triggered (OLED + buzzer + LED, based on mode)
→ Confirmed → back to Idle
→ Loop until powered off

## What I'd Improve

- Move credentials (WiFi SSID/password, Blynk auth token) out of the source file and into a separate config file excluded from version control
- Add a snooze feature with configurable countdown duration
- Battery-powered enclosure for full portability instead of USB power
- Making it as a keychain so it can be small to carry on everywhere
