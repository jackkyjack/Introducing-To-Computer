#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET-1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 OLED(OLED_RESET);

struct ball
{
  uint8_t x , y = 14 ;
  uint8_t radius = 1;
  uint8_t dir_x, dir_y;
  int ran_dir[2] = {-1,1}; 
  unsigned long rate = 1;
  unsigned long ball_update;
};
ball ball;

struct player {
  uint8_t x, y = 31;
  int line = 12;
};
player player;

void player_change() {
  player.x = map(analogRead(A0), 0, 950, 0, 106);
  OLED.drawLine(player.x, player.y, player.x + player.line, player.y, WHITE);
}

void ball_change() {
  ball.x += ball.dir_x;
  ball.y += ball.dir_y;
  OLED.fillCircle(ball.x, ball.y , ball.radius, WHITE);
}

void wall() {
  OLED.drawLine(0, 0, 127, 0, WHITE);
  OLED.drawLine(0, 0, 0, 31, WHITE);
  OLED.drawLine(127, 0, 127, 31, WHITE);
}



void setup() {
  Serial.begin(9600);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  randomSeed(analogRead(1));
  ball.x = random(ball.radius, 127 - ball.radius);
  ball.dir_x = ball.ran_dir[random(0,2)];
  ball.dir_y = ball.ran_dir[random(0,2)];
}

void loop() {
  OLED.clearDisplay();
  wall();
  player_change();
  ball_change();

  if (ball.x  > 127 - ball.radius-3) {
    ball.dir_x = -ball.dir_x;
  }
  else if (ball.x < ball.radius+3){
    ball.dir_x = -ball.dir_x;
    }
  if (ball.y > player.y - ball.radius){
    if(ball.x >= player.x && ball.x <= player.x + player.line){
      ball.dir_y = -ball.dir_y;
    }
    else{
      OLED.clearDisplay();
      OLED.setTextColor(WHITE);
      OLED.setCursor(45, 15);
      OLED.setTextSize(1);
      OLED.println("GAME OVER");
      OLED.display();
      } 
    
  }
  else if (ball.y < ball.radius+3){
    ball.dir_y = -ball.dir_y;
    }


  OLED.display();

  //  Serial.println(val1);
  //  delay(250);
}
