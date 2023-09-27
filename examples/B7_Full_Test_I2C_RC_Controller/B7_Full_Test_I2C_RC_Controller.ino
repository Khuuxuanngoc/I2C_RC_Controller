#include "I2C_RC_Controller.h"

I2C_RC_Controller myDriver(0);

char tempInput = 0;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Start Test, input from 1 to 4 (OFF)");
    Serial.println("Baud 115200, No line ending");
    myDriver.begin();
}

void loop()
{
    if (Serial.available() > 0)
    {
        tempInput = Serial.read();
        Serial.write(tempInput);
    }

    if (tempInput == '1')
    {
        myDriver.writeS1(45);
        myDriver.writeS2(90);
        myDriver.writeS3(90);
        myDriver.writeS4(90);
        myDriver.writeS5(90);
        myDriver.writeS6(90);
        myDriver.writeS7(90);
//        myDriver.writeMA(1, 50);
//        myDriver.writeMB(1, 50);
    }
    else if (tempInput == '2')
    {
//        myDriver.writeS1(90);
//        myDriver.writeS2(90);
//        myDriver.writeMA(1, 0);
//        myDriver.writeMB(1, 0);
        myDriver.writeS1(45);
        myDriver.writeS2(45);
        myDriver.writeS3(90);
        myDriver.writeS4(90);
        myDriver.writeS5(90);
        myDriver.writeS6(90);
        myDriver.writeS7(90);
    }
    else if (tempInput == '3')
    {
//        myDriver.writeS1(135);
//        myDriver.writeS2(45);
//        myDriver.writeMA(0, 100);
//        myDriver.writeMB(0, 100);
        myDriver.writeS1(45);
        myDriver.writeS2(45);
        myDriver.writeS3(45);
        myDriver.writeS4(90);
        myDriver.writeS5(90);
        myDriver.writeS6(90);
        myDriver.writeS7(90);
    }
    else if (tempInput == '4')
    {
//        myDriver.writeS1(0);
//        myDriver.writeS2(0);
//        myDriver.writeMA(0, 0);
//        myDriver.writeMB(0, 0);
        myDriver.writeS1(45);
        myDriver.writeS2(45);
        myDriver.writeS3(45);
        myDriver.writeS4(45);
        myDriver.writeS5(90);
        myDriver.writeS6(90);
        myDriver.writeS7(90);
    }
    else if (tempInput == '5')
    {
//        myDriver.writeS1(0);
//        myDriver.writeS2(0);
//        myDriver.writeMA(0, 0);
//        myDriver.writeMB(0, 0);
        myDriver.writeS1(45);
        myDriver.writeS2(45);
        myDriver.writeS3(45);
        myDriver.writeS4(45);
        myDriver.writeS5(45);
        myDriver.writeS6(90);
        myDriver.writeS7(90);
    }
    else if (tempInput == '6')
    {
//        myDriver.writeS1(0);
//        myDriver.writeS2(0);
//        myDriver.writeMA(0, 0);
//        myDriver.writeMB(0, 0);
        myDriver.writeS1(45);
        myDriver.writeS2(45);
        myDriver.writeS3(45);
        myDriver.writeS4(45);
        myDriver.writeS5(45);
        myDriver.writeS6(45);
        myDriver.writeS7(90);
    }
    else if (tempInput == '7')
    {
//        myDriver.writeS1(0);
//        myDriver.writeS2(0);
//        myDriver.writeMA(0, 0);
//        myDriver.writeMB(0, 0);
        myDriver.writeS1(45);
        myDriver.writeS2(45);
        myDriver.writeS3(45);
        myDriver.writeS4(45);
        myDriver.writeS5(45);
        myDriver.writeS6(45);
        myDriver.writeS7(45);
    }
    else if (tempInput == '8')
    {
//        myDriver.writeS1(0);
//        myDriver.writeS2(0);
//        myDriver.writeMA(0, 0);
//        myDriver.writeMB(0, 0);
        myDriver.writeS1(90);
        myDriver.writeS2(90);
        myDriver.writeS3(90);
        myDriver.writeS4(90);
        myDriver.writeS5(90);
        myDriver.writeS6(90);
        myDriver.writeS7(90);
    }

    else if (tempInput == '9')
    {
//        myDriver.writeS1(0);
//        myDriver.writeS2(0);
//        myDriver.writeMA(0, 0);
//        myDriver.writeMB(0, 0);

//        myDriver.freeS1();
//        myDriver.freeS2();
//        myDriver.freeS3();
//        myDriver.freeS4();
//        myDriver.S4(0);
        
//        myDriver.writeS2(0);
//        myDriver.writeS3(0);
//        myDriver.writeS4(0);
      myDriver.disable();
    }
    else if (tempInput == '0')
    {
//        myDriver.writeS1(0);
//        myDriver.writeS2(0);
//        myDriver.writeMA(0, 0);
//        myDriver.writeMB(0, 0);

//        myDriver.freeS1();
//        myDriver.freeS2();
//        myDriver.freeS3();
//        myDriver.freeS4();
//        myDriver.S4(0);
        
//        myDriver.writeS2(0);
//        myDriver.writeS3(0);
//        myDriver.writeS4(0);
      myDriver.enable();
    }
    tempInput = 0;
}
