/*
  fsbrowserPlus

  Depends library:
  https://github.com/arjenhiemstra/ESPAsyncWebServer.git
  https://github.com/me-no-dev/AsyncTCP.git

  upload the contents of the data folder with:
  Option 1: "ESP32 Sketch Data Upload" in Tools menu in Arduino IDE
  source: https://github.com/lorol/arduino-esp32fs-plugin
  Option 2: upload the file contents with curl command
  CD to data folder and run command: for file in `ls -A1`; do curl -F "file=@$PWD/$file" fsbrowserPlus.local/edit; done

  Access the home page: http://fsbrowserplus.local
  A simple web editor: http://fsbrowserplus.local/edit
  OV2640 Camera Setting: http://fsbrowserplus.local/ov2640.htm
  OV3660 Camera Setting: http://fsbrowserplus.local/ov3660.htm
  OV5640 Camera Setting: http://fsbrowserplus.local/ov5640.htm
  Camera Snap: http://fsbrowserplus.local/snap
  Camera Stream: http://fsbrowserplus.local:81/stream
  WebSocket Tester: http://fsbrowserplus.local/wstester.htm
  Robot Cat Pose Design: http://fsbrowserplus.local/pose.htm
  Graph Demo: http://fsbrowserplus.local/graphs.htm
  Touch Remote: http://fsbrowserplus.local/touchremote.htm
  Camera Robot: http://fsbrowserplus.local/camerarobot.htm
*/

#include <ESPAsyncWebServer.h>

// Please select camera model in camera_pins.h
#include "cameraAPI.h"

// custom motor pins
// #define MOTOR
// #define Motor_EN_Pin 1
// #define MotorL_A_Pin 26
// #define MotorL_B_Pin 25
// #define MotorR_A_Pin 13
// #define MotorR_B_Pin 14

// custom servo 360 pins
// #define SERVO360MOTOR
// #define SERVO360_L_Pin 2
// #define SERVO360_R_Pin 4

const char *ssid = "YourAP";
const char *password = "PleaseInputYourPasswordHere";
// const char *hostname = "fsbrowserplus";
// const char *apName = "FS Browser Plus";
// const char *hostname = "striderwalker";
// const char *apName = "Strider Walker";
const char *hostname = "xiaonanotank";
const char *apName = "XIAO nanotank";
// const char *apPassword = "PleaseInputYourPasswordHere";
const char *apPassword = ""; // Open WiFi
const char *httpEditUserName = "admin";
const char *httpEditPassword = "PleaseInputYourPasswordHere";
const uint8_t analogPin = 2;
#ifdef CAMERA
// #define CAPTIVERESPONSEFILE "/camerarobot.htm"
const uint8_t digitalInputList[] = {0};
void startCameraStreamServer();
#else
// #define CAPTIVERESPONSEFILE "/index.htm"
const uint8_t digitalInputList[] = {0, 2, 5, 27};
#endif

#include <ArduinoOTA.h>
#include <DNSServer.h>
#ifdef ESP32
#include <FS.h>
#include <FFat.h>
#include <LittleFS.h>
#include <SPIFFS.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#define FILESYSTEM LittleFS
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#define FILESYSTEM SPIFFS
#endif
#include <SPIFFSEditor.h>
#include "CaptiveRequestHandler.h"

#define GPIOCMD "GPIO:"
#define MOTORCMD "MOTOR:"
#define PCA9685CMD "PCA9685:"
#define PCA9685ALLCMD "PCA9685ALL:"

#include "gpioAPI.h"

#if defined(MOTOR)
#include "motorAPI.h"
#elif defined(SERVO360MOTOR)
#include "servo360motorAPI.h"
#endif

#ifndef I2C_SDA_NUM // defined according to camera model
#define I2C_SDA_NUM 21
#define I2C_SCL_NUM 22
#endif

#include <Wire.h>

#ifdef I2C_SSD1306_ADDRESS
#include "SSD1306.h"
SSD1306Wire display(I2C_SSD1306_ADDRESS, I2C_SDA_NUM, I2C_SCL_NUM, GEOMETRY_128_32);
#endif

// #define PCA9685ADDRESS 0x40
#ifdef PCA9685ADDRESS
#include "pca9685API.h"
#endif

char strBuf[1024];

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");

static String IpAddress2String(const IPAddress &ipAddress)
{
  return String(ipAddress[0]) + String(".") +
         String(ipAddress[1]) + String(".") +
         String(ipAddress[2]) + String(".") +
         String(ipAddress[3]);
}

static uint16_t hexValue(uint8_t h)
{
  if (h >= '0' && (h <= '9'))
  {
    return h - '0';
  }
  else if ((h >= 'A') && (h <= 'F'))
  {
    return 10 + h - 'A';
  }
  else if ((h >= 'a') && (h <= 'f'))
  {
    return 10 + h - 'a';
  }
  return 0;
}

// format bytes
String formatBytes(size_t bytes)
{
  if (bytes < 1024)
  {
    return String(bytes) + "B";
  }
  else if (bytes < (1024 * 1024))
  {
    return String(bytes / 1024.0) + "KB";
  }
  else if (bytes < (1024 * 1024 * 1024))
  {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
  else
  {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("ws[%s][%u] connected\n", server->url(), client->id());
    client->ping();
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
  }
  else if (type == WS_EVT_ERROR)
  {
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
  }
  else if (type == WS_EVT_PONG)
  {
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
  }
  else if (type == WS_EVT_DATA)
  {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    String msg = "";
    if (info->final && info->index == 0 && info->len == len)
    {
      // the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);

      if (info->opcode == WS_TEXT)
      {
        if (memcmp(GPIOCMD, data, sizeof(GPIOCMD) - 1) == 0)
        {
          uint8_t *p = data + sizeof(GPIOCMD) - 1;
          uint8_t gpio = *(p++) - 0x30; // ASCII code of '0'
          if (*p != ':')
          {
            gpio = (gpio * 10) + (*(p++) - 0x30);
          }
          p++; // skip seperator
          uint8_t value = *(p++) - 0x30;
          sprintf(strBuf, "[%u] GPIO command, gpio: %d, value: %d\n", client->id(), gpio, value);
          setGPIO(gpio, value);
        }
#if defined(MOTOR) || defined(SERVO360MOTOR)
        else if (memcmp(MOTORCMD, data, sizeof(MOTORCMD) - 1) == 0)
        {
          uint8_t *p = data + sizeof(MOTORCMD) - 1;
          uint8_t la = hexValue(*(p++));
          if (*p != ':')
          {
            la = (la * 16) + hexValue(*(p++));
          }
          p++; // skip seperator
          uint8_t lb = hexValue(*(p++));
          if (*p != ':')
          {
            lb = (lb * 16) + hexValue(*(p++));
          }
          p++; // skip seperator
          uint8_t ra = hexValue(*(p++));
          if (*p != ':')
          {
            ra = (ra * 16) + hexValue(*(p++));
          }
          p++; // skip seperator
          uint8_t rb = hexValue(*(p++));
          if (*p != ':')
          {
            rb = (rb * 16) + hexValue(*(p++));
          }
          sprintf(strBuf, "[%u] Motor command, la: %d, lb: %d, ra: %d, rb: %d\n", client->id(), la, lb, ra, rb);
          setMotor(la, lb, ra, rb);
        }
#endif
#if defined(PCA9685ADDRESS)
        else if (memcmp(PCA9685CMD, data, sizeof(PCA9685CMD) - 1) == 0)
        {
          uint8_t *p = data + sizeof(PCA9685CMD) - 1;
          uint8_t channel = *(p++) - 0x30; // ASCII code of '0'

          if (*p != ':')
          {
            channel = (channel * 10) + (*(p++) - 0x30);
          }
          p++; // skip seperator
          uint16_t value = atoi((char *)p);
          sprintf(strBuf, "[%u] PCA9685 command, channel: %d, value: %d\n", client->id(), channel, value);
          setChannel(channel, value);
        }
        else if (memcmp(PCA9685ALLCMD, data, sizeof(PCA9685ALLCMD) - 1) == 0)
        {
          uint8_t *p = data + sizeof(PCA9685ALLCMD) - 1;
          uint8_t delayMs = *(p++) - '0';
          while (*p != ':')
          {
            delayMs = (delayMs * 10) + (*(p++) - '0');
          }
          p++; // skip seperator
          uint16_t values[16];
          for (int i = 0; i < 16; i++)
          {
            values[i] = (hexValue(*(p++)) * 16 * 16);
            values[i] += (hexValue(*(p++)) * 16);
            values[i] += hexValue(*(p++));
          }
          sprintf(strBuf, "[%u] PCA9685 all command, delayMs: %d, values: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", client->id(), delayMs, values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8], values[9], values[10], values[11], values[12], values[13], values[14], values[15]);
          batchSetChannel(delayMs, values);
        }
#endif
        else
        {
          sprintf(strBuf, "[%u] get Text: %s\n", client->id(), data);
        }

        // send message to client
        client->printf(strBuf);
        Serial.print(strBuf);
      }
      else // binary
      {
        char buff[3];
        for (size_t i = 0; i < info->len; i++)
        {
          sprintf(buff, "%02x ", (uint8_t)data[i]);
          msg += buff;
        }
        Serial.printf("%s\n", msg.c_str());
        client->binary("I got your binary message");
      }
    }
    else
    {
      // message is comprised of multiple frames or the frame is split into multiple packets
      if (info->index == 0)
      {
        if (info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);

      if (info->opcode == WS_TEXT)
      {
        for (size_t i = 0; i < len; i++)
        {
          msg += (char)data[i];
        }
      }
      else
      {
        char buff[3];
        for (size_t i = 0; i < len; i++)
        {
          sprintf(buff, "%02x ", (uint8_t)data[i]);
          msg += buff;
        }
      }
      Serial.printf("%s\n", msg.c_str());

      if ((info->index + len) == info->len)
      {
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if (info->final)
        {
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
          if (info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  Serial.printf("Wire.begin(%d, %d);\n", I2C_SDA_NUM, I2C_SCL_NUM);
  Wire.begin(I2C_SDA_NUM, I2C_SCL_NUM);
#ifdef I2C_SSD1306_ADDRESS
  Serial.println("display.init();");
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(128 / 2, 0, apName);
  display.display();
#endif

  //  Serial.println("gpioSetup();");
  //  gpioSetup();

#if defined(MOTOR) || defined(SERVO360MOTOR)
  Serial.println("motorSetup();");
  motorSetup();
#endif

#ifdef PCA9685ADDRESS
  Serial.println("pca9685Setup();");
  pca9685Setup();
#endif

  Serial.println("Init filesystem");
  if (FILESYSTEM.begin())
  {
    File root = FILESYSTEM.open("/");
    File file = root.openNextFile();
    while (file)
    {
      String fileName = file.name();
      size_t fileSize = file.size();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
      file = root.openNextFile();
    }
    Serial.printf("\n");
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(apName, apPassword);
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP

  Serial.printf("Connect WiFi\n");
  WiFi.begin(ssid, password);
  delay(1000);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.printf("Retry WiFi connection\n");
    WiFi.disconnect(false);
    delay(1000);
    WiFi.begin(ssid, password);
    delay(3000);
  }

  if (WiFi.waitForConnectResult() == WL_CONNECTED)
  {
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
#ifdef I2C_SSD1306_ADDRESS
    display.drawString(128 / 2, 16, IpAddress2String(WiFi.localIP()));
    display.display();
#endif
  }

  // Send OTA events to the browser
  ArduinoOTA.onStart([]()
                     { events.send("Update Start", "ota"); });
  ArduinoOTA.onEnd([]()
                   { events.send("Update End", "ota"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        {
                          char p[32];
                          sprintf(p, "Progress: %u%%\n", (progress / (total / 100)));
                          events.send(p, "ota"); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
                       if (error == OTA_AUTH_ERROR)
                         events.send("Auth Failed", "ota");
                       else if (error == OTA_BEGIN_ERROR)
                         events.send("Begin Failed", "ota");
                       else if (error == OTA_CONNECT_ERROR)
                         events.send("Connect Failed", "ota");
                       else if (error == OTA_RECEIVE_ERROR)
                         events.send("Recieve Failed", "ota");
                       else if (error == OTA_END_ERROR)
                         events.send("End Failed", "ota"); });
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.begin();

  MDNS.addService("http", "tcp", 80);

  FILESYSTEM.begin();

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  events.onConnect([](AsyncEventSourceClient *client)
                   { client->send("hello!", NULL, millis(), 1000); });
  server.addHandler(&events);

#ifdef ESP32
  server.addHandler(new SPIFFSEditor(FILESYSTEM, httpEditUserName, httpEditPassword));
#elif defined(ESP8266)
  server.addHandler(new SPIFFSEditor(httpEditUserName, httpEditPassword));
#endif

  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(ESP.getFreeHeap())); });

  server.onNotFound([](AsyncWebServerRequest *request)
                    {
                      Serial.printf("NOT_FOUND: ");
                      if (request->method() == HTTP_GET)
                        Serial.printf("GET");
                      else if (request->method() == HTTP_POST)
                        Serial.printf("POST");
                      else if (request->method() == HTTP_DELETE)
                        Serial.printf("DELETE");
                      else if (request->method() == HTTP_PUT)
                        Serial.printf("PUT");
                      else if (request->method() == HTTP_PATCH)
                        Serial.printf("PATCH");
                      else if (request->method() == HTTP_HEAD)
                        Serial.printf("HEAD");
                      else if (request->method() == HTTP_OPTIONS)
                        Serial.printf("OPTIONS");
                      else
                        Serial.printf("UNKNOWN");
                      Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

                      if (request->contentLength())
                      {
                        Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
                        Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
                      }

                      int headers = request->headers();
                      int i;
                      for (i = 0; i < headers; i++)
                      {
                        AsyncWebHeader *h = request->getHeader(i);
                        Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
                      }

                      int params = request->params();
                      for (i = 0; i < params; i++)
                      {
                        AsyncWebParameter *p = request->getParam(i);
                        if (p->isFile())
                        {
                          Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
                        }
                        else if (p->isPost())
                        {
                          Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
                        }
                        else
                        {
                          Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
                        }
                      }

                      request->send(404); });
  server.onFileUpload([](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
                      {
                        if (!index)
                          Serial.printf("UploadStart: %s\n", filename.c_str());
                        Serial.printf("%s", (const char *)data);
                        if (final)
                          Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len); });
  server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                       {
                         if (!index)
                           Serial.printf("BodyStart: %u\n", total);
                         Serial.printf("%s", (const char *)data);
                         if (index + len == total)
                           Serial.printf("BodyEnd: %u\n", total); });

#ifdef CAMERA
  initCamera();

  server.on("/status", HTTP_GET, handleStatus);
  server.on("/snap", HTTP_GET, handleSnap);
  // e.g. http://fsbrowserplus.local/control?var=brightness&val=2
  server.on("/control", HTTP_GET, handleControl);

  startCameraStreamServer();
#endif

  // get heap status, analog input value and all GPIO statuses in one json call
  server.on("/all", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String json = handleReadGPIO();
              request->send(200, "text/json", json); });

  // set GPIO value
  // e.g. http://fsbrowserplus.local/gpio?pin=4&val=1
  server.on("/gpio", HTTP_GET, handleGPIO);

#if defined(MOTOR) || defined(SERVO360MOTOR)
  // set Motor value
  // e.g. http://fsbrowserplus.local/motor?la=127&lb=0&ra=127&rb=0
  server.on("/motor", HTTP_GET, handleMotor);
#endif

#if defined(PCA9685ADDRESS)
  // set pca9685 value
  // e.g. http://fsbrowserplus.local/pca9685?ch=0&val=360
  server.on("/pca9685", HTTP_GET, handleSetChannel);
#endif

  server.serveStatic("/", FILESYSTEM, "/").setDefaultFile("index.htm");
  server.begin();
  Serial.println("ESP Async Web Server started.");
}

void loop()
{
  ArduinoOTA.handle();
  dnsServer.processNextRequest();
  ws.cleanupClients();
}
