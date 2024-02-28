//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ezButton.h>


//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
#define SW_PIN_LEFT   2  // Arduino pin connected to SW  pin
#define SW_PIN_RIGHT   3  // Arduino pin connected to SW  pin


ezButton buttonLeft(SW_PIN_LEFT);
ezButton buttonRight(SW_PIN_RIGHT);


int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int bValueLeft = 0; // To store value of the button
int bValueRight = 0; // To store value of the button


//address through which two modules communicate.
const byte address[6] = "00001";

struct TrainReplyStruct {
  int x;
  int y; // M or S  in Auto mode S signals that the move is complete
  int isPresedLeft;
  int isPresedRight;
};


void setup()
{
    Serial.begin(9600);

  radio.begin();

  bool check = radio.isChipConnected();
  Serial.print("check-");
  Serial.println(check);

  //set the address
  radio.openWritingPipe(address);

  //Set module as transmitter
  radio.stopListening();

  buttonLeft.setDebounceTime(50); // set debounce time to 50 milliseconds
  buttonRight.setDebounceTime(50); // set debounce time to 50 milliseconds
}


void loop()
{




  // стик

  buttonLeft.loop(); // MUST call the loop() function first
  buttonRight.loop(); // MUST call the loop() function first

  // read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  // Read the button value
  bValueLeft = buttonLeft.getState();
  bValueRight = buttonRight.getState();


  if (buttonLeft.isPressed()) {
    Serial.println("The left button is pressed");
    // TODO do something here
  }

  if (buttonRight.isPressed()) {
    Serial.println("The right button is pressed");
    // TODO do something here
  }

  // print data to Serial Monitor on Arduino IDE
  Serial.print("x = ");
  Serial.print(xValue);
  Serial.print(", y = ");
  Serial.print(yValue);
  Serial.println(" ");

  TrainReplyStruct struckForTheCar;
  struckForTheCar.x = xValue;
  struckForTheCar.y = yValue;
  struckForTheCar.isPresedLeft = bValueLeft;
  struckForTheCar.isPresedRight = bValueRight;


  radio.write(&struckForTheCar, sizeof(struckForTheCar));
  delay(10);
}
