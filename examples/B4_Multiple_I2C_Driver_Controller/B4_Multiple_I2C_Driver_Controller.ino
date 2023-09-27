#include "I2C_RC_Controller.h"

I2C_RC_Controller myDriver;

// Declare the second Driver with ID 1
I2C_RC_Controller myDriver2(1);  // To set the ID please use Example "B3_I2C_Driver_Set_ID"

void setup() {
  // put your setup code here, to run once:
  myDriver.begin();
  myDriver2.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  myDriver.writeS1(0);
  myDriver2.writeS1(0);
  delay(3000);

  myDriver.writeS1(90);
  myDriver2.writeS1(180);
  delay(3000);

  myDriver.writeS1(180);
  delay(3000);  
}
