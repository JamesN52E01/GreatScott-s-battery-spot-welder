#include <Wire.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int timems;
bool wait = 0;

void setup() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TIMSK1 = 0;
  TIMSK1 |= (1 << OCIE1A);

  pinMode(A0, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  attachInterrupt(0, weld, FALLING);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(4);
  display.setTextColor(WHITE);
}

void loop() {
  if (wait == 0) {
    display.clearDisplay();
    display.setCursor(0, 0);
    timems = map(analogRead(A0), 0, 1023, 0, 100);
    display.print(timems);
    display.print("ms");
    display.display();
    delay(100);
  }
  else {
    delay(2000);
    wait = 0;
    attachInterrupt(0, weld, FALLING);
  }
}

void weld() {
  if (digitalRead(2) == LOW) {
    digitalWrite(9, HIGH);
    TCNT1 = 0;
    OCR1A = timems * 250;
    wait = 1;
    detachInterrupt(0);
  }
}

ISR(TIMER1_COMPA_vect) {
  digitalWrite(9, LOW);
}
