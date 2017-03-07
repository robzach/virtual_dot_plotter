/*
 * Read two potentiometers and send their values as serial data.
 * For an interaction prototype.
 *
 * Potentiometers on A0 and A1.
 *
 */


#include <EEPROM.h>

int pushbutton = 5;
int Lpot = A0;
int Rpot = A1;

int Lpothigh, Lpotlow, Rpothigh, Rpotlow;

void setup() {
  Serial.begin(9600);
  pinMode(pushbutton, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // if there are nonzero EEPROM values stored, load them
  int EEPROMValueCheck;
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

  // push button to record both high values, wait 5 secs, record both low values to EEPROM
  if (digitalRead(pushbutton)) {
    digitalWrite(LED_BUILTIN, HIGH);
    recordNewValues();
    digitalWrite(LED_BUILTIN, LOW);
  }

  ///////// uncomment these lines to use stored EEPROM values in case your potentiometers
  ///////// don't go all the way from 0-1023, or are wired backwards, etc.
  //  int Lval = map(analogRead(Lpot), Lpotlow, Lpothigh, 0, 1023);
  //  int Rval = map(analogRead(Rpot), Rpotlow, Rpothigh, 0, 1023);

  int Lval = analogRead(Lpot);
  delay(2);
  int Rval = analogRead(Rpot);
  delay(2);

  //  Serial.print(Lval); Serial.print(","); Serial.println(Rval);

  delay(5);


}

void recordNewValues() {

  Serial.println("recordNewValues running");

  int potValues [8];
  //record high values for both pots to array
  potValues[0] = analogRead(Lpot) / 4; // quotient
  potValues[1] = analogRead(Lpot) % 4; // remainder
  potValues[2] = analogRead(Rpot) / 4;
  potValues[3] = analogRead(Rpot) % 4;

  delay(5000); // wait two seconds for people to move pots to low position

  potValues[4] = analogRead(Lpot) / 4;
  potValues[5] = analogRead(Lpot) % 4;
  potValues[6] = analogRead(Rpot) / 4;
  potValues[7] = analogRead(Rpot) % 4;

  Serial.print("EEPROM values: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(potValues[i]); Serial.print(' ');
    EEPROM.write(i, potValues[i]);
  }
  Serial.println();

  // immediately load new pot range values from EEPROM
  Lpothigh =  EEPROM.read(0) * 4 + EEPROM.read(1);
  Rpothigh =   EEPROM.read(2) * 4 + EEPROM.read(3);
  Lpotlow =  EEPROM.read(4) * 4 + EEPROM.read(5);
  Rpotlow =   EEPROM.read(6) * 4 + EEPROM.read(7);

  Serial.println("recordNewValues complete\n\n\n");
}

