#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// Create an RF24 object
RF24 radio(9, 8);  // CE, CSN

const byte address[6] = "00001";
byte servoPin = 10;
Servo servo;

struct TrainReplyStruct {
  int x;
  int y;
  int isPresed;
};

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  Serial.begin(9600);
  while (!Serial); // Wait for serial port to connect

  radio.begin();
  if (!radio.isChipConnected()) {
    Serial.println("Radio not connected!");
    while (1); // Halt if no radio
  }

  radio.openReadingPipe(0, address);
  radio.startListening();
  servo.attach(servoPin);
  servo.writeMicroseconds(1500); // Stop signal to ESC
  delay(0.1); // Delay for ESC
}

void loop() {
  if (radio.available()) {
    TrainReplyStruct struckForTheCar;
    radio.read(&struckForTheCar, sizeof(struckForTheCar));
    processJoystick(struckForTheCar.x, struckForTheCar.y);
  }

}

void processJoystick(int xValue, int yValue) {
  if (xValue < 390) {
    digitalWrite(2, LOW);  // Ensure pin 2 is LOW
    digitalWrite(3, HIGH); // Set pin 3 to HIGH
  } else if (xValue > 525) {
    Serial.println("Alo");
    digitalWrite(2, LOW);  // Ensure pin 2 is LOW
    digitalWrite(3, HIGH); // Set pin 3 to HIGH
  } else {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
  }

  if (yValue < 490) {
    servo.writeMicroseconds(1650);
  } else if (yValue > 523) {
    servo.writeMicroseconds(1390);
  } else {
    servo.writeMicroseconds(1500);
  }
}
