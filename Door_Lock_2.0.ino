#include <Keypad.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

const int rs = 13, en = 5, d4 = 4, d5 = 3, d6 = 2, d7 = 1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'},
};
byte rowPins[ROWS] = {12, 11, 10, 9};
byte colPins[COLS] = {8, 7, 6};

Keypad mykeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char initialPIN[4];
char getPIN[4];
char newPIN[4];
char keyPressed = 0;
char checkKey = 0;
int count = 0;
int wrong = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Keypad Lock");
  delay(2000);
  lcd.clear();
  lcd.print("Enter PIN: ");
  lcd.setCursor(0, 1);
  defaultPIN();
}

void defaultPIN() {
  for (int i = 0; i < 4; i++) {
    EEPROM.write(i, i + 49);
  }

  for (int i = 0; i < 4; i++) {
    initialPIN[i] = EEPROM.read(i);
  }
}

void changethePIN() {
  int j = 0;
  char key = 0;
  
  lcd.clear();
  lcd.print("Current PIN: ");
  lcd.setCursor(0, 1);

  while (j < 4) {
    key = mykeypad.getKey();

    if (key && key != '#') {
      newPIN[j] = key;
      lcd.print(key);
      j++;
    }
    key = 0;
  }

  delay(500);

  if (strncmp(newPIN, initialPIN, 4)) {
    lcd.clear();
    lcd.print("Wrong PIN");
    delay(2000);
    lcd.clear();
    lcd.print("Try Again");
    delay(2000);
  }
  else {
    j = 0;
    lcd.clear();
    lcd.print("New PIN: ");
    lcd.setCursor(0, 1);

    while (j < 4) {
      key = mykeypad.getKey();

      if (key && key != '#') {
        initialPIN[j] = key;
        lcd.print("*");
        EEPROM.write(j, key);
        j++;
      }
    }
    delay(500);
    lcd.clear();
    lcd.print("PIN Changed");
    delay(2000);
  }

  lcd.clear();
  lcd.print("Enter PIN: ");
  lcd.setCursor(0, 1);
  keyPressed = 0;
}

void lockdown() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Try Again In");
  lcd.setCursor(4, 1);
  lcd.print("1 minute");
  delay(1000);
  lcd.clear();

  for (int k = 60; k >= 0; k--) {
    lcd.setCursor(7,0);
    lcd.print(k);
    delay(1000);
    lcd.clear();
  }

  lcd.print("Enter PIN: ");
  lcd.setCursor(0, 1);
  count = 0;
  wrong = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  keyPressed = mykeypad.getKey();

  if (keyPressed == '#') {
    changethePIN();
  }

  if (keyPressed && keyPressed != '#') {
    getPIN[count] = keyPressed;
    lcd.print('*');
    count++;
  }

  if (count == 4) {
    delay(2000);

    for (int i = 0; i < 4; i++) {
      initialPIN[i] = EEPROM.read(i);
    }

    if (!(strncmp(getPIN, initialPIN, 4))) {
      wrong = 0;
      lcd.clear();
      lcd.print("Correct PIN");
      delay(1000);
      lcd.clear();
      while (checkKey != '*') {
        lcd.setCursor(4, 0);
        lcd.print("WELCOME!");
        checkKey = mykeypad.getKey();
      }

      if (checkKey == '*') {
        lcd.clear();
        lcd.print("Enter PIN: ");
        lcd.setCursor(0, 1);
        checkKey = 0;
        count = 0;
      }
    }
    else {
      lcd.clear();
      lcd.print("Wrong PIN");
      delay(1000);
      wrong++;
      if (wrong >= 5) {
        lockdown();
      }
      else {
        delay(1000);
        lcd.clear();
        lcd.print("Try Again");
        delay(1000);
        lcd.clear();
        lcd.print("Enter PIN: ");
        lcd.setCursor(0, 1);
        count = 0;
      }
    }
  }
}
