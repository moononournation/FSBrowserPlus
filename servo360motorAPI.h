#ifdef ESP32
#include <ESP32Servo.h>
Servo servo1;
Servo servo2;
// Published values for SG90 servos; adjust if needed
int minUs = 1000;
int maxUs = 2000;
#else
// TODO
#endif

static void motorSetup()
{
#ifdef ESP32
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(1);
    servo1.setPeriodHertz(50); // Standard 50hz servo
    servo2.setPeriodHertz(50); // Standard 50hz servo
#else
// TODO
#endif
}

static void setMotor(uint8_t la, uint8_t lb, uint8_t ra, uint8_t rb)
{
#ifdef ESP32
    int lv = la + 255 - lb;
    int rv = ra + 255 - rb;
#ifdef SERVO360_REVERSE
    int lAngle = map(lv, 0, 510, 179, 0);
    int rAngle = map(rv, 0, 510, 0, 179);
#else
    int lAngle = map(lv, 0, 510, 0, 179);
    int rAngle = map(rv, 0, 510, 179, 0);
#endif
    servo1.attach(SERVO360_L_Pin, minUs, maxUs);
    servo2.attach(SERVO360_R_Pin, minUs, maxUs);
    servo1.write(lAngle);
    servo2.write(rAngle);
    Serial.printf("la: %d, lb: %d, lAngle: %d, ra: %d, rb: %d, rAngle: %d\n", la, lb, lAngle, ra, rb, rAngle);
#else
// TODO
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
