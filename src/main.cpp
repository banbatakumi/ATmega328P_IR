#include <Arduino.h>

#define PIN_0 9
#define PIN_1 3
#define PIN_2 6
#define PIN_3 5
#define PIN_4 8
#define PIN_5 2
#define PIN_6 7
#define PIN_7 4
#define IR_NUM 8
#define READ_NUMBER_OF_TIME 500

uint16_t value[8];

void setup() {
      pinMode(PIN_0, INPUT);
      pinMode(PIN_1, INPUT);
      pinMode(PIN_2, INPUT);
      pinMode(PIN_3, INPUT);
      pinMode(PIN_4, INPUT);
      pinMode(PIN_5, INPUT);
      pinMode(PIN_6, INPUT);
      pinMode(PIN_7, INPUT);

      Serial.begin(9600);
}

void loop() {
      for (uint8_t count = 0; count < IR_NUM; count++) value[count] = 0;
      for (uint16_t count = 0; count < READ_NUMBER_OF_TIME; count++) {
            value[0] += digitalRead(PIN_0);
            value[1] += digitalRead(PIN_1);
            value[2] += digitalRead(PIN_2);
            value[3] += digitalRead(PIN_3);
            value[4] += digitalRead(PIN_4);
            value[5] += digitalRead(PIN_5);
            value[6] += digitalRead(PIN_6);
            value[7] += digitalRead(PIN_7);
      }
      Serial.print("0: ");
      Serial.print(value[0]);
      Serial.print("1: ");
      Serial.print(value[1]);
      Serial.print("2: ");
      Serial.print(value[2]);
      Serial.print("3: ");
      Serial.print(value[3]);
      Serial.print("4: ");
      Serial.print(value[4]);
      Serial.print("5: ");
      Serial.print(value[5]);
      Serial.print("6: ");
      Serial.print(value[6]);
      Serial.print("7: ");
      Serial.println(value[7]);
}