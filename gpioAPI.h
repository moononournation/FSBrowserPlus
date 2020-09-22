static uint8_t gpioMode[40] = {};

static void setPinMode(uint8_t pin, uint8_t mode)
{
    gpioMode[pin] = mode;
    pinMode(pin, mode);
}

static void gpioSetup()
{
    setPinMode(0, INPUT);
    setPinMode(2, INPUT);
    setPinMode(5, INPUT);
    setPinMode(27, INPUT);
    setPinMode(32, INPUT);
}

static void setGPIO(uint8_t gpio, uint8_t value)
{
    if ((gpio < 34) && (value >= 0) && (value <= 1))
    {
        if (gpioMode[gpio] != OUTPUT)
        {
            setPinMode(gpio, OUTPUT);
        }
        digitalWrite(gpio, value);
    }
}

// TODO: per request GPIO list
static String handleReadGPIO()
{
    String json = "{";
    json += "\"heap\":" + String(ESP.getFreeHeap());
    json += ", \"analog\":" + String(analogRead(32));
    int32_t v = 0;
    for (int i = 0; i < 32; i++)
    {
        if (gpioMode[i] && INPUT)
        {
            v |= (digitalRead(i) << i);
        }
    }
    json += ", \"gpio\":" + String(v);
    json += "}";

    return json;
}
