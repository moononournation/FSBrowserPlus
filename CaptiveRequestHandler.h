class CaptiveRequestHandler : public AsyncWebHandler
{
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request)
  {
    if (request->host().indexOf(hostname) == 0)
    {
      return false; // don't captive self hostname
    }
    else if (request->host().indexOf("192.168") == 0)
    {
      return false; // don't captive local IP access
    }
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request)
  {
#ifdef CAPTIVERESPONSEFILE
    request->send(FILESYSTEM, CAPTIVERESPONSEFILE);
#else
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->print("<!DOCTYPE html><html><head><title>Captive Portal</title><meta name='viewport' content='width=device-width, initial-scale=1'></head><body>");
    response->print("<h1>Captive Portal</h1>");
    response->print("<p>You were trying to reach:<br>");
    response->printf("http://%s%s</p>", request->host().c_str(), request->url().c_str());
    response->printf("<h2><a href='http://%s.local/edit'>Edit Page</a></h2>", hostname);
    response->printf("<h2><a href='http://%s.local/wstester.htm'>WebSocket Tester</a></h2>", hostname);
    response->printf("<h2><a href='http://%s.local/pose.htm'>Robot Cat Pose Design</a></h2>", hostname);
    response->printf("<h2><a href='http://%s.local/graphs.htm'>Graph Demo</a></h2>", hostname);
    response->printf("<h2><a href='http://%s.local/touchremote.htm'>Touch Remote</a></h2>", hostname);

#ifdef CAMERA
    sensor_t *s = esp_camera_sensor_get();
    if (s->id.PID == OV2640_PID)
    {
    response->printf("<h2><a href='http://%s.local/ov2640.htm'>OV2640 Camera Setting</a></h2>", hostname);
    }
    else if (s->id.PID == OV3660_PID)
    {
    response->printf("<h2><a href='http://%s.local/ov3660.htm'>OV3660 Camera Setting</a></h2>", hostname);
    }
    else if (s->id.PID == OV5640_PID)
    {
    response->printf("<h2><a href='http://%s.local/ov5640.htm'>OV5640 Camera Setting</a></h2>", hostname);
    }
    response->printf("<h2><a href='http://%s.local/snap'>Camera Snap</a></h2>", hostname);
    response->printf("<h2><a href='http://%s.local/camerarobot.htm'>Camera Robot</a></h2>", hostname);
#endif

    response->print("</body></html>");
    request->send(response);
#endif
  }
};
