#include <Arduino.h>

const int dataPin = 2;
const int latchPin = 3;
const int clockPin = 4;
const int leftButtonPin = 10;
const int rightButtonPin = 9;
const int buzzerPin = 12;


byte digitsMask[10] = {
  // a, b, c, d, e, f, g, dp
  0b00000011,  // 0
  0b10011111,  // 1
  0b00100101,  // 2
  0b00001101,  // 3
  0b10011001,  // 4
  0b01001001,  // 5
  0b01000001,  // 6
  0b00011111,  // 7
  0b00000001,  // 8
  0b00001001   // 9
};


byte gameOverChars[8] = {
  // a, b, c, d, e, f, g, dp
  0b01000011,  // G
  0b00010001,  // A
  0b01010111,  // M
  0b01100001,  // E
  0b00000011,  // O
  0b10101011,  // V
  0b01100001,  // E
  0b00110011   // R
};

byte digitsInOrder[8] = {
  0b10000000,
  0b01000000,
  0b00100000,
  0b00010000,
  0b00001000,
  0b00000100,
  0b00000010,
  0b00000001
};

int position = 0;
int game = -1;

void main1();
void main2();
void main3();



void send(byte digitSelect, byte segments) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, digitSelect);
  shiftOut(dataPin, clockPin, LSBFIRST, segments);
  digitalWrite(latchPin, HIGH);
}

void clear(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
  shiftOut(dataPin, clockPin, LSBFIRST, 0b11111111);
  digitalWrite(latchPin, HIGH);
}

bool buttonLeftPressed(){
  return !digitalRead(leftButtonPin);
}

bool buttonRightPressed(){
  return !digitalRead(rightButtonPin);
}






void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  while (game == -1){
    if (buttonLeftPressed()){
      delay(70);
      if (buttonLeftPressed()){
        game = position;
      }
    }
    
    if (buttonRightPressed()){
      delay(70);
      if (buttonRightPressed()){
        tone(buzzerPin, 880, 200);
        if (position < 9){
          position += 1;
          delay(100);
        } else{
          position = 0;
          delay(100);
        }
      }
    }
    send(0b10000000, digitsMask[position]);
  }
  if (game == 0){
    while (1){
      main1();
    }
  } else if (game == 1){
    while (1){
      main2();
    }
  } else if (game == 2){
    while (1){
      main3();
    }
  }
}



void main1(){
  const int moving_time = 300;
  byte player_sprites[2] = {
    // a, b, c, d, e, f, g, dp
    0b00111001, // up
    0b11000101  // down
  };

  byte o_sprites[4] = {
    // a, b, c, d, e, f, g, dp
    0b11111011,  // up 2
    0b11110111,  // down 2
    0b10111111,  // up 1
    0b11011111   // down 1
  };

  byte up_digs[4] = {
    // 1, 2, 3, 4
    0b10000000,  // 1
    0b01000000,  // 2
    0b00100000,  // 3
    0b00010000   // 4
  };

  byte down_digs[4] = {
    // 5, 6, 7, 8
    0b00001000,  // 1
    0b00000100,  // 2
    0b00000010,  // 3
    0b00000001   // 4
  };

  randomSeed(analogRead(0));
  int pos = 3;
  int pos_o_x_1 = 7;
  int pos_o_y_1 = random(4);
  int o_1_dig;
  int o_1_sprite;
  int pos_o_x_2 = 5;
  int pos_o_y_2 = random(4);
  int o_2_dig;
  int o_2_sprite;
  int pos_o_x_3 = 3;
  int pos_o_y_3 = random(4);
  int o_3_dig;
  int o_3_sprite;
  long int timer = millis();
  long int button_timer = millis();
  while (1){
    if (millis() - button_timer >= 130){
      button_timer = millis();
      if (buttonLeftPressed() && pos < 3){
        tone(buzzerPin, 440, 100);
        pos += 1;
      }
      if (buttonRightPressed() && pos > 0){
        tone(buzzerPin, 440, 100);
        pos -= 1;
      }
    }
    
    if ((pos_o_x_1 == 1 | pos_o_x_1 == 0) && (pos_o_y_1 == pos)) {
      break;
    }
    if ((pos_o_x_2 == 1 | pos_o_x_2 == 0) && (pos_o_y_2 == pos)) {
      break;
    }
    if ((pos_o_x_2 == 1 | pos_o_x_2 == 0) && (pos_o_y_2 == pos)) {
      break;
    }

    bool disp_player_num = pos / 2;
    send(disp_player_num ? 0b00001000 : 0b10000000, player_sprites[pos % 2]);
    o_1_sprite = o_sprites[pos_o_y_1 % 2 + 2 * (pos_o_x_1 % 2)];
    o_2_sprite = o_sprites[pos_o_y_2 % 2 + 2 * (pos_o_x_2 % 2)];
    o_3_sprite = o_sprites[pos_o_y_3 % 2 + 2 * (pos_o_x_3 % 2)];
    if (pos_o_y_1 < 2){
      o_1_dig = up_digs[pos_o_x_1 / 2];
    } else {
      o_1_dig = down_digs[pos_o_x_1 / 2];
    }
    if (pos_o_y_2 < 2){
      o_2_dig = up_digs[pos_o_x_2 / 2];
    } else {
      o_2_dig = down_digs[pos_o_x_2 / 2];
    }
    if (pos_o_y_3 < 2){
      o_3_dig = up_digs[pos_o_x_3 / 2];
    } else {
      o_3_dig = down_digs[pos_o_x_3 / 2];
    }
    send(o_1_dig, o_1_sprite);
    send(o_2_dig, o_2_sprite);
    send(o_3_dig, o_3_sprite);
    if (millis() - timer > moving_time){
      pos_o_x_1 -= 1;
      pos_o_x_2 -= 1;
      pos_o_x_3 -= 1;
      timer = millis();
      if (pos_o_x_1 < 0){
        if (pos_o_x_2 == pos_o_x_3 == 1){
          pos_o_x_1 = random(1, 4);
        } else if (pos_o_x_2 == pos_o_x_3 == 2){
          pos_o_x_1 = random(3);
        } else {
          if (random(3) == 0){
            pos_o_y_1 = pos;
          } else {
            pos_o_y_1 = random(4);
          }
        }
        pos_o_x_1 = 7;
      }
      if (pos_o_x_2 < 0){
        if (random(3) == 0){
          pos_o_y_2 = pos;
        } else {
          pos_o_y_2 = random(4);
        }
        pos_o_x_2 = 7;
      }
      if (pos_o_x_3 < 0){
        if (random(3) == 0){
          pos_o_y_3 = pos;
        } else {
          pos_o_y_3 = random(4);
        }
        pos_o_x_3 = 7;
      }
    }
  }
  clear();
  tone(buzzerPin, 587.32, 200);
  delay(200);
  tone(buzzerPin, 466.16, 200);
  delay(200);
  tone(buzzerPin, 392, 200);
  delay(600);
  while (1){
    for (int i = 0; i < 8; i++){
      send(digitsInOrder[i], gameOverChars[i]);
    }
    if (button_timer - millis() >= 50 && buttonLeftPressed()){
      break;
    }
  }

}

void main2() {
  byte player_sprite = 0b11101111;
  byte stones[2] = {
    0b00111001, 
    0b11000101
  };
  byte up_digs[4] = {
    0b10000000, 
    0b01000000, 
    0b00100000, 
    0b00010000
  };
  byte down_digs[4] = {
    0b00001000, 
    0b00000100, 
    0b00000010, 
    0b00000001
  };
  
  randomSeed(analogRead(0));
  int pos = 3;
  int pos_o_x_1 = random(4);
  int pos_o_y_1 = 0;
  int o_1_dig;
  int o_1_sprite;
  int pos_o_x_2 = random(4);
  int pos_o_y_2 = 1;
  int o_2_dig;
  int o_2_sprite;
  int pos_o_x_3 = random(4);
  int pos_o_y_3 = 2;
  int o_3_dig;
  int o_3_sprite;
  long int timer = millis();
  long int button_timer = millis();

  while (1) {
    if (millis() - button_timer >= 130){
      button_timer = millis();
      if (buttonLeftPressed() && pos > 0){
        tone(buzzerPin, 440, 100);
        pos -= 1;
      }
      if (buttonRightPressed() && pos < 3){
        tone(buzzerPin, 440, 100);
        pos += 1;
      }
    }
    
    if ((pos_o_y_1 == 3) && (pos_o_x_1 != pos)) {
      break;
    }
    if ((pos_o_y_1 == 3) && (pos_o_x_1 != pos)) {
      break;
    }
    if ((pos_o_y_1 == 3) && (pos_o_x_1 != pos)) {
      break;
    }

    if (pos_o_y_1 < 2){
      o_1_dig = up_digs[pos_o_x_1 / 2];
    } else {
      o_1_dig = down_digs[pos_o_x_1 / 2];
    }
    if (pos_o_y_2 < 2){
      o_2_dig = up_digs[pos_o_x_2 / 2];
    } else {
      o_2_dig = down_digs[pos_o_x_2 / 2];
    }
    if (pos_o_y_3 < 2){
      o_3_dig = up_digs[pos_o_x_3 / 2];
    } else {
      o_3_dig = down_digs[pos_o_x_3 / 2];
    }
    send(down_digs[pos % 4], player_sprite);
    send(pos_o_y_1 < 2 ? up_digs[pos_o_x_1] : down_digs[pos_o_x_1], stones[pos_o_y_1 % 2]);
    send(pos_o_y_2 < 2 ? up_digs[pos_o_x_2] : down_digs[pos_o_x_2], stones[pos_o_y_2 % 2]);
    send(pos_o_y_3 < 2 ? up_digs[pos_o_x_3] : down_digs[pos_o_x_3], stones[pos_o_y_3 % 2]);
    if (millis() - timer >= 1000) {
      if (pos_o_y_1 < 3) {
        pos_o_y_1 += 1;
      } else {
        pos_o_y_1 = 0;
        pos_o_x_1 = random(4);
      }
      if (pos_o_y_2 < 3) {
        pos_o_y_2 += 1;
      } else {
        pos_o_y_2 = 0;
        pos_o_x_2 = random(4);
      }
      if (pos_o_y_3 < 3) {
        pos_o_y_3 += 1;
      } else {
        pos_o_y_3 = 0;
        pos_o_x_3 = random(4);
      }
    }
  }
  clear();
  tone(buzzerPin, 587.32, 200);
  delay(200);
  tone(buzzerPin, 466.16, 200);
  delay(200);
  tone(buzzerPin, 392, 200);
  delay(600);
  while (1){
    for (int i = 0; i < 8; i++){
      send(digitsInOrder[i], gameOverChars[i]);
    }
    if (button_timer - millis() >= 50 && buttonLeftPressed()){
      break;
    }
  }
}

void main3() {
  byte label[6] = {
    0b01110011,  // r 
    0b01100001,  // e
    0b00010001,  // a
    0b10000101,  // d
    0b10001001,  // y
    0b00110100   // ?
  };
  byte digits[6] = {
    0b10000000, 
    0b01000000, 
    0b00100000, 
    0b00010000, 
    0b00001000, 
    0b00000100
  };
  byte numbers[10] = {
    0b00000111,  // 0
    0b10011111,  // 1
    0b00100101,  // 2
    0b00001101,  // 3
    0b10011001,  // 4
    0b01001001,  // 5
    0b01000001,  // 6
    0b00011111,  // 7
    0b00000001,  // 8
    0b00001001,  // 9
  };
  randomSeed(analogRead(A0));
  long int timer = millis();
  int delta = random(3000);
  while (!buttonLeftPressed()) {
    for (int i; i < 6; i++) {
      send(digits[i], label[i]);
    }  
  }
  while (millis() - timer <= delta) {
    if (buttonRightPressed()) {
      break;
    }
  }
  timer = millis();
  while (!buttonRightPressed()) {
    send(0b1111111, 0b00000000);
  }
  int time = (millis() - timer);
  if (time <= 9999) {
    timer = millis();
    while (millis() - timer < 3000) {
      for (int i; i < 4; i++) {
        send(digits[i], numbers[time / (10 * (i + 1)) % 10]);
      }
      send(digits[0], 0b11111110);
    }
  }
}


void loop() {
}