// Select camera model
// #define CAMERA_MODEL_ESP_EYE // Has PSRAM
// #define CAMERA_MODEL_ESP32_CAM // Has PSRAM
// #define CAMERA_MODEL_ESP32_CAM_ROBOT // Has PSRAM
// #define CAMERA_MODEL_ESP_S3_EYE // Has PSRAM
// #define CAMERA_MODEL_JSZWY_CYIS // No PSRAM
// #define CAMERA_MODEL_JSZWY_CYIS_2 // No PSRAM
// #define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
// #define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
// #define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
// #define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
// #define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
#define CAMERA_MODEL_TTGO_T_JOURNAL_ROBOT // No PSRAM
// #define CAMERA_MODEL_PYWIFI_ESP32_S2 // Has PSRAM
// #define CAMERA_MODEL_PYWIFI_ESP32_S3 // Has PSRAM
// #define CAMERA_MODEL_WROVER_KIT // Has PSRAM

#if defined(CAMERA_MODEL_ESP_EYE)
#define CAMERA

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 4
#define SIOD_GPIO_NUM 18
#define SIOC_GPIO_NUM 23

#define Y9_GPIO_NUM 36
#define Y8_GPIO_NUM 37
#define Y7_GPIO_NUM 38
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 35
#define Y4_GPIO_NUM 14
#define Y3_GPIO_NUM 13
#define Y2_GPIO_NUM 34
#define VSYNC_GPIO_NUM 5
#define HREF_GPIO_NUM 27
#define PCLK_GPIO_NUM 25

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_ESP32_CAM)
#define CAMERA

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#define SD_CLK 14
#define SD_CMD 15
#define SD_SDA 2
#define SD_SDA1 4
#define SD_SDA2 12
#define SD_SDA3 13

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_ESP32_CAM_ROBOT)
#define CAMERA

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

// #define SD_CLK 14
// #define SD_CMD 15
// #define SD_SDA 2
// #define SD_SDA1 4
// #define SD_SDA2 12
// #define SD_SDA3 13

#define MOTOR
#define MotorL_A_Pin 13
#define MotorL_B_Pin 12
#define MotorR_A_Pin 2
#define MotorR_B_Pin 14

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_ESP32_CAM_ROBOT)
#define CAMERA

#define PWDN_GPIO_NUM -1 // CAM_PWDN
#define RESET_GPIO_NUM -1 // DVP_RESET
#define XCLK_GPIO_NUM 15 // XMCLK
#define SIOD_GPIO_NUM 4 // I2C_SDA
#define SIOC_GPIO_NUM 5 // I2C_SCL

#define Y9_GPIO_NUM 16 // DVP_Y9
#define Y8_GPIO_NUM 17 // DVP_Y8
#define Y7_GPIO_NUM 18 // DVP_Y7
#define Y6_GPIO_NUM 12 // DVP_Y6
#define Y5_GPIO_NUM 11 // DVP_Y5
#define Y4_GPIO_NUM 10 // DVP_Y4
#define Y3_GPIO_NUM 9 // DVP_Y3
#define Y2_GPIO_NUM 8 // DVP_Y2
#define VSYNC_GPIO_NUM 6 // DVP_VSYNC
#define HREF_GPIO_NUM 7 // DVP_HREF
#define PCLK_GPIO_NUM 13 // DVP_PCLK

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_JSZWY_CYIS)
#define CAMERA

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 27
#define SIOD_GPIO_NUM 2
#define SIOC_GPIO_NUM 15

#define Y9_GPIO_NUM 14
#define Y8_GPIO_NUM 26
#define Y7_GPIO_NUM 25
#define Y6_GPIO_NUM 32
#define Y5_GPIO_NUM 34
#define Y4_GPIO_NUM 38
#define Y3_GPIO_NUM 39
#define Y2_GPIO_NUM 35
#define VSYNC_GPIO_NUM 13
#define HREF_GPIO_NUM 12
#define PCLK_GPIO_NUM 33

#define Headlamp_Pin 4

#define MOTOR
#define MotorR_A_Pin 9
#define MotorR_B_Pin 10
#define MotorL_A_Pin 18
#define MotorL_B_Pin 23

#elif defined(CAMERA_MODEL_JSZWY_CYIS_2)
#define CAMERA

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 27
#define SIOD_GPIO_NUM 2
#define SIOC_GPIO_NUM 15

#define Y9_GPIO_NUM 14
#define Y8_GPIO_NUM 26
#define Y7_GPIO_NUM 25
#define Y6_GPIO_NUM 32
#define Y5_GPIO_NUM 34
#define Y4_GPIO_NUM 38
#define Y3_GPIO_NUM 39
#define Y2_GPIO_NUM 35
#define VSYNC_GPIO_NUM 13
#define HREF_GPIO_NUM 12
#define PCLK_GPIO_NUM 33

#define Headlamp_Pin 4

#define MOTOR
#define MotorR_A_Pin 19
#define MotorR_B_Pin 22
#define MotorL_A_Pin 18
#define MotorL_B_Pin 23

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_M5STACK_PSRAM)
#define CAMERA

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM 27
#define SIOD_GPIO_NUM 25
#define SIOC_GPIO_NUM 23

#define Y9_GPIO_NUM 19
#define Y8_GPIO_NUM 36
#define Y7_GPIO_NUM 18
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 5
#define Y4_GPIO_NUM 34
#define Y3_GPIO_NUM 35
#define Y2_GPIO_NUM 32
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM 26
#define PCLK_GPIO_NUM 21

#define I2C_SDA_NUM 12
#define I2C_SCL_NUM 13

#elif defined(CAMERA_MODEL_M5STACK_V2_PSRAM)
#define CAMERA

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM 27
#define SIOD_GPIO_NUM 22
#define SIOC_GPIO_NUM 23

#define Y9_GPIO_NUM 19
#define Y8_GPIO_NUM 36
#define Y7_GPIO_NUM 18
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 5
#define Y4_GPIO_NUM 34
#define Y3_GPIO_NUM 35
#define Y2_GPIO_NUM 32
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 26
#define PCLK_GPIO_NUM 21

#define I2C_SDA_NUM 12
#define I2C_SCL_NUM 13

#elif defined(CAMERA_MODEL_M5STACK_WIDE)
#define CAMERA

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM 27
#define SIOD_GPIO_NUM 22
#define SIOC_GPIO_NUM 23

#define Y9_GPIO_NUM 19
#define Y8_GPIO_NUM 36
#define Y7_GPIO_NUM 18
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 5
#define Y4_GPIO_NUM 34
#define Y3_GPIO_NUM 35
#define Y2_GPIO_NUM 32
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 26
#define PCLK_GPIO_NUM 21

#define I2C_SDA_NUM 12
#define I2C_SCL_NUM 13

#elif defined(CAMERA_MODEL_M5STACK_ESP32CAM)
#define CAMERA

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM 27
#define SIOD_GPIO_NUM 25
#define SIOC_GPIO_NUM 23

#define Y9_GPIO_NUM 19
#define Y8_GPIO_NUM 36
#define Y7_GPIO_NUM 18
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 5
#define Y4_GPIO_NUM 34
#define Y3_GPIO_NUM 35
#define Y2_GPIO_NUM 17
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM 26
#define PCLK_GPIO_NUM 21

#define I2C_SDA_NUM 12
#define I2C_SCL_NUM 13

#elif defined(CAMERA_MODEL_TTGO_T_JOURNAL)
#define CAMERA

#define PWDN_GPIO_NUM 0
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM 27
#define SIOD_GPIO_NUM 25
#define SIOC_GPIO_NUM 23

#define Y9_GPIO_NUM 19
#define Y8_GPIO_NUM 36
#define Y7_GPIO_NUM 18
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 5
#define Y4_GPIO_NUM 34
#define Y3_GPIO_NUM 35
#define Y2_GPIO_NUM 17
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM 26
#define PCLK_GPIO_NUM 21

#define I2C_SDA_NUM 14
#define I2C_SCL_NUM 13
#define I2C_SSD1306_ADDRESS 0x3c

#elif defined(CAMERA_MODEL_TTGO_T_JOURNAL_ROBOT)
#define CAMERA

#define PWDN_GPIO_NUM 0
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM 27
#define SIOD_GPIO_NUM 25
#define SIOC_GPIO_NUM 23

#define Y9_GPIO_NUM 19
#define Y8_GPIO_NUM 36
#define Y7_GPIO_NUM 18
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 5
#define Y4_GPIO_NUM 34
#define Y3_GPIO_NUM 35
#define Y2_GPIO_NUM 17
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM 26
#define PCLK_GPIO_NUM 21

#define SERVO360MOTOR
#define SERVO360_L_Pin 2
#define SERVO360_R_Pin 4

#define I2C_SDA_NUM 14
#define I2C_SCL_NUM 13
#define I2C_SSD1306_ADDRESS 0x3c

#elif defined(CAMERA_MODEL_PYWIFI_ESP32_S2)
#define CAMERA

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 14
#define SIOD_GPIO_NUM 21
#define SIOC_GPIO_NUM 18

#define Y9_GPIO_NUM 15
#define Y8_GPIO_NUM 13
#define Y7_GPIO_NUM 12
#define Y6_GPIO_NUM 10
#define Y5_GPIO_NUM 8
#define Y4_GPIO_NUM 5
#define Y3_GPIO_NUM 7
#define Y2_GPIO_NUM 9
#define VSYNC_GPIO_NUM 17
#define HREF_GPIO_NUM 16
#define PCLK_GPIO_NUM 11

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_PYWIFI_ESP32_S3)
#define CAMERA

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 10
#define SIOD_GPIO_NUM 21
#define SIOC_GPIO_NUM 14

#define Y9_GPIO_NUM 11
#define Y8_GPIO_NUM 9
#define Y7_GPIO_NUM 8
#define Y6_GPIO_NUM 16
#define Y5_GPIO_NUM 6
#define Y4_GPIO_NUM 4
#define Y3_GPIO_NUM 5
#define Y2_GPIO_NUM 17
#define VSYNC_GPIO_NUM 13
#define HREF_GPIO_NUM 12
#define PCLK_GPIO_NUM 18

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_WROVER_KIT)
#define CAMERA

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 21
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 19
#define Y4_GPIO_NUM 18
#define Y3_GPIO_NUM 5
#define Y2_GPIO_NUM 4
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#endif
