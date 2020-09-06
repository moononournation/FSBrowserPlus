#include "src/PCA9685/PCA9685.h"

#ifndef I2C_SDA_NUM
#define I2C_SDA_NUM 12 //21
#define I2C_SCL_NUM 13 //22
#endif

static PCA9685 pca9685(0x40);

static void pca9685Off()
{
    pca9685.setChannels(0, 16, 0);
}

static void pca9685Setup()
{
    Wire.begin(I2C_SDA_NUM, I2C_SCL_NUM);
    pca9685.begin();
    pca9685.setFrequency(50);

    pca9685.setChannels(0, 16, 360);
    delay(500);
    pca9685Off();
}

static void pca9685SetChannel(uint8_t channel, uint16_t value)
{
    if ((channel < 16) && (value >= 0) && (value < 4096))
    {
        pca9685.setChannel(channel, value);
        delay(500);
        pca9685Off();
    }
}

static void pca9685All(uint16_t delayMs, uint16_t *values)
{
    pca9685.setChannelsValues(0, 16, values);
    if (delayMs > 0)
    {
        delay(delayMs);
        pca9685Off();
    }
}
