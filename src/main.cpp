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
#define ANGLE_MOVING_AVERAGE_COUNT_NUMBER 10
#define DISTANCE_MOVING_AVERAGE_COUNT_NUMBER 10
#define SERIAL_BAUD 38400   // UART通信速度

uint8_t angle_plus, angle_minus;

uint16_t value[8];
int16_t result_vector_x, result_vector_y;
int16_t angle, distance;
uint8_t max_ir, left_side_ir, right_side_ir, max_value;
uint16_t distance_moving_average_count, tmp_distance[DISTANCE_MOVING_AVERAGE_COUNT_NUMBER];
int16_t angle_moving_average_count, tmp_angle[ANGLE_MOVING_AVERAGE_COUNT_NUMBER];
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

      Serial.begin(SERIAL_BAUD);   // UART通信速度;

      // それぞれのIRセンサの座標を求める
      for (uint8_t count = 0; count < IR_NUM; count++) {
            unit_vector_x[count] = cos((count * 360.00000 / IR_NUM) * PI / 180.00000);
            unit_vector_y[count] = sin((count * 360.00000 / IR_NUM) * PI / 180.00000);
      }
}

void loop() {
      // ボールセンサの値入手
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
      for (uint8_t count = 0; count < IR_NUM; count++) value[count] = (READ_NUMBER_OF_TIME - value[count]) * (100.00000 / READ_NUMBER_OF_TIME);

      // ３つのIRに限定
      max_value = 0;
      for (uint8_t count = 0; count < IR_NUM; count++) {
            if (max_value < value[count]) {
                  max_value = value[count];
                  max_ir = count;
            }
      }
      left_side_ir = max_ir - 1 < 0 ? 7 : max_ir - 1;
      right_side_ir = max_ir + 1 > 7 ? 0 : max_ir + 1;

      // ベクトルの合成
      result_vector_x = value[left_side_ir] * unit_vector_x[left_side_ir] + value[max_ir] * unit_vector_x[max_ir] + value[right_side_ir] * unit_vector_x[right_side_ir];
      result_vector_y = value[left_side_ir] * unit_vector_y[left_side_ir] + value[max_ir] * unit_vector_y[max_ir] + value[right_side_ir] * unit_vector_y[right_side_ir];

      // ボールの角度を求める
      angle = atan2(result_vector_y, result_vector_x) / PI * 180.50000;

      // 角度の移動平均
      if (angle_moving_average_count == ANGLE_MOVING_AVERAGE_COUNT_NUMBER) angle_moving_average_count = 0;
      tmp_angle[angle_moving_average_count] = angle;
      angle = 0;
      for (uint8_t count = 0; count < ANGLE_MOVING_AVERAGE_COUNT_NUMBER; count++) angle += tmp_angle[count];
      angle /= ANGLE_MOVING_AVERAGE_COUNT_NUMBER;
      angle_moving_average_count++;

      // ボールの距離を求める
      //distance = (value[left_side_ir] + value[max_ir] + value[right_side_ir]) / 1.5;
      distance = sqrt(pow(result_vector_x, 2) + pow(result_vector_y, 2)) / 1.1;

      // 距離の移動平均
      if (distance_moving_average_count == DISTANCE_MOVING_AVERAGE_COUNT_NUMBER) distance_moving_average_count = 0;
      tmp_distance[distance_moving_average_count] = distance;
      distance = 0;
      for (uint8_t count = 0; count < DISTANCE_MOVING_AVERAGE_COUNT_NUMBER; count++) distance += tmp_distance[count];
      distance /= DISTANCE_MOVING_AVERAGE_COUNT_NUMBER;
      distance_moving_average_count++;

      if (distance > 100) distance = 100;

      // 距離補正
      //distance *= 2;
      //if (distance == 50) distance = 0;

      // メインマイコンに送る
      angle_plus = angle > 0 ? angle : 0;
      angle_minus = angle < 0 ? angle * -1 : 0;

      Serial.write('a');
      Serial.write(angle_plus);
      Serial.write(angle_minus);
      Serial.write(distance);
      Serial.flush();   // 送り終わるまで待機
}