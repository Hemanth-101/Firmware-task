#include <EEPROM.h>

unsigned long startTime;
unsigned long endTime;
unsigned long totalBits;
String currentWord;

const int EEPROM_SIZE = 1024;
const int EEPROM_START_ADDR = 0;

void setup() {
  Serial.begin(2400);
}

void loop() {
  static String paragraph;

  if (Serial.available() > 0) {
    startTime = micros();
    char incomingByte = Serial.read();
    paragraph += incomingByte;
    
    EEPROM.write(EEPROM_START_ADDR + paragraph.length() - 1, incomingByte);
    
    if (incomingByte == '\n') {
      endTime = micros();
      totalBits = paragraph.length() * 8;
      float transmissionTime = (endTime - startTime) / 1000000.0;
      float transmissionSpeed = totalBits / transmissionTime;
      
      Serial.println("Received paragraph:");
      Serial.println(paragraph);
      Serial.print("Transmission speed for received input: ");
      Serial.print(transmissionSpeed);
      Serial.println(" bits/second");
      
      for (int i = 0; i < paragraph.length(); i++) {
        char c = paragraph[i];
        if (c != ' ' && c != '\n') {
          currentWord += c;
        } 
        else {
          startTime = micros();
          
          Serial.print("Transmission speed for ");
          Serial.print(currentWord);
          Serial.print(": ");
          Serial.flush();
          delay(100);
          endTime = micros();
          
          totalBits = currentWord.length() * 8;
          transmissionTime = (endTime - startTime) / 1000000.0;
          transmissionSpeed = totalBits / transmissionTime;
     
          Serial.print(transmissionSpeed);
          Serial.println(" bits/second");
          currentWord = "";
        }
      }
     paragraph = "";
    }
  }
}
