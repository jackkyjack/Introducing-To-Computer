#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerOne.h>
#include <EEPROM.h>
#define OLED_RESET-1
#define SCREEN_ADDRESS 0x3C
#define Z_AXIS_PIN A0
#define Y_AXIS_PIN A1
#define X_AXIS_PIN A2
#define button_mode 8
#define button_select 9
#define button_save 10
#define button_plus 11
#define button_minus 12
Adafruit_SSD1306 OLED(OLED_RESET);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
const unsigned int BUFFER_SIZE = 16;
const unsigned int BAUD_RATE = 9600;
const unsigned int NUM_AXES = 3;
const unsigned int PINS[NUM_AXES] = {
  X_AXIS_PIN, Y_AXIS_PIN, Z_AXIS_PIN
};
int buffer[NUM_AXES][BUFFER_SIZE];
int buffer_pos[NUM_AXES] = { 0 };
//default


int MODE, SELECT, SAVE, PLUS, MINUS;
bool lastMODE, lastSELECT, lastSAVE, lastPLUS, lastMINUS;
bool SELECTSELECT = false;
//button

bool clockRotation = false;
//bool TIMETIME = false;
int timecur = 0;
int TIMEUNIT = 0;
int TIMEMODE = 0;
int hh , mm , ss, fh = 0 , fm = 0 , fs = 0;
//clock


//██████╗░███████╗███████╗██╗███╗░░██╗███████╗
//██╔══██╗██╔════╝██╔════╝██║████╗░██║██╔════╝
//██║░░██║█████╗░░█████╗░░██║██╔██╗██║█████╗░░
//██║░░██║██╔══╝░░██╔══╝░░██║██║╚████║██╔══╝░░
//██████╔╝███████╗██║░░░░░██║██║░╚███║███████╗
//╚═════╝░╚══════╝╚═╝░░░░░╚═╝╚═╝░░╚══╝╚══════╝

//========================================================================

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(runtime);

  pinMode(button_mode, INPUT_PULLUP);
  pinMode(button_select, INPUT_PULLUP);
  pinMode(button_save, INPUT_PULLUP);
  pinMode(button_plus, INPUT_PULLUP);
  pinMode(button_minus, INPUT_PULLUP);


  EEPROM.get(0, hh);
  EEPROM.get(8, mm);
  EEPROM.get(16, ss);

  EEPROM.put(0, hh);
  EEPROM.put(8, mm);
  EEPROM.put(16, ss);

  hh  = EEPROM.read(0);
  mm  = EEPROM.read(8);
  ss  = EEPROM.read(16);

  hh = 0;
  mm = 0;
  ss = 0;

}


//░██████╗███████╗████████╗██╗░░░██╗██████╗░
//██╔════╝██╔════╝╚══██╔══╝██║░░░██║██╔══██╗
//╚█████╗░█████╗░░░░░██║░░░██║░░░██║██████╔╝
//░╚═══██╗██╔══╝░░░░░██║░░░██║░░░██║██╔═══╝░
//██████╔╝███████╗░░░██║░░░╚██████╔╝██║░░░░░
//╚═════╝░╚══════╝░░░╚═╝░░░░╚═════╝░╚═╝░░░░░

//========================================================================

void runtime() {
  ss++;
  if (ss == 60) {
    ss = 0;
    mm++;
  }
  if (mm == 60) {
    mm = 0;
    hh++;
  }
  hh = hh % 24;
  if (TIMEMODE == 2 && timecur % 2 == 0) {
    fs++;
    if (fs == 60) {
      fs = 0;
      fm++;
    }
    if (fm == 60) {
      fm = 0;
      fh++;
    }
    fh = fh % 24;
  }
}

void checkbutton() {
  MODE = digitalRead(button_mode);
  if (MODE == LOW && MODE != lastMODE) {
    TIMEMODE ++;
    timecur = 0;
    TIMEUNIT = 0;
    if (TIMEMODE == 5) {
      TIMEMODE = 0;
    }
  }
  lastMODE = MODE;
  //MODE

  SELECT = digitalRead(button_select);
  if (SELECT == LOW && SELECT != lastSELECT) {
    SELECTSELECT = true;
    timecur++;
    TIMEUNIT++;
    if (TIMEUNIT == 4) {
      TIMEUNIT = 0;

    }
  }




  //  else {SELECTSELECT = false;}
  lastSELECT = SELECT;
  //SELECT

  SAVE = digitalRead(button_save);
  if (SAVE == LOW && SAVE != lastSAVE) {
    EEPROM.put(0, hh);
    EEPROM.put(8, mm);
    EEPROM.put(16, ss);
  }
  lastSAVE = SAVE;
  //SAVE

  PLUS = digitalRead(button_plus);
  if (PLUS == LOW && PLUS != lastPLUS) {
    plus();
  }
  lastPLUS = PLUS;
  //PLUS

  MINUS = digitalRead(button_minus);
  if (MINUS == LOW && MINUS != lastMINUS) {
    minus();
  }
  lastMINUS = MINUS;
  //MINUS
}

void plus() {
  if (TIMEUNIT == 1) {
    hh++;
    fh++;
    if (hh > 23) {
      hh = 0;
    }
    if (fh > 23) {
      fh = 0;
    }
  }
  if (TIMEUNIT == 2) {
    mm++;
    fm++;
    if (mm > 59) {
      mm = 0;
    }
    if (fm > 59) {
      fm = 0;
    }
  }
  if (TIMEUNIT == 3) {
    ss++;
    fs++;
    if (ss > 59) {
      ss = 0;
    }
    if (fs > 59) {
      fs = 0;
    }
  }
}

void minus() {
  if (TIMEUNIT == 1 ) {
    hh--;
    fh--;
    if (hh < 0) {
      hh = 23;
    }
    if (fh < 0) {
      fh = 23;
    }
  }
  if (TIMEUNIT == 2) {
    mm--;
    fm--;
    if (mm < 0) {
      mm = 59;
    }
    if (fm < 0) {
      fm = 59;
    }
  }
  if (TIMEUNIT == 3) {
    ss--;
    fs--;
    if (ss < 0) {
      ss = 59;
    }
    if (fs < 0) {
      fs = 59;
    }
  }
}

void modemode() {
  if (TIMEMODE == 0) {
    fh = 0;
    fm = 0;
    fs = 0;
  }
  else if (TIMEMODE == 1 || TIMEMODE == 2 || TIMEMODE == 3 || TIMEMODE == 4) {
    //    if(TIMEUNIT != 0){TIMEUNIT = 0;}
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1.5);
    display.setTextColor(WHITE);
    if (TIMEMODE == 1) {
      display.print("EDIT TIME MODE");
      if (SELECTSELECT == true) {
        if (TIMEUNIT == 1) {
          display.clearDisplay();
          display.setCursor(0, 0);
          display.setTextSize(1.5);
          display.setTextColor(WHITE);
          display.print("Set hour");
        }
        else if (TIMEUNIT == 2) {
          display.clearDisplay();
          display.setCursor(0, 0);
          display.setTextSize(1.5);
          display.setTextColor(WHITE);
          display.print("Set minute");
        }
        else if (TIMEUNIT == 3) {
          display.clearDisplay();
          display.setCursor(0, 0);
          display.setTextSize(1.5);
          display.setTextColor(WHITE);
          display.print("Set second");
        }
        printtime();
        if (TIMEUNIT == 0) {
          SELECTSELECT = false;
        }
      }
    }
    else if (TIMEMODE == 2) {
      display.print("COUNT TIME MODE");
      if (SELECTSELECT == true) {
        if (timecur == 1 ) {
          fh = 0;
          fm = 0;
          fs = 0;
          printtimef();
        }
        display.setCursor(20, 15);
        display.setTextSize(2);
        display.setTextColor(WHITE);
        printtimef();
        if (TIMEMODE != 2) {
          SELECTSELECT = false;
        }
      }
    }
    else if (TIMEMODE == 3) {
      SELECTSELECT = false;
      display.print("COUNTDOWN TIME MODE");
      if (SELECTSELECT == true) {
        
        }
    }
    else if (TIMEMODE == 4) {
      SELECTSELECT = false;
      display.print("ALARM TIME MODE");
      //////////
    }
    display.setCursor(20, 15);
    display.setTextSize(2);
    display.setTextColor(WHITE);

  }
}

void clockro() {
  if (clockRotation == true)
  {
    display.setRotation(2);
  }
  else
  {
    display.setRotation(0);
  }
}

int get_axis(const int axis) {
  delay(1);
  buffer[axis][buffer_pos[axis]] = analogRead(PINS[axis]);
  buffer_pos[axis] = (buffer_pos[axis] + 1) % BUFFER_SIZE;
  long sum = 0;
  for (unsigned int i = 0; i < BUFFER_SIZE; i++)sum += buffer[axis][i];
  return round(sum / BUFFER_SIZE);
}

int get_x() {
  return get_axis(0);
}
int get_y() {
  return get_axis(1);
}
int get_z() {
  return get_axis(2);
}

void printtime() {
  display.setCursor(20, 15);
  display.setTextSize(2);
  display.setTextColor(WHITE);

  if (hh < 10) {
    display.print("0");
  }
  display.print(hh);
  display.print(":");
  if (mm < 10) {
    display.print("0");
  }
  display.print(mm);
  display.print(":");
  if (ss < 10) {
    display.print("0");
  }
  display.print(ss);
}

void printtimef() {
  display.setCursor(20, 15);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  if (fh < 10) {
    display.print("0");
  }
  display.print(fh);
  display.print(":");
  if (fm < 10) {
    display.print("0");
  }
  display.print(fm);
  display.print(":");
  if (fs < 10) {
    display.print("0");
  }
  display.print(fs);
}






//███████╗██╗░░░██╗░█████╗░████████╗██╗░█████╗░███╗░░██╗
//██╔════╝██║░░░██║██╔══██╗╚══██╔══╝██║██╔══██╗████╗░██║
//█████╗░░██║░░░██║██║░░╚═╝░░░██║░░░██║██║░░██║██╔██╗██║
//██╔══╝░░██║░░░██║██║░░██╗░░░██║░░░██║██║░░██║██║╚████║
//██║░░░░░╚██████╔╝╚█████╔╝░░░██║░░░██║╚█████╔╝██║░╚███║
//╚═╝░░░░░░╚═════╝░░╚════╝░░░░╚═╝░░░╚═╝░╚════╝░╚═╝░░╚══╝

//========================================================================

void loop() {
  display.clearDisplay();
  checkbutton();
  printtime();
  modemode();
  Serial.println(timecur);


  display.display();
  delay(100);


}

//██╗░░░░░░█████╗░░█████╗░██████╗░
//██║░░░░░██╔══██╗██╔══██╗██╔══██╗
//██║░░░░░██║░░██║██║░░██║██████╔╝
//██║░░░░░██║░░██║██║░░██║██╔═══╝░
//███████╗╚█████╔╝╚█████╔╝██║░░░░░
//╚══════╝░╚════╝░░╚════╝░╚═╝░░░░░
