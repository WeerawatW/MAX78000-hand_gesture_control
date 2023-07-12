#include <IRremote.h>
String key;
int buzzer = 7;

IRsend irsend;

void setup() {
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
}

void loop() {

  while(Serial.available() > 0){
    key = char(Serial.read());
    Serial.print("Receiver: ");
    Serial.println(key);

  if(key == "S"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
  }
    
  if(key == "q"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E08877, 32);  // channel 0
    Serial.println("Channel 0");
  }

  else if(key == "1"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E020DF, 32);  // channel 1
    Serial.println("Channel 1");
  }
  else if(key == "2"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E0A05F, 32); // channel 2
    Serial.println("Channel 2");
  }
  else if(key == "3"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E0609F, 32); // channel 3
    Serial.println("Channel 3");
  }
  else if(key == "4"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E010EF, 32); // channel 4
    Serial.println("Channel 4");
  }
  else if(key == "5"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E0906F, 32); // channel 5
    Serial.println("Channel 5");
  }
  else if(key == "6"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E050AF, 32); // channel 6
    Serial.println("Channel 6");
  }
  else if(key == "7"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E030CF, 32); // channel 7
    Serial.println("Channel 7");
  }
  else if(key == "8"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E0B04F, 32); // channel 8
    Serial.println("Channel 8");
  }
  else if(key == "9"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E0708F, 32); // channel 9
    Serial.println("Channel 9");
  }
  else if(key == "O"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E040BF, 32); // 
    Serial.println("ON, OFF");
  }
  else if(key == "K"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E016E9, 32); // 
    Serial.println("OK");
  }
  else if(key == "L"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E0A659, 32); // 
    Serial.println("LEFT");
  }
  else if(key == "R"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E046B9, 32); // 
    Serial.println("RIGHT");
  }
  else if(key == "U"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E006F9, 32); // up
    Serial.println("UP");
  }
  else if(key == "D"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E08679, 32); // down
    Serial.println("DOWN");
  }
  else if(key == "H"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E09E61, 32);
    Serial.println("Home");
  }
  else if(key == "E"){
    digitalWrite(buzzer, 1);
    delay(100);
    digitalWrite(buzzer, 0);
    irsend.sendSAMSUNG(0xE0E01AE5, 32);
    delay(100);
    irsend.sendSAMSUNG(0xE0E01AE5, 32);
    Serial.println("Return");
  }
  // else if(key == "M"){
  //   digitalWrite(buzzer, 1);
  //   delay(100);
  //   digitalWrite(buzzer, 0);
  //   irsend.sendSAMSUNG(0xE0E0F00F, 32);
  //   Serial.println("Mute");
  // }
  // else if(key == "P"){
  //   digitalWrite(buzzer, 1);
  //   delay(100);
  //   digitalWrite(buzzer, 0);
  //   irsend.sendSAMSUNG(0xE0E0E01F, 32);
  //   delay(50);
  //   irsend.sendSAMSUNG(0xE0E0D02F, 32);
  //   Serial.println("volume up");
  // }
  // else if(key == "N"){
  //   digitalWrite(buzzer, 1);
  //   delay(100);
  //   digitalWrite(buzzer, 0);
  //   irsend.sendSAMSUNG(0xE0E0D02F, 32);
  //   delay(50);
  //   irsend.sendSAMSUNG(0xE0E0D02F, 32);
  //   Serial.println("volume down");
  // }
  // else if(key == "F"){
  //   digitalWrite(buzzer, 1);
  //   delay(100);
  //   digitalWrite(buzzer, 0);
  //   irsend.sendSAMSUNG(0xE0E016E9, 32);
  //   delay(300);
  //   irsend.sendSAMSUNG(0xE0E0A659, 32);
  //   delay(300);
  //   irsend.sendSAMSUNG(0xE0E0A659, 32);
  //   delay(300);
  //   irsend.sendSAMSUNG(0xE0E016E9, 32);
  //   delay(100);
  //   irsend.sendSAMSUNG(0xE0E016E9, 32);
  //   Serial.println("next L");
  // }
  // else if(key == "I"){
  //   digitalWrite(buzzer, 1);
  //   delay(100);
  //   digitalWrite(buzzer, 0);
  //   irsend.sendSAMSUNG(0xE0E016E9, 32);
  //   delay(300);
  //   irsend.sendSAMSUNG(0xE0E046B9, 32);
  //   delay(300);
  //   irsend.sendSAMSUNG(0xE0E046B9, 32);
  //   delay(300);
  //   irsend.sendSAMSUNG(0xE0E016E9, 32);
  //   delay(100);
  //   irsend.sendSAMSUNG(0xE0E016E9, 32);
  //   Serial.println("next R");
  // }
  

  }
  
}


