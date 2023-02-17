#include <Arduino.h>
int counter = 0;   // counterという名前の変数を用意して、0をセットする

void setup() {
      // 起動後、この中の内容が上から順に1回実行される
      Serial.begin(9600);   // シリアル通信の準備をする
      while (!Serial)
            ;   // 準備が終わるのを待つ
      Serial.println("プログラム開始");   // シリアル通信でメッセージをPCに送信

      // 最後まで実行された後はloop関数に移行する
}

void loop() {
      // setup関数が終了すると、この中の内容が上から順に何度も実行されつづける

      Serial.print("カウンターの値 ");   // シリアル通信でメッセージをPCに送信
      Serial.println(counter);   // シリアル通信でカウンターの値をPCに送信
      counter = counter + 1;   // カウンターの値を1増やす
      delay(1000);   // 1000ミリ秒(1秒)待機

      // 最後まで実行された後はloop関数の先頭に戻る
}