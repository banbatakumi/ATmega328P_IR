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
#define DISTANCE_RC 0.25
#define SERIAL_BAUD 38400   // UART通信速度

uint8_t angle_plus, angle_minus;
uint16_t value[8];
int16_t result_vector_x, result_vector_y;
int16_t pre_angle, pre_distance, angle, distance;
int16_t max_ir, max_value;
float unit_vector_x[IR_NUM];
float unit_vector_y[IR_NUM];

void setup() {
      DDRB = 0b00000000;   // ポートBを全てINPUT
      DDRD = 0b00000000;   // ポートDを全てINPUT
      /*
      pinMode(PIN_0, INPUT);
      pinMode(PIN_1, INPUT);
      pinMode(PIN_2, INPUT);
      pinMode(PIN_3, INPUT);
      pinMode(PIN_4, INPUT);
      pinMode(PIN_5, INPUT);
      pinMode(PIN_6, INPUT);
      pinMode(PIN_7, INPUT);
      */

      Serial.begin(SERIAL_BAUD);   // UART通信速度);

      for (uint8_t count = 0; count < IR_NUM; count++) {
            unit_vector_x[count] = cos((count * 360.000 / IR_NUM) * PI / 180.000);
            unit_vector_y[count] = sin((count * 360.000 / IR_NUM) * PI / 180.000);
      }
}

void loop() {
      for (uint8_t count = 0; count < IR_NUM; count++) value[count] = 0;
      for (uint16_t count = 0; count < READ_NUMBER_OF_TIME; count++) {
            value[0] += (PINB >> 1) & 0b00000001;   // digitalRead(PIN_0);
            value[1] += (PIND >> 3) & 0b00000001;   // digitalRead(PIN_1);
            value[2] += (PIND >> 6) & 0b00000001;   // digitalRead(PIN_2);
            value[3] += (PIND >> 5) & 0b00000001;   // digitalRead(PIN_3);
            value[4] += PINB & 0b00000001;   // digitalRead(PIN_4);
            value[5] += (PIND >> 2) & 0b00000001;   // digitalRead(PIN_5);
            value[6] += (PIND >> 7) & 0b00000001;   // digitalRead(PIN_6);
            value[7] += (PIND >> 4) & 0b00000001;   // digitalRead(PIN_7);
      }
      for (uint8_t count = 0; count < IR_NUM; count++) value[count] = (READ_NUMBER_OF_TIME - value[count]) / (READ_NUMBER_OF_TIME / 100);

      result_vector_x = 0;
      result_vector_y = 0;
      for (uint8_t count = 0; count < IR_NUM; count++) {
            result_vector_x += value[count] * unit_vector_x[count];
            result_vector_y += value[count] * unit_vector_y[count];
      }

      angle = atan2(result_vector_y, result_vector_x) / PI * 180.500;

      max_value = 0;
      for (uint8_t count = 0; count < IR_NUM; count++) {
            if (max_value < value[count]){
                  max_value = value[count];
                  max_ir = count;
            }
      }
      distance = (value[max_ir - 1 < 0 ? 7 : max_ir - 1] + value[max_ir] + value[max_ir + 1 > 7 ? 0 : max_ir + 1]) / 3;
/*
      distance = 0;
      for (uint8_t count = 0; count < IR_NUM; count++) {
            if (distance < value[count]) distance = value[count];
      }*/
      distance += 50;
      if (distance == 50) distance = 0;
      distance = (1 - DISTANCE_RC) * distance + DISTANCE_RC * pre_distance;
      pre_distance = distance;

      angle_plus = angle > 0 ? angle : 0;
      angle_minus = angle < 0 ? angle * -1 : 0;

      Serial.write('a');
      Serial.write(angle_plus);
      Serial.write(angle_minus);
      Serial.write(distance);
      Serial.flush();

      /*
      Serial.print("angle: ");
      Serial.print(angle);
      Serial.print(" distance: ");
      Serial.println(distance);
      */
}