<h1 align="center">
Pongduino
</h1>

Yet another Arduino Pong game using distance sensors as controllers. Build and programmed as part of an university class in the summer semester 2024.

## Hardware

- Arduino Uno
- AZ-Delivery 1,3´´ I2C OLED Display
- HC-SR04 Ultrasonic Sensor Module
- Piezo Speaker

## Libraries

- [U8g2lib](https://github.com/olikraus/u8g2)
- the file Pitches.h, originally written by Brett Hagman, to supply a note table

## Wiring

Below the pin layout and wiring used by the author is shown. It is possible to specify the pins in use via the Controller constructor:

```
Controller(int fieldXDim, int fieldYDim, int paddleLength, int paddleWidth, int ballSize, int speakerPin, int leftDSTriggerPin, int leftDSEchoPin, int rightDSTriggerPin, int rightDSEchoPin);
```

![Schaltung_Steckplatine](https://github.com/user-attachments/assets/c2991407-2c82-42f7-a19e-bc8822909a67)

## Class diagram

![classController__coll__graph](https://github.com/user-attachments/assets/68b7c27c-1871-41ab-8958-f2886254c901)

## License

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/
