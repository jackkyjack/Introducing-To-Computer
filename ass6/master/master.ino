#include <Wire.h>
#define SLAVE_ADDR1 8
#define SLAVE_ADDR2 9
char messageR[300];
char empty[10] = "Token#0#";
char temp[300];
int mess = 0;
int len;

void setup() {
  // Initialize I2C communications as Master
  Serial.begin(9600);
  Wire.begin();
  setNormal();
}

void getChar() {
  while (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte != '\n') {
      messageR[mess] = (char)inByte;
      mess++;
    } else {
      len = mess;
      messageR[mess] = '\0';
      mess = 0;
    }
  }
}

int regex() {
  if (messageR[0] > 50 || messageR[0] < 48 || messageR[1] != ' ') {
    return 0;
  }
  return 1;
  //  for(int i=0; i<6; i++){
  //    if(messageR[i] != empty[i]){
  //      return 0;
  //    }
  //  }
  //  if((messageR[6]=='0' || messageR[6] == '1') && messageR[7]=='#' && messageR[9]=='#' && messageR[8]<=50 && messageR[8]>=48 ){
  //    return 1;
  //  } else {
  //    return 0;
  //  }
}

void request() {
  if (Wire.available() > 0) {
    int i = 0;
    while (Wire.available() > 0) {
      int x = Wire.read();
      messageR[i] = (char)x;
      if (messageR[i] == '\0' || x == 255) {
        messageR[i] = '\0';
        break;
      }
      i++;
    }
    len = i;
    Serial.println(messageR);
    showMessage();
    if (isEmpty()) {
      getChar();
      if (!regex()) {
        setNormal();
      } else {
        setMessage();
      }
    }
  }
}

void setMessage() {
  for (int i = 0; i < 8; i++) {
    temp[i] = empty[i];
  }
  temp[6] = '1';
  temp[8] = messageR[0];
  temp[9] = '#';
  int i;
  for (i = 10; i < len + 8; i++) {
    temp[i] = messageR[i - 8];
  }
  temp[i] = '\0';
  for (int j = 0; j < i; j++) {
    messageR[j] = temp[j];
  }
  messageR[i] = '\0';
}

void setNormal() {
  for (int i = 0; i < 8; i++) {
    messageR[i] = empty[i];
  }
  messageR[8] = '\0';
}

int isEmpty() {
  for (int i = 0; i < 8; i++) {
    if (messageR[i] != empty[i]) {
      return 0;
    }
  }
  return 1;
}

void showMessage() {
  if (messageR[6] == '1' && messageR[8] == '0') {
    for (int i = 10; i < len; i++) {
      Serial.print(messageR[i]);
    }
    Serial.println();
    setNormal();
  }
}

void loop() {
  Wire.beginTransmission(SLAVE_ADDR1);
  Wire.write(messageR);
  Wire.endTransmission();
  delay(300);
  Wire.requestFrom(SLAVE_ADDR1, 300);    // request 6 bytes from slave device #8
  request();
  delay(300);

  Wire.beginTransmission(SLAVE_ADDR2);
  Wire.write(messageR);
  Wire.endTransmission();
  delay(300);
  Wire.requestFrom(SLAVE_ADDR2, 300);    // request 6 bytes from slave device #8
  request();
  delay(300);

}
