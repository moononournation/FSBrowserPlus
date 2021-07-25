/*
  fsbrowserPlus

  Depends library:
  https://github.com/me-no-dev/ESPAsyncWebServer.git
  https://github.com/me-no-dev/AsyncTCP.git

  upload the contents of the data folder with:
  Option 1: "ESP32 Sketch Data Upload" in Tools menu in Arduino IDE
  source: https://github.com/lorol/arduino-esp32fs-plugin
  Option 2: upload the file contents with curl command
  CD to data folder and run command: for file in `ls -A1`; do curl -F "file=@$PWD/$file" fsbrowserPlus.local/edit; done

  Access the home page: http://fsbrowserplus.local
  A simple web editor: http://fsbrowserplus.local/edit
  Camera Setting: http://fsbrowserplus.local/ov2640.htm
  Camera Snap: http://fsbrowserplus.local/snap
  Camera Stream: http://fsbrowserplus.local/stream
  WebSocket Tester: http://fsbrowserplus.local/wstester.htm
  Robot Cat Pose Design: http://fsbrowserplus.local/pose.htm
  Graph Demo: http://fsbrowserplus.local/graphs.htm
  Touch Remote: http://fsbrowserplus.local/touchremote.htm
  Camera Robot: http://fsbrowserplus.local/camerarobot.htm
*/

#include <ESPAsyncWebServer.h>

// Select camera model
// #define CAMERA_MODEL_WROVER_KIT // Has PSRAM
// #define CAMERA_MODEL_ESP_EYE // Has PSRAM
// #define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
// #define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
// #define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
// #define CAMERA_MODEL_ESP32_CAM // Has PSRAM
// #define CAMERA_MODEL_ESP32_CAM_ROBOT // Has PSRAM
// #define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
// #define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
// #define CAMERA_MODEL_JSZWY_CYIS
#include "cameraAPI.h"

const char *ssid = "YourAP";
const char *password = "PleaseInputYourPasswordHere";
const char *hostname = "fsbrowserplus";
const char *apPassword = "PleaseInputYourPasswordHere";
const char *httpEditUserName = "admin";
const char *httpEditPassword = "PleaseInputYourPasswordHere";
const uint8_t analogPin = 32;
#ifdef CAMERA
const uint8_t digitalInputList[] = { 0 };
#else
const uint8_t digitalInputList[] = {0, 2, 5, 27};
#endif

#include <ArduinoOTA.h>
#include <DNSServer.h>
#ifdef ESP32
#include <FS.h>
#include <FFat.h>
#include <SPIFFS.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#endif
#include <SPIFFSEditor.h>
#include "CaptiveRequestHandler.h"

#define GPIOCMD "GPIO:"
#define MOTORCMD "MOTOR:"
#define PCA9685CMD "PCA9685:"
#define PCA9685ALLCMD "PCA9685ALL:"

#include "gpioAPI.h"

#ifdef MOTOR
#include "motorAPI.h"
#endif

#ifndef I2C_SDA_NUM // defined according to camera model
#define I2C_SDA_NUM 21
#define I2C_SCL_NUM 22
#endif
#define PCA9685ADDRESS 0x40
#include "pca9685API.h"

char strBuf[1024];

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");

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

//format bytes
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
      //the whole message is in a single frame and we got all of it's data
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
#ifdef MOTOR
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
      //message is comprised of multiple frames or the frame is split into multiple packets
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

  Wire.begin(I2C_SDA_NUM, I2C_SCL_NUM);
  gpioSetup();
#ifdef MOTOR
  motorSetup();
#endif
  pca9685Setup();
#ifdef ESP32
  // if (SPIFFS.begin())
  if (FFat.begin())
#elif defined(ESP8266)
  if (SPIFFS.begin())
#endif
  {
#ifdef ESP32
    // File root = SPIFFS.open("/");
    File root = FFat.open("/");
#elif defined(ESP8266)
    File root = SPIFFS.open("/");
#endif
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
  WiFi.softAP(hostname, apPassword);
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); //only when requested from AP

  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.printf("STA: Failed!\n");
    WiFi.disconnect(false);
    delay(1000);
    WiFi.begin(ssid, password);
  }
  else
  {
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }

  //Send OTA events to the browser
  ArduinoOTA.onStart([]() { events.send("Update Start", "ota"); });
  ArduinoOTA.onEnd([]() { events.send("Update End", "ota"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    char p[32];
    sprintf(p, "Progress: %u%%\n", (progress / (total / 100)));
    events.send(p, "ota");
  });
  ArduinoOTA.onError([](ota_error_t error) {
    if (error == OTA_AUTH_ERROR)
      events.send("Auth Failed", "ota");
    else if (error == OTA_BEGIN_ERROR)
      events.send("Begin Failed", "ota");
    else if (error == OTA_CONNECT_ERROR)
      events.send("Connect Failed", "ota");
    else if (error == OTA_RECEIVE_ERROR)
      events.send("Recieve Failed", "ota");
    else if (error == OTA_END_ERROR)
      events.send("End Failed", "ota");
  });
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.begin();

  MDNS.addService("http", "tcp", 80);

#ifdef ESP32
  // SPIFFS.begin();
  FFat.begin();
#elif defined(ESP8266)
  SPIFFS.begin();
#endif

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  events.onConnect([](AsyncEventSourceClient *client) {
    client->send("hello!", NULL, millis(), 1000);
  });
  server.addHandler(&events);

#ifdef ESP32
  // server.addHandler(new SPIFFSEditor(SPIFFS, httpEditUserName, httpEditPassword));
  server.addHandler(new SPIFFSEditor(FFat, httpEditUserName, httpEditPassword));
#elif defined(ESP8266)
  server.addHandler(new SPIFFSEditor(httpEditUserName, httpEditPassword));
#endif

  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

#ifdef ESP32
  // server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");
  server.serveStatic("/", FFat, "/").setDefaultFile("index.htm");
#elif defined(ESP8266)
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");
#endif

  server.onNotFound([](AsyncWebServerRequest *request) {
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

    request->send(404);
  });
  server.onFileUpload([](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {
    if (!index)
      Serial.printf("UploadStart: %s\n", filename.c_str());
    Serial.printf("%s", (const char *)data);
    if (final)
      Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
  });
  server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    if (!index)
      Serial.printf("BodyStart: %u\n", total);
    Serial.printf("%s", (const char *)data);
    if (index + len == total)
      Serial.printf("BodyEnd: %u\n", total);
  });

#ifdef CAMERA
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/snap", HTTP_GET, handleSnap);
  server.on("/stream", HTTP_GET, handleStream);
  // e.g. http://fsbrowserplus.local/control?var=brightness&val=2
  server.on("/control", HTTP_GET, handleControl);
#endif

  //get heap status, analog input value and all GPIO statuses in one json call
  server.on("/all", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = handleReadGPIO();
    request->send(200, "text/json", json);
  });

  // set GPIO value
  // e.g. http://fsbrowserplus.local/gpio?pin=4&val=1
  server.on("/gpio", HTTP_GET, handleGPIO);

#ifdef MOTOR
  // set Motor value
  // e.g. http://fsbrowserplus.local/motor?la=127&lb=0&ra=127&rb=0
  server.on("/motor", HTTP_GET, handleMotor);
#endif

  // set pca9685 value
  // e.g. http://fsbrowserplus.local/pca9685?ch=0&val=360
  server.on("/pca9685", HTTP_GET, handleSetChannel);

  server.begin();
  Serial.println("ESP Async Web Server started.");
}

void loop()
{
  ArduinoOTA.handle();
  dnsServer.processNextRequest();
  ws.cleanupClients();
}
