/*
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
*/

#include "pitches.h"

// global variable definitions
const int SpeakerPin = 13;

void setup () {

    // play startup jingle
    playStartSound();

}


void loop () {

}


void playStartSound() {

    tone(SpeakerPin, NOTE_C4);
    delay(100);
    noTone(SpeakerPin);
    delay(100);
    tone(SpeakerPin, NOTE_C4);
    delay(100);
    tone(SpeakerPin, NOTE_E4);
    delay(200);
    tone(SpeakerPin, NOTE_G4);
    delay(200);
    noTone(SpeakerPin);

}


void playStopSound() {

    tone(SpeakerPin, NOTE_G4);
    delay(100);
    noTone(SpeakerPin);
    delay(100);
    tone(SpeakerPin, NOTE_G4);
    delay(100);
    tone(SpeakerPin, NOTE_E4);
    delay(200);
    tone(SpeakerPin, NOTE_C4);
    delay(200);
    noTone(SpeakerPin);

}