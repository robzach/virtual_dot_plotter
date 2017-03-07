/*
 * Read two potentiometers and send their values as serial data.
 *
 * Built for an interaction prototype I'm working on.
 *
 * Optionally, the sketch can store min and max hardware values
 * for both potentiometers so that their ranges are
 * scaled to run 0-1023 (even if they're imperfect). To use this
 * optional feature, wire a momentary button from pin 2 to GND.
 *
 * Wire up potentiometers to A0 and A1 and a momentary button
 * from pin 2 to GND.
 *
 * Robert Zacharias, rz@rzach.me, 2/20/17
 * released by the author to the public domain
 */


#include <EEPROM.h>

int pushbutton = 2;
int Lpot = A0;
int Rpot = A1;

int Lpothigh, Lpotlow, Rpothigh, Rpotlow;
int EEPROMValueCheck; // will be zero if not using EEPROM storage, nonzero otherwise

void setup() {
  Serial.begin(9600);
  pinMode(pushbutton, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // if there are nonzero EEPROM values stored, load them
  for (int i = 0; i < 7; i++) {
    EEPROMValueCheck += EEPROM.read(i);
  }
  if (EEPROMValueCheck > 0) {
    // load pot range values from EEPROM
    Lpothigh =  EEPROM.read(0) * 4 + EEPROM.read(1);
    Rpothigh =   EEPROM.read(2) * 4 + EEPROM.read(3);
    Lpotlow =  EEPROM.read(4) * 4 + EEPROM.read(5);
    Rpotlow =   EEPROM.read(6) * 4 + EEPROM.read(7);
    Serial.print("Lpothigh, Rpothigh, Lpotlow, Rpotlow = ");
    Serial.print(Lpothigh); Serial.print('\t');
    Serial.print(Rpothigh); Serial.print('\t');
    Serial.print(Lpotlow); Serial.print('\t');
    Serial.println(Rpotlow);
  }
  else Serial.println("nonzero EEPROM values not found");
}

void loop() {

  int Lval, Rval;

  // push button to record both high values, wait 5 secs, record both low values to EEPROM
  if (digitalRead(pushbutton) == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
    recordNewValues();
    digitalWrite(LED_BUILTIN, LOW);
  }

  // this block will run if you're using stored EEPROM values
  if (EEPROMValueCheck) {
    Lval = map(analogRead(Lpot), Lpotlow, Lpothigh, 0, 1023);
    delay(2); // the ADC wants a bit of time to settle, apparently
    Rval = map(analogRead(Rpot), Rpotlow, Rpothigh, 0, 1023);
    delay(2);
  }

  // this block will run if you only have zeros stored in EEPROM
  else {
    Lval = analogRead(Lpot);
    delay(2); // the ADC wants a bit of time to settle, apparently
    Rval = analogRead(Rpot);
    delay(2);
  }

  Serial.print(Lval); Serial.print(","); Serial.println(Rval);

  delay(5);

}

void recordNewValues() {

  Serial.println("recordNewValues running. Before this procedure, start with both knobs fully \"up,\"\nand as soon as it's begun, you have five seconds to turn them fully to \"down.\" to calibrate properly.");

  int potValues [8];
  // immediately record high values for both pots to array
  potValues[0] = analogRead(Lpot) / 4; // quotient
  potValues[1] = analogRead(Lpot) % 4; // remainder
  potValues[2] = analogRead(Rpot) / 4;
  potValues[3] = analogRead(Rpot) % 4;

  delay(5000); // wait five seconds for people to move pots to low position and record their values

  potValues[4] = analogRead(Lpot) / 4;
  potValues[5] = analogRead(Lpot) % 4;
  potValues[6] = analogRead(Rpot) / 4;
  potValues[7] = analogRead(Rpot) % 4;

  Serial.print("EEPROM values stored: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(potValues[i]); Serial.print(' ');
    EEPROM.write(i, potValues[i]);
  }
  Serial.println();

  // immediately load new pot range values from EEPROM
  Lpothigh =  EEPROM.read(0) * 4 + EEPROM.read(1);
  Rpothigh =  EEPROM.read(2) * 4 + EEPROM.read(3);
  Lpotlow =   EEPROM.read(4) * 4 + EEPROM.read(5);
  Rpotlow =   EEPROM.read(6) * 4 + EEPROM.read(7);

  Serial.print("Lpothigh, Rpothigh, Lpotlow, Rpotlow = ");
  Serial.print(Lpothigh); Serial.print('\t');
  Serial.print(Rpothigh); Serial.print('\t');
  Serial.print(Lpotlow); Serial.print('\t');
  Serial.println(Rpotlow);

  Serial.println("recordNewValues complete\n\n\n");
}

