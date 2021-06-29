#include "src/PCA9685/PCA9685.h"

static PCA9685 pca9685(PCA9685ADDRESS);

static void pca9685Off()
{
    pca9685.setChannels(0, 16, 0);
}

static void pca9685Setup()
{
    pca9685.begin();
    pca9685.setFrequency(50);

    pca9685.setChannels(0, 16, 360);
    delay(500);
    pca9685Off();
}

static void setChannel(uint8_t channel, uint16_t value)
{
    if ((channel < 16) && (value < 4096))
    {
        pca9685.setChannel(channel, value);
        delay(500);
        pca9685Off();
    }
}

static void batchSetChannel(uint16_t delayMs, uint16_t *values)
{
    pca9685.setChannelsValues(0, 16, values);
    if (delayMs > 0)
    {
        delay(delayMs);
        pca9685Off();
    }
}

static void handleSetChannel(AsyncWebServerRequest *request)
{
    String channel = request->arg("ch");
    String value = request->arg("val");

    setChannel(atoi(channel.c_str()), atoi(value.c_str()));

    request->send(200, "text/plain", "Set channel " + channel + " to " + value);
}
