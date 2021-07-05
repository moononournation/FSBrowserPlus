static void motorSetup()
{
#ifdef ESP32
    ledcSetup(1 /* LEDChannel */, 5000 /* freq */, 8 /* resolution */);
    ledcAttachPin(MotorL_A_Pin, 1 /* LEDChannel */);
    ledcWrite(1 /* LEDChannel */, 0); /* 0-255 */
    ledcSetup(2 /* LEDChannel */, 5000 /* freq */, 8 /* resolution */);
    ledcAttachPin(MotorL_B_Pin, 2 /* LEDChannel */);
    ledcWrite(2 /* LEDChannel */, 0); /* 0-255 */
    ledcSetup(3 /* LEDChannel */, 5000 /* freq */, 8 /* resolution */);
    ledcAttachPin(MotorR_A_Pin, 3 /* LEDChannel */);
    ledcWrite(3 /* LEDChannel */, 0); /* 0-255 */
    ledcSetup(4 /* LEDChannel */, 5000 /* freq */, 8 /* resolution */);
    ledcAttachPin(MotorR_B_Pin, 4 /* LEDChannel */);
    ledcWrite(4 /* LEDChannel */, 0); /* 0-255 */
#else
    pinMode(MotorL_A_Pin, OUTPUT);
    pinMode(MotorL_B_Pin, OUTPUT);
    pinMode(MotorR_A_Pin, OUTPUT);
    pinMode(MotorR_B_Pin, OUTPUT);
#endif
}

static void setMotor(uint8_t la, uint8_t lb, uint8_t ra, uint8_t rb)
{
#ifdef ESP32
    ledcWrite(1 /* LEDChannel */, la); /* 0-255 */
    ledcWrite(2 /* LEDChannel */, lb); /* 0-255 */
    ledcWrite(3 /* LEDChannel */, ra); /* 0-255 */
    ledcWrite(4 /* LEDChannel */, rb); /* 0-255 */
#else
    analogWrite(MotorL_A_Pin, la);
    analogWrite(MotorL_B_Pin, lb);
    analogWrite(MotorR_A_Pin, ra);
    analogWrite(MotorR_B_Pin, rb);
#endif
}

static void handleMotor(AsyncWebServerRequest *request)
{
    String la = request->arg("la");
    String lb = request->arg("lb");
    String ra = request->arg("ra");
    String rb = request->arg("rb");

    setMotor(atoi(la.c_str()), atoi(lb.c_str()), atoi(ra.c_str()), atoi(rb.c_str()));

    request->send(200, "text/plain", "Set Motor " + la + ", " + lb + ", " + ra + ", " + rb);
}
