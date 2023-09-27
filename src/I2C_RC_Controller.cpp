#include "I2C_RC_Controller.h"

#define I2C_RC_Controller_CREATE_RC(index)    void I2C_RC_Controller::S##index(uint16_t _pulse_us_){motorRC_Write((index-1), _pulse_us_);}\
                                              void I2C_RC_Controller::SetS##index(uint16_t _pulseMin_0_Degree, uint16_t _pulseMax_180_Degree){infoSx[index - 1].pulseMin = _pulseMin_0_Degree;infoSx[index - 1].pulseMax = _pulseMax_180_Degree;}\
                                              void I2C_RC_Controller::writeS##index(uint16_t _degree_){uint16_t tempPulse = map(_degree_, 0, 180, infoSx[index-1].pulseMin, infoSx[index-1].pulseMax);I2C_RC_Controller::S##index(tempPulse);}\
                                              void I2C_RC_Controller::freeS##index(){I2C_RC_Controller::S##index(I2C_MOTOR_DRIVER_PULSE_DISABLE);}

// #define I2C_RC_Controller_CREATE_RC(index)    void I2C_RC_Controller::S##index##(uint16_t _pulse_us_){motorRC_Write((index-1), _pulse_us_);}\
//                                               void I2C_RC_Controller::SetS##index##(uint16_t _pulseMin_0_Degree, uint16_t _pulseMax_180_Degree){infoSx[index - 1].pulseMin = _pulseMin_0_Degree;infoSx[index - 1].pulseMax = _pulseMax_180_Degree;}\
//                                               void I2C_RC_Controller::writeS##index##(uint16_t _degree_){uint16_t tempPulse = map(_degree_, 0, 180, infoSx[index-1].pulseMin, infoSx[index-1].pulseMax);I2C_RC_Controller::S##index##(tempPulse);}\
//                                               void I2C_RC_Controller::freeS##index##(){I2C_RC_Controller::S##index##(I2C_MOTOR_DRIVER_PULSE_DISABLE);}

// #define I2C_RC_Controller_CREATE_RC(index)

I2C_RC_Controller::I2C_RC_Controller(uint8_t _address_)
{
  if(_address_ <= MAKERLABVN_I2C_DRIVER_MAX_NUMBER_DRIVER_SUPPORT){
    addressDriver = _address_ + MAKERLABVN_I2C_DRIVER_SLAVE_ADDRESS_MIN;
  }else if((_address_ >= MAKERLABVN_I2C_DRIVER_SLAVE_ADDRESS_MIN) && (_address_ <= MAKERLABVN_I2C_DRIVER_SLAVE_ADDRESS_MAX)){
    addressDriver = _address_;
  }else
  addressDriver = MAKERLABVN_I2C_DRIVER_SLAVE_ADDRESS_MIN;
}

void I2C_RC_Controller::begin()
{
  Wire.begin(); // join i2c bus (address optional for master)
  motorI2C_init();
  ReadI2cOverSerial();
}

void I2C_RC_Controller::checkSumCalculate(str_serial_data_dcMotor *_myMotor_)
{
  uint16_t tempSum = _myMotor_->addressId +
                     _myMotor_->modeId +
                     _myMotor_->index +
                     _myMotor_->pwm +
                     _myMotor_->dir;
  _myMotor_->checkSum = (uint8_t)tempSum;
}

void I2C_RC_Controller::checkSumCalculate(str_serial_data_rcMotor *_myMotor_)
{
  uint16_t tempSum = _myMotor_->addressId +
                     _myMotor_->modeId +
                     _myMotor_->index +
                     _myMotor_->value.pwm +
                     _myMotor_->value.dir;
  _myMotor_->checkSum = (uint8_t)tempSum;
}

I2C_Driver_Error_Code I2C_RC_Controller::scanI2CAddress()
{
  I2C_Driver_Error_Code tempStatus = I2C_MOTOR_DRIVER_CODE_NOT_CONNECT;
  uint8_t tempCount = 0;
  delayMicroseconds(20);
  for (byte address = MAKERLABVN_I2C_DRIVER_SLAVE_ADDRESS_MIN; address <= MAKERLABVN_I2C_DRIVER_SLAVE_ADDRESS_MAX; ++address)
  {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0)
    {
      tempCount++;
      addressDriver = address;
    }
  }
  if (tempCount == 1)
    tempStatus = I2C_MOTOR_DRIVER_CODE_SUCCESS;
  else if (tempCount > 1)
    tempStatus = I2C_MOTOR_DRIVER_CODE_MANY_CONNECT;
  return tempStatus;
}

I2C_Driver_Error_Code I2C_RC_Controller::setAddress(uint8_t _address_)
{
  I2C_Driver_Error_Code tempStatus = scanI2CAddress();
  if (tempStatus == I2C_MOTOR_DRIVER_CODE_SUCCESS)
  {
    str_serial_data_dcMotor *_myMotor_ = &motorDC[0];
    _myMotor_->modeId = SADDR_ID;
    if(_address_ <= MAKERLABVN_I2C_DRIVER_MAX_NUMBER_DRIVER_SUPPORT){
      _myMotor_->addressId = _address_ + MAKERLABVN_I2C_DRIVER_SLAVE_ADDRESS_MIN;
    }else if((_address_ >= MAKERLABVN_I2C_DRIVER_SLAVE_ADDRESS_MIN) && (_address_ <= MAKERLABVN_I2C_DRIVER_SLAVE_ADDRESS_MAX)){
      _myMotor_->addressId = _address_;
    }else return I2C_MOTOR_DRIVER_CODE_INVALID_ADDRESS;
    
    checkSumCalculate(_myMotor_);
    sendI2cAddress_Data(addressDriver, _myMotor_);
    delayMicroseconds(DELAY_I2C_SEND);

    addressDriver = _myMotor_->addressId;
    return ReadI2cOverSerial();
  }
  return tempStatus;
}

void I2C_RC_Controller::motorRC_Write(uint8_t _index_, uint16_t _pulse_)
{
  str_serial_data_rcMotor *_myMotor_ = &motorRC[_index_];
  if (_index_ < MAX_I2C_MOTOR_RC_SIZE)
  {
    _myMotor_->modeId = RC_ID;
    _myMotor_->value.pulse = _pulse_;
    checkSumCalculate(_myMotor_);
    sendI2cMotorRC_Data((str_serial_data_rcMotor *)_myMotor_);
  }
}

void I2C_RC_Controller::motorDC_Write(uint8_t _index_, uint8_t _dir_, uint8_t _speed_)
{
  if (_index_ < MAX_I2C_MOTOR_DC_SIZE)
  {
    motorDC[_index_].pwm = _speed_;
    motorDC[_index_].dir = _dir_;
    checkSumCalculate(&motorDC[_index_]);
    sendI2cMotorDC_Data(&motorDC[_index_]);
  }
}

void I2C_RC_Controller::sendI2cMotorDC_Data(str_serial_data_dcMotor *_myMotor_)
{
  delayMicroseconds(DELAY_I2C_SEND);
  Wire.beginTransmission(addressDriver); // transmit to device #8
  Wire.write(_myMotor_->addressId);      // sends one byte
  Wire.write(_myMotor_->modeId);         // sends one byte
  Wire.write(_myMotor_->index);          // sends one byte
  Wire.write(_myMotor_->pwm);            // sends one byte
  Wire.write(_myMotor_->dir);            // sends one byte
  Wire.write(_myMotor_->checkSum);       // sends one byte
  Wire.endTransmission();                // stop transmitting
}

void I2C_RC_Controller::sendI2cMotorRC_Data(str_serial_data_rcMotor *_myMotor2_)
{
  str_serial_data_rcMotor *_myMotor_ = (str_serial_data_rcMotor *)_myMotor2_;
  // ReadI2cOverSerial();
  delayMicroseconds(DELAY_I2C_SEND);
  Wire.beginTransmission(addressDriver); // transmit to device #8
  Wire.write(_myMotor_->addressId);      // sends one byte
  Wire.write(_myMotor_->modeId);         // sends one byte
  Wire.write(_myMotor_->index);          // sends one byte
  Wire.write(_myMotor_->value.pwm);      // sends one byte
  Wire.write(_myMotor_->value.dir);      // sends one byte
  Wire.write(_myMotor_->checkSum);       // sends one byte
  Wire.endTransmission();                // stop transmitting
}

void I2C_RC_Controller::sendI2cAddress_Data(uint8_t _address_, str_serial_data_dcMotor *_myMotor_)
{
  // ReadI2cOverSerial();
  delayMicroseconds(DELAY_I2C_SEND);
  Wire.beginTransmission(_address_); // transmit to device #8
  Wire.write(_myMotor_->addressId);  // sends one byte
  Wire.write(_myMotor_->modeId);     // sends one byte
  Wire.write(_myMotor_->index);      // sends one byte
  Wire.write(_myMotor_->pwm);        // sends one byte
  Wire.write(_myMotor_->dir);        // sends one byte
  Wire.write(_myMotor_->checkSum);   // sends one byte
  Wire.endTransmission();            // stop transmitting
}

I2C_Driver_Error_Code I2C_RC_Controller::ReadI2cOverSerial()
{
  I2C_Driver_Error_Code tempStatus = I2C_MOTOR_DRIVER_CODE_FAIL;
  delay(DELAY_I2C_SEND);

  // clear I2C Bufer
  while (Wire.available()){
    unsigned char c = Wire.read(); // receive a byte as character
  }

  Wire.requestFrom(addressDriver, 6); // request 6 bytes from slave device #8
  while (Wire.available())
  {                                // slave may send less than requested
    // Serial.print(Wire.available()); Serial.print("\t");
    unsigned char c = Wire.read(); // receive a byte as character
    // Serial.print(c); Serial.print("\t");
    // Serial.println(addressDriver);
    if (c == addressDriver)
      tempStatus = I2C_MOTOR_DRIVER_CODE_SUCCESS;
  }
  return tempStatus;
}

I2C_RC_Controller_CREATE_RC(1)
I2C_RC_Controller_CREATE_RC(2)
I2C_RC_Controller_CREATE_RC(3)
I2C_RC_Controller_CREATE_RC(4)
I2C_RC_Controller_CREATE_RC(5)
I2C_RC_Controller_CREATE_RC(6)
I2C_RC_Controller_CREATE_RC(7)

void I2C_RC_Controller::MA(bool _dir_, uint8_t _PWMspeed_)
{
  motorDC_Write(MAKERLABVN_I2C_DRIVER_INDEX_MA, _dir_, _PWMspeed_);
}

void I2C_RC_Controller::MB(bool _dir_, uint8_t _PWMspeed_)
{
  motorDC_Write(MAKERLABVN_I2C_DRIVER_INDEX_MB, _dir_, _PWMspeed_);
}

void I2C_RC_Controller::writeMA(bool _dir_, uint8_t _PercentSpeed_)
{
  uint8_t tempPercentSpeed = constrain(_PercentSpeed_,0,100);
  uint8_t tempPwm = map(tempPercentSpeed,0,100,0,255);
  I2C_RC_Controller::MA(_dir_, tempPwm);
}

void I2C_RC_Controller::writeMB(bool _dir_, uint8_t _PercentSpeed_)
{
  uint8_t tempPercentSpeed = constrain(_PercentSpeed_,0,100);
  uint8_t tempPwm = map(tempPercentSpeed,0,100,0,255);
  I2C_RC_Controller::MB(_dir_, tempPwm);
}

void I2C_RC_Controller::disable()
{
    motorDC[0].modeId = PWR_ID;
    motorDC[0].dir = 0;
    checkSumCalculate(&motorDC[0]);
    sendI2cMotorDC_Data(&motorDC[0]);
}

void I2C_RC_Controller::enable()
{
    motorDC[0].modeId = PWR_ID;
    motorDC[0].dir = 1;
    motorDC[0].pwm = 1;
    checkSumCalculate(&motorDC[0]);
    sendI2cMotorDC_Data(&motorDC[0]);
}

// void I2C_RC_Controller::S1(uint16_t _pulse_us_)
// {
//   motorRC_Write(MAKERLABVN_I2C_DRIVER_INDEX_S1, _pulse_us_);
// }

// void I2C_RC_Controller::S2(uint16_t _pulse_us_)
// {
//   motorRC_Write(MAKERLABVN_I2C_DRIVER_INDEX_S2, _pulse_us_);
// }

// void I2C_RC_Controller::SetS1(uint16_t _pulseMin_0_Degree, uint16_t _pulseMax_180_Degree)
// {
//   infoS1.pulseMin = _pulseMin_0_Degree;
//   infoS1.pulseMax = _pulseMax_180_Degree;
// }

// void I2C_RC_Controller::SetS2(uint16_t _pulseMin_0_Degree, uint16_t _pulseMax_180_Degree)
// {
//   infoS2.pulseMin = _pulseMin_0_Degree;
//   infoS2.pulseMax = _pulseMax_180_Degree;
// }

// void I2C_RC_Controller::writeS1(uint16_t _degree_)
// {
//   uint16_t tempPulse = map(_degree_, 0, 180, infoS1.pulseMin, infoS1.pulseMax);
//   I2C_RC_Controller::S1(tempPulse);
// }

// void I2C_RC_Controller::writeS2(uint16_t _degree_)
// {
//   uint16_t tempPulse = map(_degree_, 0, 180, infoS2.pulseMin, infoS2.pulseMax);
//   I2C_RC_Controller::S2(tempPulse);
// }

// void I2C_RC_Controller::freeS1(){
//   I2C_RC_Controller::S1(I2C_MOTOR_DRIVER_PULSE_DISABLE);
// }

// void I2C_RC_Controller::freeS2(){
//   I2C_RC_Controller::S2(I2C_MOTOR_DRIVER_PULSE_DISABLE);
// }