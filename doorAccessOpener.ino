#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <IRremote.h>

#define SS_PIN 53
#define RST_PIN 5
#define LED_DENY 22
#define LED_ACCEPT 24
#define OPEN_GATE 4000
#define LED_PROCESS 28

MFRC522 mfrc522(SS_PIN, RST_PIN); 
Servo myservo;

int pos = 1;
int x = 0;

boolean both = true;

int speeder[3] = {4, 15, 100};
byte circle_card[4] = {0xD4, 0x33, 0x83, 0x63};
byte flat_card[4] = {0xC0, 0xE6, 0x85, 0xA7};


void setup() {
  pinMode(38, INPUT);
  pinMode(LED_ACCEPT, OUTPUT);
  pinMode(LED_DENY, OUTPUT);
  pinMode(LED_PROCESS, OUTPUT);
  pinMode(A0, INPUT);
  myservo.attach(9);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  myservo.write(pos);
  Serial.println("SCAN CARD:");
}

void loop() {

  Serial.println(analogRead(A0));
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  if (check_card()) {
    open_gate(5000, speeder[0]);
  } else {
    blinker(LED_DENY);
  }
}


boolean check_card() {

  for (int i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] == flat_card[i] || mfrc522.uid.uidByte[i] == circle_card[i]) {
      ++x;
    }
  }

  if (x == 4) {
    x = 0;
    return true;
  } else {
    x = 0;
    return false;
  }

}

void open_gate(int delay_open, int speed_of_gate) {

  blinker(LED_ACCEPT);

  for (int x = 0; x < 2; x++) {

  }
  for (pos = 0; pos <= 170; pos += 1) {
    myservo.write(pos);
    delay(speed_of_gate);
    digitalWrite(LED_PROCESS, HIGH);
  }
  delay(OPEN_GATE);
  for (pos = 170; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(speed_of_gate);
  }
  digitalWrite(LED_PROCESS, LOW);
}

void blinker(int led) {
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
}


