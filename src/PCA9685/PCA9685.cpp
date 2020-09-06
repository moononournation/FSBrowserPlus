/**
* PCA9685.cpp - Arduino library to control PCA9685 chip.
*/
#include "PCA9685.h"

PCA9685::PCA9685(uint8_t address)
{
  _address = address;
}

/**
* I2C.begin for I2C.
*/
void PCA9685::begin()
{
  Wire.begin();
  restart();
}

/**
* Wake PCA9685 oscillator and enable auto increment
*/
void PCA9685::restart()
{
  // wake
  wake();

  // go to sleep so we can restart
  sleep();

 /** 
  * Restart and set Mode1 register to our prefered mode:
  * Restart         : bit 7 = 1 - Will revert to 0 after restart
  * Internal Clock  : bit 6 = 0
  * Auto Increment  : bit 5 = 1
  * Normal Mode     : bit 4 = 0
  * SUB1 Disabled   : bit 3 = 0
  * SUB2 Disabled   : bit 2 = 0
  * SUB3 Disabled   : bit 1 = 0
  * ALLCALL Enabled : bit 0 = 1
  *
  * B10100001 == 0xA1
  */
  _write8bits(PCA9685_MODE1, 0xA1);

  // delay at least 500 us to wake 
  delay(1);

 /**
  * Set Mode2 register to our prefered mode:
  * Reserved                        : bits 7 through 5 are reserved
  * Output logic state not inverted : bit 4 = 0
  * Outputs change on STOP command  : bit 3 = 0
  * All outputs are configured with
  * a totem pole structure          : bit 2 = 1
  * When OE = 1 (output drivers not
  * enabled), LEDn = 0              : bit 1 & 0 = 00
  *
  * B00000100 == 0x04
  */
  _write8bits(PCA9685_MODE2, 0x04);
}

void PCA9685::setInvertedLogicMode()
{
  // get current mode2
  uint8_t mode2 = _read8bits(PCA9685_MODE2);

  // set inverted logic mode by setting bit 4 to 1
  bitSet(mode2, 4);

  _write8bits(PCA9685_MODE2, mode2);
}

void PCA9685::setNormalLogicMode()
{
  // get current mode2
  uint8_t mode2 = _read8bits(PCA9685_MODE2);

  // set inverted logic mode by setting bit 4 to 0
  bitClear(mode2, 4);

  _write8bits(PCA9685_MODE2, mode2);
}

//
//void PCA9685::printBin(uint8_t bin)
//{
//  Serial.print("B");
//  for (int8_t i = 7; i >= 0; i--)
//  {
//    Serial.print(bitRead(bin, i));
//  }
//  Serial.println();
//}

void PCA9685::wake()
{
  // get current mode1
  uint8_t mode1 = _read8bits(PCA9685_MODE1);

  // set normal mode by setting bit 4 to 0
  bitClear(mode1, 4);

  // wake
  _write8bits(PCA9685_MODE1, mode1);

  // delay at least 500 us 
  delay(1);
}

void PCA9685::sleep()
{
  // get current mode1
  uint8_t mode1 = _read8bits(PCA9685_MODE1);

  // set sleep mode by setting bit 4 to 1
  bitSet(mode1, 4);

  // go to sleep
  _write8bits(PCA9685_MODE1, mode1);

  // delay at least 500 us 
  delay(1);
}

/**
* Set PWM Frequency 40-1000Hz, Default 200Hz
*/
void PCA9685::setFrequency(uint16_t freq)
{
  uint8_t prescale = round(((float)25000000 / (float)(freq * (long)4096))) - 1;
  _setPreScale(prescale);
}

/**
* Get PWM Frequency
*/
uint16_t PCA9685::getFrequency()
{
  uint8_t prescale;
  Wire.beginTransmission((uint8_t)_address);
  Wire.write((uint8_t)PCA9685_PRESCALE);
  Wire.endTransmission();
  Wire.requestFrom(_address, (uint8_t)1);
  if (Wire.available())
  {
    prescale = Wire.read();
  }

  return round((float)25000000 / (float)(((long)prescale + 1) * (long)4096));
}

/**
* PCA9685 PWM set frequency prescale
*/
void PCA9685::_setPreScale(uint8_t prescale)
{
  // must be in sleep mode to set the prescaler
  sleep();
  // set the prescaler
  _write8bits(PCA9685_PRESCALE, prescale);
  wake();
}

uint8_t PCA9685::_read8bits(uint8_t addr) {
  Wire.beginTransmission((uint8_t)_address);
  Wire.write((uint8_t)addr);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)_address, (uint8_t)1);
  return Wire.read();
}

void PCA9685::_write8bits(uint8_t reg, uint8_t value) {
  Wire.beginTransmission((uint8_t)_address);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  Wire.endTransmission();
}

/**
* Get the current 12 bit PWM value
*/
uint16_t PCA9685::getChannel(uint8_t channel)
{
  uint16_t level = 0;
  channel = (channel * 4) + 6;
  Wire.beginTransmission((uint8_t)_address);
  Wire.write((uint8_t)channel);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)_address, (uint8_t)PCA9685_SUBADR3);
  while (Wire.available()){
    uint8_t onLow = Wire.read();
    uint8_t onHi = Wire.read();
    uint8_t offLow = Wire.read();
    uint8_t offHi = Wire.read();
    level = (word((offHi & 0x0F), offLow) - word((onHi & 0x0F), onLow));
  }
  return level;
}

/*
Send PWM only OFF to channel, PWM start at 0. auto-incremental.
40% faster than single write and channel changes together.
*/
void PCA9685::setChannelsValues(uint8_t start, uint8_t count, uint16_t* values)
{
  Wire.beginTransmission(_address);
  byte startcmd = start * 4 + 6;
  Wire.write((uint8_t)startcmd);

  // start from channel 0 ON
  for (uint8_t ch = 0; ch < count; ch++)
  {
    Wire.write((uint8_t)0x00); // set all ON time to 0
    Wire.write((uint8_t)0x00);
    Wire.write(lowByte(values[ch])); // set OFF according to value
    Wire.write(highByte(values[ch]));
  }
  Wire.endTransmission();
}

/**
* Set multiple channels to the same 12bit value
*/
void PCA9685::setChannels(uint8_t start, uint8_t count, uint16_t value)
{
  uint16_t newvals[count];
  memset(newvals, 0, count * 2);

  for (uint8_t ch = 0; ch < count; ch++)
  {
    newvals[ch] = value;
  }

  setChannelsValues(start, count, newvals);
}

/**
* Single channel 12bit PWM only.  Slow in loop
*/
void PCA9685::setChannel(uint8_t channel, uint16_t value)
{
  setChannels(channel, 1, value);
}

/**
* Set multiple channels to the same 8bit value
*/
void PCA9685::setChannels8bit(uint8_t start, uint8_t count, uint8_t value)
{
  setChannels(start, count, CIEL_8_12(value));
}

/**
* Single channel 8bit PWM only.  Slow in loop
*/
void PCA9685::setChannel8bit(uint8_t channel, uint8_t value)
{
  setChannels(channel, 1, CIEL_8_12(value));
}

/*
Default 8 bit input and 12 bit sRGB CIE lab corrected PWM output
*/
RGBLevels PCA9685::getRGB(uint8_t red, uint8_t green, uint8_t blue)
{
  return RGBLevels(CIEL_8_12(blue), CIEL_8_12(green), CIEL_8_12(red));
}

/*
10 bit input and 12 bit sRGB CIE lab corrected PWM output
*/
RGBLevels PCA9685::goToRGBHI(uint16_t red, uint16_t green, uint16_t blue)
{
  return RGBLevels(CIEL_10_12(blue), CIEL_10_12(green), CIEL_10_12(red));
}

