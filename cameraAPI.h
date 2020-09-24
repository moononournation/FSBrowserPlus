#include "src/camera_pins.h"
#ifdef CAMERA
#include <esp_camera.h>

#define PART_BOUNDARY "123456789000000000000987654321"
static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

static void cameraSetup()
{
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    //init with high specs to pre-allocate larger buffers
    if (psramFound())
    {
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 20;
        config.fb_count = 2;
    }
    else
    {
        config.frame_size = FRAMESIZE_QVGA;
        config.jpeg_quality = 30;
        config.fb_count = 2;
    }

#if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
#endif

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        log_i("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t *s = esp_camera_sensor_get();
    //initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV3660_PID)
    {
        s->set_vflip(s, 1);       //flip it back
        s->set_brightness(s, 1);  //up the blightness just a bit
        s->set_saturation(s, -2); //lower the saturation
    }
    else
    {
        // s->set_brightness(s, 2);
        // s->set_contrast(s, 2);
        s->set_saturation(s, 2);
        s->set_aec2(s, true);
        s->set_gainceiling(s, GAINCEILING_128X);
        s->set_lenc(s, true);
    }

#if defined(CAMERA_MODEL_M5STACK_WIDE)
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
#endif
}

static void handleSnap(AsyncWebServerRequest *request)
{
    camera_fb_t *fb = NULL;

    fb = esp_camera_fb_get();
    if (!fb)
    {
        log_e("Camera capture failed");
        request->send(500);
    }

    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/jpeg", fb->buf, fb->len);
    response->addHeader("Content-Disposition", "inline; filename=capture.jpg");
    request->send(response);

    esp_camera_fb_return(fb);
}

static camera_fb_t *fb = NULL;
static uint8_t *_jpg_buf = NULL;
static size_t _jpg_buf_len = 0;
static size_t _jpg_buf_offset = 0;

static size_t streamChunkCallback(uint8_t *buffer, size_t maxLen, size_t index)
{
    // Serial.printf("streamChunkCallback, maxLen: %d\n", maxLen);
    size_t len = 0;
    if (!fb)
    {
        if (maxLen < 100)
        {
            Serial.printf("maxLen %d is too small to fit the STREAM header, send a dummy line feed instead.\n", maxLen);
            buffer[0] = '\r';
            buffer[1] = '\n';
            return 2;
        }

        fb = esp_camera_fb_get();
        if (!fb)
        {
            Serial.println("Camera capture failed");
            return 0;
        }
        else
        {
            _jpg_buf = fb->buf;
            _jpg_buf_len = fb->len;
            _jpg_buf_offset = 0;
            memcpy(buffer, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
            len = strlen(_STREAM_BOUNDARY);
            len += snprintf((char *)(buffer + len), 64, _STREAM_PART, _jpg_buf_len);
            // Serial.printf("esp_camera_fb_get() %d\n", _jpg_buf_len);
        }
    }

    size_t l;
    if (maxLen < (len + (_jpg_buf_len - _jpg_buf_offset)))
    {
        l = maxLen - len;
    }
    else
    {
        l = _jpg_buf_len - _jpg_buf_offset;
    }
    memcpy(buffer + len, _jpg_buf + _jpg_buf_offset, l);
    len += l;
    _jpg_buf_offset += l;

    // Serial.printf("_jpg_buf_offset: %d, _jpg_buf_len: %d\n", _jpg_buf_offset, _jpg_buf_len);
    if (_jpg_buf_offset >= _jpg_buf_len)
    {
        if (fb)
        {
            esp_camera_fb_return(fb);
            fb = NULL;
            _jpg_buf = NULL;
        }
        else if (_jpg_buf)
        {
            free(_jpg_buf);
            _jpg_buf = NULL;
        }
    }

    // Write up to "maxLen" bytes into "buffer" and return the amount written.
    // index equals the amount of bytes that have been already sent
    // You will be asked for more data until 0 is returned
    // Keep in mind that you can not delay or yield waiting for more data!
    return len;
}

static void handleStream(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response = request->beginChunkedResponse(_STREAM_CONTENT_TYPE, streamChunkCallback);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}
#endif // CAMERA
