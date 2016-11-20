#include <Wire.h>
#include "Adafruit_Trellis.h"
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);
#define numKeys 16
//GLOBAL VARIABLES
int midiArray[10][12];
int buttonMap[16];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  trellis.begin(0x70);
  // light up all the LEDs in order
  for (uint8_t i = 0; i < numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();
    delay(50);
  }
  // then turn them off
  for (uint8_t i = 0; i < numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();
    delay(50);
  }
  keyBuilder(7); //just in C for now...
  //Set up midiArray
  for (int i = 0; i < 10 ; i++) {
    for (int j = 0; j < 12; j++) {
      midiArray[i][j] = 12 * i + j;
    }
  }
}
void loop() {
  delay(30); // 30ms delay is required, dont remove me!
  // If a button was just pressed or released...
  if (trellis.readSwitches()) {
    // go through every button
    for (uint8_t i = 0; i < numKeys; i++) {
      // if it was pressed, turn it on
      if (trellis.justPressed(i)) {
        Serial.print("v"); Serial.println(i);
        trellis.setLED(i);
        usbMIDI.sendNoteOn(buttonMap[i], 100, 1);
        //        int note = 0;
        //        if (i < 8) {
        //          note = midiArray[5][i];
        //        }
        //        else {
        //          note = midiArray[6][8 - i];
        //        }
        //        usbMIDI.sendNoteOn(note, 100, 1);
      }
      // if it was released, turn it off
      if (trellis.justReleased(i)) {
        Serial.print("^"); Serial.println(i);
        trellis.clrLED(i);
        //        int note = 0;
        //        if (i < 8) {
        //          note = midiArray[5][i];
        //        }
        //        else {
        //          note = midiArray[6][8 - i];
        //        }
        //        usbMIDI.sendNoteOff(note, 100, 1);
        usbMIDI.sendNoteOff(buttonMap[i], 100, 1);
      }
    }
    // tell the trellis to set the LEDs we requested
    trellis.writeDisplay();
  }
}
void keyBuilder(int key) {
  int majorKey[7] = {0, 2, 4, 5, 7, 9, 11};
  int octave = 5 * 12;
  for (int i = 0; i < 16; i++) {
    int note = i / 7;
    note = note * 12 + majorKey[i % 7] + key + octave;
    buttonMap[i] = note;
  }
}

