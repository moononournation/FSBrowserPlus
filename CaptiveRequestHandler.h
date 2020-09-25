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
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request)
  {
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->print("<!DOCTYPE html><html><head><title>Captive Portal</title></head><body>");
    response->print("<h1>Captive Portal</h1>");
    response->printf("<p>You were trying to reach: http://%s%s</p>", request->host().c_str(), request->url().c_str());
    response->printf("<h2>Try <a href='http://%s.local/index.htm'>Home Page</a></h2>", hostname);
    response->printf("<h2>Or <a href='http://%s.local/ov2640.htm'>Camera Setting Page</a></h2>", hostname);
    response->printf("<h2>Or <a href='http://%s.local/snap'>Camera Snap</a></h2>", hostname);
    response->printf("<h2>Or <a href='http://%s.local/stream'>Camera Stream</a></h2>", hostname);
    response->printf("<h2>Or <a href='http://%s.local/wstester.htm'>WebSocket Tester Page</a></h2>", hostname);
    response->printf("<h2>Or <a href='http://%s.local/pose.htm'>Pose Design Page</a></h2>", hostname);
    response->printf("<h2>Or <a href='http://%s.local/graphs.htm'>Graph Demo Page</a></h2>", hostname);
    response->print("</body></html>");
    request->send(response);
  }
};
