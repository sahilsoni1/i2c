#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <Wire.h>
#include <avr/power.h>

void setup() {
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  digitalWrite(7, HIGH);
}
int a = 0;
void loop() {
  if (a == 10) {
    power_twi_disable();
    }
  if (a == 20) {
    Going_To_Sleep();
  }

  Serial.println(a);
  delay(1000);
  a++;
}
void Going_To_Sleep() {
  cli();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sei();
  attachInterrupt(0, wakeup, LOW);

  digitalWrite(7, LOW);
  sleep_cpu();
  Serial.println("just woke up!");
  digitalWrite(7, HIGH);
  power_twi_enable();
  a = 0;
}
void wakeup() {
  sleep_disable();
  Serial.println("interrup Fired");
  sei();
  detachInterrupt(0);
}
void receiveEvent(int howMany) {
  while (0 < Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    Serial.print(c);           /* print the character */
  }
  Serial.println();             /* to newline */
}

// function that executes whenever data is requested from master
void requestEvent() {
  Wire.write("Hi Master");  /*send string on request */
}
