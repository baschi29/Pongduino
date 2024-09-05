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

// include for local files
#include "Speaker.h"
#include "Display.h"
#include "DistanceSensor.h"
#include "Controller.h"

// Pin assignment in controller: speaker, leftdstrigger, leftdsecho, rightdstrigger, rightdsecho
Controller controller;


void setup () {

    // Debugging
    Serial.begin(115200);
    while (!Serial);

    Serial.println("Juchuuu");

    controller.startup(13, 3, 2, 5, 4); 

}


void loop () {

    controller.tick();

}
