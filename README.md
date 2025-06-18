# Timer-Based-Automatic-Pill-Dispenser

An intelligent, voice-interactive pill dispenser that ensures timely medication using Arduino and a fine-tuned GPT-2 model. This system supports scheduled pill delivery, voice commands, and AI-based health query handling.

Youtube link: https://www.youtube.com/watch?v=bo0DdjGiL6c&ab_channel=BhavyanshiKarela

## 🚀 Features

- **Scheduled Pill Dispensing:** Uses RTC module to release pills at specific times using a servo motor.
- **Voice Interaction:** Accepts spoken commands to query information or interact with the system.
- **AI Integration:** GPT-2 responds to medication-related queries using natural language.
- **Alerts & Display:** IR sensor detects user presence, and buzzer + LCD provides notifications.
- **Simulated Hardware:** Circuit built and tested in Wokwi simulator.

## 🛠️ Tech Stack

| Component      | Description                           |
|----------------|---------------------------------------|
| Arduino UNO    | Microcontroller for hardware control  |
| RTC Module     | Real-time clock for scheduling        |
| Servo Motor    | Controls pill release mechanism       |
| IR Sensor      | Detects user presence                 |
| LCD + Buzzer   | For visual and audible alerts         |
| Python         | Backend for voice & AI logic          |
| GPT-2          | Fine-tuned model for health queries   |
| SpeechRecognition | Voice command processing           |
| Wokwi          | Online simulator for circuit testing  |

## 🧩 Circuit Components

- **Arduino UNO**
- **RTC DS1307**
- **IR Sensor**
- **16x2 I2C LCD**
- **Servo Motor**
- **Buzzer**
- **LEDs (optional)**

## 🧠 AI Model

- Fine-tuned GPT-2 using health-related datasets.
- Backend runs using Python with `transformers`, `torch`, and `SpeechRecognition`.
