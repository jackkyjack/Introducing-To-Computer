#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerOne.h>
#include <EEPROM.h>

#define BUTTON_TIMER 8
#define BUTTON_SAVE 9
#define BUTTON_EDIT 10
#define BUTTON_PLUS 11
#define BUTTON_MINUS 12

#define analogZ A0
#define analogY A1
#define analogX A2
bool clockRotation = false;
const unsigned int numAxes = 3;
const unsigned int pins[numAxes] = {analogX, analogY, analogZ};
const unsigned int  bufferSize = 16;
int buffer[numAxes] [bufferSize];
int bufferPos[numAxes] = {0};

unsigned long timer = millis();

int timeUnit = 0;
int timerON = 0;


bool UnderLineWhite = true;
bool LastButtonReadMinus, LastButtonReadPlus, LastButtonReadEdit, LastButtonReadSave , LastButtonReadTimer ;
bool timerun = false;
bool printSave = false;

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);//
int hour , minute , second, Thour = 0 , Tminute = 0 , Tsecond = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);

  //EEPROM
  EEPROM.get(0, hour);//เลข 0 คือตำแหน่งที่เก็บข้อมูล
  EEPROM.get(8, minute);
  EEPROM.get(16, second);

  //Button
  pinMode(BUTTON_TIMER, INPUT_PULLUP);
  pinMode(BUTTON_EDIT, INPUT_PULLUP);
  pinMode(BUTTON_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_MINUS, INPUT_PULLUP);
  pinMode(BUTTON_SAVE, INPUT_PULLUP);


  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(count);

  EEPROM.put(0, hour);
  EEPROM.put(8, minute);
  EEPROM.put(16, second);

  hour    = EEPROM.read(0);
  minute  = EEPROM.read(8);
  second  = EEPROM.read(16);
}
void loop() {
  display.clearDisplay();
  CheckButton();

  printtime();
  printUnderLine();
  LineBlink();
  Timer();


  display.display();
  delay(100);

  Serial.println(getX());
  Serial.println(getY());
  Serial.println(getZ());

  if (getZ() < 400)
  {
    clockRotation = true;
  }
  else
  {
    clockRotation = false;
  }
}


void CheckButton()
{
  //  Button Edit
  int buttonReadEdit = digitalRead(BUTTON_EDIT);
  if (buttonReadEdit == LOW && buttonReadEdit != LastButtonReadEdit )
  {
    Serial.println("push");
    timeUnit++;
    if (timeUnit == 4)
    {
      timeUnit = 0;
    }
    //EditButton = true;

  }
  LastButtonReadEdit = buttonReadEdit;

  //  Button Plus
  int buttonReadPlus = digitalRead(BUTTON_PLUS);
  if (buttonReadPlus == LOW && buttonReadPlus != LastButtonReadPlus )
  {
    Serial.println("push");
    Plus();

  }
  LastButtonReadPlus = buttonReadPlus;

  //  Button Minus
  int buttonReadMinus = digitalRead(BUTTON_MINUS);
  if (buttonReadMinus == LOW && buttonReadMinus != LastButtonReadMinus )
  {
    Serial.println("push");
    Minus();
  }
  LastButtonReadMinus = buttonReadMinus;

  //  Button Save
  int buttonReadSave = digitalRead(BUTTON_SAVE);
  if (buttonReadSave == LOW && buttonReadSave != LastButtonReadSave )
  {
    Serial.println("push");
      EEPROM.put(0, hour);
      EEPROM.put(8, minute);
      EEPROM.put(16, second);
  }

  LastButtonReadSave = buttonReadSave;

  //  Button Timer
  int buttonReadTimer = digitalRead(BUTTON_TIMER);
  if (buttonReadTimer == LOW && buttonReadTimer != LastButtonReadTimer )
  {
    Serial.println("timer");
    timerON++;
    if (timerON == 4)
    {
      timerON = 0;
    }
  }
  LastButtonReadTimer = buttonReadTimer;


}


void Timer()
{
  if (clockRotation == true)
  {
    display.setRotation(2);
  }
  else
  {
    display.setRotation(0);
  }
  if (timerON == 0)
  {
    Thour = 0;
    Tminute = 0;
    Tsecond = 0;
  }
  else if (timerON == 1 || timerON == 2 || timerON == 3)
  {
    timerun = true;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1.5);
    display.setTextColor(WHITE);
    if (timerON == 1)
    {
      display.print("Timer MODE");
    }
    else if (timerON == 2)
    {
      display.print("Timer MODE On");
    }
    else if (timerON == 3)
    {
      display.print("Timer MODE Stop");
    }

    display.setCursor(20, 15);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    if (Thour < 10) {
      display.print("0");
    }
    display.print(Thour);

    display.print(":");
    if (Tminute < 10) {
      display.print("0");
    }
    display.print(Tminute);
    display.print(":");
    if (Tsecond < 10) {
      display.print("0");
    }
    display.print(Tsecond);
  }
  if (timerON == 3)
  {
    timerun = false;
  }
}

void Plus()
{
  if (timeUnit == 1)
  {
    hour++;
    if (hour > 23)
    {
      hour = 0;
    }
  }
  if (timeUnit == 2)
  {
    minute++;
    if (minute > 59)
    {
      minute = 0;
    }
  }
  if (timeUnit == 3)
  {
    second++;
    if (second > 59)
    {
      second = 0;
    }
  }
}

void Minus()
{
  if (timeUnit == 1 && timeUnit != 0 )
  {
    hour--;
    if (hour < 0)
    {
      hour = 23;
    }
  }
  if (timeUnit == 2)
  {
    minute--;
    if (minute < 0)
    {
      minute = 59;
    }
  }
  if (timeUnit == 3)
  {
    second--;
    if (second < 0)
    {
      second = 59;
    }
  }
}

void printtime()
{
  if (clockRotation == true)
  {
    display.setRotation(2);
  }
  else
  {
    display.setRotation(0);
  }
  if (timeUnit != 0)
  {
    if (timeUnit == 1)
    {
      display.setCursor(0, 0);
      display.setTextSize(1.5);
      display.setTextColor(WHITE);
      display.print("Set hour");
    }
    if (timeUnit == 2)
    {
      display.setCursor(0, 0);
      display.setTextSize(1.5);
      display.setTextColor(WHITE);
      display.print("Set minute");
    }
    if (timeUnit == 3)
    {
      display.setCursor(0, 0);
      display.setTextSize(1.5);
      display.setTextColor(WHITE);
      display.print("Set secound");
    }


    display.setCursor(20, 15);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    if (hour < 10) {
      display.print("0");
    }
    display.print(hour);

    display.print(":");
    if (minute < 10) {
      display.print("0");
    }
    display.print(minute);
    display.print(":");
    if (second < 10) {
      display.print("0");
    }
    display.print(second);
  }
  else if (timeUnit == 0) {
    display.clearDisplay();
    display.setCursor(20, 15);
    display.setTextColor(WHITE);
    if (hour < 10) {
      display.print("0");
    }
    display.print(hour);

    display.print(":");
    if (minute < 10) {
      display.print("0");
    }
    display.print(minute);
    display.print(":");
    if (second < 10) {
      display.print("0");
    }
    display.print(second);
  }

}

void printUnderLine()
{
  if (clockRotation == true)
  {
    display.setRotation(2);
  }
  else
  {
    display.setRotation(0);
  }
  Serial.println(UnderLineWhite);
  display.setCursor(20, 18);
  if (timeUnit == 0)
  {
    Serial.println("nooooo");
    display.setTextColor(BLACK);
  }
  else if (UnderLineWhite == true && timeUnit != 0 )
  {
    display.setTextColor(WHITE);
  }
  else if (UnderLineWhite == false && timeUnit != 0 )
  {
    display.setTextColor(BLACK);
  }

  if (timeUnit == 1)
  {
    display.print("__");
  }
  else if (timeUnit == 2)
  {
    display.print("   __");
  }
  else if (timeUnit == 3)
  {
    display.print("      __");
  }
}

void count() {
  second++;
  if (second == 60)
  {
    second = 0;
    minute++;
  }
  if (minute == 60)
  {
    minute = 0;
    hour++;
  }
  hour = hour % 24;

  if (timerON == 2 && timerun == true)
  {
    Tsecond++;
    if (Tsecond == 60)
    {
      Tsecond = 0;
      Tminute++;
    }
    if (Tminute == 60)
    {
      Tminute = 0;
      Thour++;
    }
    Thour = Thour % 24;
  }

}


void LineBlink()
{
  if (millis() - timer >= 500)
  {
    UnderLineWhite = !UnderLineWhite ;
    timer += 500;
  }
}

int getAxis(const int axis)
{
  buffer[axis][bufferPos[axis]] = analogRead(pins[axis]);
  bufferPos[axis] = (bufferPos[axis] + 1) % bufferSize;
  long sum = 0;
  for (unsigned int i = 0; i < bufferSize; i++)
  {
    sum += buffer[axis][i];
  }
  return round(sum / bufferSize);
}

int getX()
{
  return getAxis(0);
}

int getY()
{
  return getAxis(1);
}

int getZ()
{
  return getAxis(2);
}
