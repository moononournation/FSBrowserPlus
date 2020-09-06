/*
  FSWebServer - Example WebServer with FS backend for esp8266/esp32
  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the WebServer library for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  upload the contents of the data folder with MkSPIFFS Tool ("ESP32 Sketch Data Upload" in Tools menu in Arduino IDE)
  or you can upload the contents of a folder if you CD in that folder and run the following command:
  for file in `ls -A1`; do curl -F "file=@$PWD/$file" fsbrowserPlus.local/edit; done

  access the sample web page at http://fsbrowserPlus.local
  edit the page by going to http://fsbrowserPlus.local/edit
*/
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ESPmDNS.h>

#include <uri/UriBraces.h>

#define FILESYSTEM SPIFFS
// You only need to format the filesystem once
#define FORMAT_FILESYSTEM false
#define DBG_OUTPUT_PORT Serial
#include "editAPI.h"
#include "gpioAPI.h"
#include "pca9685API.h"

const char *ssid = "YourAP";
const char *password = "PleaseInputYourPasswordHere";
const char *host = "fsbrowserplus";

#define GPIOCMD "GPIO:"
#define PCA9685CMD "PCA9685:"
#define PCA9685ALLCMD "PCA9685ALL:"

char strBuf[1024];

WiFiMulti WiFiMulti;
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16)
{
  const uint8_t *src = (const uint8_t *)mem;
  DBG_OUTPUT_PORT.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for (uint32_t i = 0; i < len; i++)
  {
    if (i % cols == 0)
    {
      DBG_OUTPUT_PORT.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    DBG_OUTPUT_PORT.printf("%02X ", *src);
    src++;
  }
  DBG_OUTPUT_PORT.printf("\n");
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

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    DBG_OUTPUT_PORT.printf("[%u] Disconnected!\n", num);
    break;
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(num);
    DBG_OUTPUT_PORT.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

    // send message to client
    webSocket.sendTXT(num, "Connected");
  }
  break;
  case WStype_TEXT:
    DBG_OUTPUT_PORT.printf("[%u] get Text: %s\n", num, payload);

    if (memcmp(GPIOCMD, payload, sizeof(GPIOCMD) - 1) == 0)
    {
      uint8_t *p = payload + sizeof(GPIOCMD) - 1;
      uint8_t gpio = *(p++) - 0x30; // ASCII code of '0'
      if (*p != ':')
      {
        gpio = (gpio * 10) + (*(p++) - 0x30);
      }
      p++; // skip seperator
      uint8_t value = *(p++) - 0x30;
      sprintf(strBuf, "[%u] GPIO command, gpio: %d, value: %d\n", num, gpio, value);
      setGPIO(gpio, value);
    }
    else if (memcmp(PCA9685CMD, payload, sizeof(PCA9685CMD) - 1) == 0)
    {
      uint8_t *p = payload + sizeof(PCA9685CMD) - 1;
      uint8_t channel = *(p++) - 0x30; // ASCII code of '0'

      if (*p != ':')
      {
        channel = (channel * 10) + (*(p++) - 0x30);
      }
      p++; // skip seperator
      uint16_t value = atoi((char *)p);
      sprintf(strBuf, "[%u] PCA9685 command, channel: %d, value: %d\n", num, channel, value);
      pca9685SetChannel(channel, value);
    }
    else if (memcmp(PCA9685ALLCMD, payload, sizeof(PCA9685ALLCMD) - 1) == 0)
    {
      uint8_t *p = payload + sizeof(PCA9685ALLCMD) - 1;
      uint8_t delayMs = *(p++) - '0';
      while (*p != ':')
      {
        delayMs = (delayMs * 10) + (*(p++) - '0');
      }
      p++; // skip seperator
      uint16_t values[16];
      for (int i = 0; i < 16; i++)
      {
        values[i] = (hexValue(*(p++)) * 16 * 16) + (hexValue(*(p++)) * 16) + hexValue(*(p++));
      }
      sprintf(strBuf, "[%u] PCA9685 all command, delayMs: %d, values: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", num, delayMs, values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8], values[9], values[10], values[11], values[12], values[13], values[14], values[15]);
      pca9685All(delayMs, values);
    }
    else
    {
      sprintf(strBuf, "[%u] get Text: %s\n", num, payload);
    }

    // send message to client
    webSocket.sendTXT(num, strBuf);

    // send data to all connected clients
    // webSocket.broadcastTXT("message here");
    break;
  case WStype_BIN:
    DBG_OUTPUT_PORT.printf("[%u] get binary length: %u\n", num, length);
    hexdump(payload, length);

    // send message to client
    // webSocket.sendBIN(num, payload, length);
    break;
  case WStype_ERROR:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
    break;
  }
}

void setup(void)
{
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.print("\n");
  DBG_OUTPUT_PORT.setDebugOutput(true);

  gpioSetup();
  pca9685Setup();

  if (FORMAT_FILESYSTEM)
  {
    FILESYSTEM.format();
  }
  if (FILESYSTEM.begin())
  {
    File root = FILESYSTEM.open("/");
    File file = root.openNextFile();
    while (file)
    {
      String fileName = file.name();
      size_t fileSize = file.size();
      DBG_OUTPUT_PORT.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
      file = root.openNextFile();
    }
    DBG_OUTPUT_PORT.printf("\n");
  }

  //WIFI INIT
  DBG_OUTPUT_PORT.print("Connecting WiFi");
  WiFiMulti.addAP(ssid, password);

  while (WiFiMulti.run() != WL_CONNECTED)
  {
    delay(500);
    DBG_OUTPUT_PORT.print(".");
  }
  DBG_OUTPUT_PORT.println("");
  DBG_OUTPUT_PORT.print("Connected! IP address: ");
  DBG_OUTPUT_PORT.println(WiFi.localIP());

  MDNS.begin(host);
  DBG_OUTPUT_PORT.print("Open http://");
  DBG_OUTPUT_PORT.print(host);
  DBG_OUTPUT_PORT.println(".local/edit to see the file browser");

  //SERVER INIT
  //list directory
  server.on("/list", HTTP_GET, []() {
    handleFileList(&server);
  });
  //load editor
  server.on("/edit", HTTP_GET, []() {
    if (!handleFileRead(&server, "/edit.htm"))
    {
      server.send(404, "text/plain", "FileNotFound");
    }
  });
  //create file
  server.on("/edit", HTTP_PUT, []() {
    handleFileCreate(&server);
  });
  //delete file
  server.on("/edit", HTTP_DELETE, []() {
    handleFileDelete(&server);
  });
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  server.on(
      "/edit", HTTP_POST,
      []() {
        server.send(200, "text/plain", "");
      },
      []() {
        handleFileUpload(&server);
      });

  //called when the url is not defined here
  //use it to load content from FILESYSTEM
  server.onNotFound([]() {
    if (!handleFileRead(&server, server.uri()))
    {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  //get heap status, analog input value and all GPIO statuses in one json call
  server.on("/all", HTTP_GET, []() {
    handleReadGPIO(&server);
  });

  server.on(UriBraces("/gpio/{}/{}"), []() {
    String gpio = server.pathArg(0);
    String value = server.pathArg(1);
    server.send(200, "text/plain", "gpio:" + gpio + ",value:" + value);
    setGPIO(atoi(gpio.c_str()), atoi(value.c_str()));
  });

  server.on(UriBraces("/pca9685/{}/{}"), []() {
    String channel = server.pathArg(0);
    String value = server.pathArg(1);
    server.send(200, "text/plain", "channel:" + channel + ",value:" + value);
    pca9685SetChannel(atoi(channel.c_str()), atoi(value.c_str()));
  });

  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started.");

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  DBG_OUTPUT_PORT.println("WebSocket server started.");
}

void loop(void)
{
  server.handleClient();
  webSocket.loop();
}
