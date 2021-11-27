#include "camera_pins.h"
#ifdef CAMERA
#include <esp_camera.h>

static void initCamera()
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

    // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
    //                      for larger pre-allocated frame buffer.
    if (psramFound())
    {
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 4;
        config.fb_count = 2;
    }
    else
    {
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

#if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
#endif

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t *s = esp_camera_sensor_get();
    // initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV3660_PID)
    {
        s->set_vflip(s, 1); // flip it back
        // s->set_brightness(s, 1); // up the brightness just a bit
        s->set_brightness(s, 0); // up the brightness just a bit
        // s->set_saturation(s, -2); // lower the saturation
        s->set_saturation(s, 3);
        s->set_sharpness(s, 3);
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

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
#endif
#if defined(CAMERA_MODEL_JSZWY_CYIS) || defined(CAMERA_MODEL_JSZWY_CYIS_2)
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
#endif
    s->set_framesize(s, FRAMESIZE_VGA);
}

static void setControl(String variable, int value)
{
    sensor_t *s = esp_camera_sensor_get();
    if (variable.startsWith("framesize"))
    {
        s->set_framesize(s, (framesize_t)value);
    }
    else if (variable.startsWith("quality"))
    {
        s->set_quality(s, value);
    }
    else if (variable.startsWith("contrast"))
    {
        s->set_contrast(s, value);
    }
    else if (variable.startsWith("brightness"))
    {
        s->set_brightness(s, value);
    }
    else if (variable.startsWith("saturation"))
    {
        s->set_saturation(s, value);
    }
    else if (variable.startsWith("gainceiling"))
    {
        s->set_gainceiling(s, (gainceiling_t)value);
    }
    else if (variable.startsWith("colorbar"))
    {
        s->set_colorbar(s, value);
    }
    else if (variable.startsWith("awb"))
    {
        s->set_whitebal(s, value);
    }
    else if (variable.startsWith("agc"))
    {
        s->set_gain_ctrl(s, value);
    }
    else if (variable.startsWith("aec"))
    {
        s->set_exposure_ctrl(s, value);
    }
    else if (variable.startsWith("hmirror"))
    {
        s->set_hmirror(s, value);
    }
    else if (variable.startsWith("vflip"))
    {
        s->set_vflip(s, value);
    }
    else if (variable.startsWith("awb_gain"))
    {
        s->set_awb_gain(s, value);
    }
    else if (variable.startsWith("agc_gain"))
    {
        s->set_agc_gain(s, value);
    }
    else if (variable.startsWith("aec_value"))
    {
        s->set_aec_value(s, value);
    }
    else if (variable.startsWith("aec2"))
    {
        s->set_aec2(s, value);
    }
    else if (variable.startsWith("dcw"))
    {
        s->set_dcw(s, value);
    }
    else if (variable.startsWith("bpc"))
    {
        s->set_bpc(s, value);
    }
    else if (variable.startsWith("wpc"))
    {
        s->set_wpc(s, value);
    }
    else if (variable.startsWith("raw_gma"))
    {
        s->set_raw_gma(s, value);
    }
    else if (variable.startsWith("lenc"))
    {
        s->set_lenc(s, value);
    }
    else if (variable.startsWith("special_effect"))
    {
        s->set_special_effect(s, value);
    }
    else if (variable.startsWith("wb_mode"))
    {
        s->set_wb_mode(s, value);
    }
    else if (variable.startsWith("ae_level"))
    {
        s->set_ae_level(s, value);
    }
}

static void handleStatus(AsyncWebServerRequest *request)
{
    static char json_response[1024];

    sensor_t *s = esp_camera_sensor_get();
    char *p = json_response;
    *p++ = '{';
    if (s->id.PID == NT99141_PID)
    {
        p += sprintf(p, "\"PID\":\"NT99141_PID\",");
    }
    else if (s->id.PID == OV9650_PID)
    {
        p += sprintf(p, "\"PID\":\"OV9650_PID\",");
    }
    else if (s->id.PID == OV7725_PID)
    {
        p += sprintf(p, "\"PID\":\"OV7725_PID\",");
    }
    else if (s->id.PID == OV2640_PID)
    {
        p += sprintf(p, "\"PID\":\"OV2640_PID\",");
    }
    else if (s->id.PID == OV3660_PID)
    {
        p += sprintf(p, "\"PID\":\"OV3660_PID\",");
    }
    else if (s->id.PID == OV5640_PID)
    {
        p += sprintf(p, "\"PID\":\"OV5640_PID\",");
    }
    else if (s->id.PID == OV7670_PID)
    {
        p += sprintf(p, "\"PID\":\"OV7670_PID\",");
    }
    p += sprintf(p, "\"framesize\":%u,", s->status.framesize);
    p += sprintf(p, "\"quality\":%u,", s->status.quality);
    p += sprintf(p, "\"brightness\":%d,", s->status.brightness);
    p += sprintf(p, "\"contrast\":%d,", s->status.contrast);
    p += sprintf(p, "\"saturation\":%d,", s->status.saturation);
    p += sprintf(p, "\"sharpness\":%d,", s->status.sharpness);
    p += sprintf(p, "\"special_effect\":%u,", s->status.special_effect);
    p += sprintf(p, "\"wb_mode\":%u,", s->status.wb_mode);
    p += sprintf(p, "\"awb\":%u,", s->status.awb);
    p += sprintf(p, "\"awb_gain\":%u,", s->status.awb_gain);
    p += sprintf(p, "\"aec\":%u,", s->status.aec);
    p += sprintf(p, "\"aec2\":%u,", s->status.aec2);
    p += sprintf(p, "\"ae_level\":%d,", s->status.ae_level);
    p += sprintf(p, "\"aec_value\":%u,", s->status.aec_value);
    p += sprintf(p, "\"agc\":%u,", s->status.agc);
    p += sprintf(p, "\"agc_gain\":%u,", s->status.agc_gain);
    p += sprintf(p, "\"gainceiling\":%u,", s->status.gainceiling);
    p += sprintf(p, "\"bpc\":%u,", s->status.bpc);
    p += sprintf(p, "\"wpc\":%u,", s->status.wpc);
    p += sprintf(p, "\"raw_gma\":%u,", s->status.raw_gma);
    p += sprintf(p, "\"lenc\":%u,", s->status.lenc);
    p += sprintf(p, "\"vflip\":%u,", s->status.vflip);
    p += sprintf(p, "\"hmirror\":%u,", s->status.hmirror);
    p += sprintf(p, "\"dcw\":%u,", s->status.dcw);
    p += sprintf(p, "\"colorbar\":%u", s->status.colorbar);
    *p++ = '}';
    *p++ = 0;
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", json_response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
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

static void handleControl(AsyncWebServerRequest *request)
{
    String variable = request->arg("var");
    String value = request->arg("val");

    setControl(variable, atoi(value.c_str()));

    request->send(200, "text/plain", "Set " + variable + " to " + value);
}

#endif // CAMERA
