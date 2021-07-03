static uint8_t gpioMode[NUM_DIGITAL_PINS] = {0xFF};

static void setPinMode(uint8_t pin, uint8_t mode)
{
    gpioMode[pin] = mode;
    pinMode(pin, mode);
}

static void gpioSetup()
{
    setPinMode(analogPin, ANALOG);
    for (int i = 0; i < sizeof(digitalInputList); i++)
    {
        setPinMode(digitalInputList[i], INPUT);
    }
}

static void setGPIO(uint8_t pin, uint8_t value)
{
    if ((pin < NUM_DIGITAL_PINS) && (value <= 1))
    {
        if (gpioMode[pin] != OUTPUT)
        {
            setPinMode(pin, OUTPUT);
        }
        digitalWrite(pin, value);
    }
}

// TODO: per request GPIO list
static String handleReadGPIO()
{
    String json = "{";
    json += "\"heap\":" + String(ESP.getFreeHeap());
    json += ", \"analog\":" + String(analogRead(analogPin));
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

static void handleGPIO(AsyncWebServerRequest *request)
{
    String pin = request->arg("pin");
    String value = request->arg("val");

    setGPIO(atoi(pin.c_str()), atoi(value.c_str()));

    request->send(200, "text/plain", "Set GPIO " + pin + " to " + value);
}
