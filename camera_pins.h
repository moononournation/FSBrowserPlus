// Select camera model
// #define CAMERA_MODEL_AI_THINKER // Has PSRAM
// #define CAMERA_MODEL_AI_THINKER_ROBOT // Has PSRAM
// #define CAMERA_MODEL_ESP_EYE // Has PSRAM
// #define CAMERA_MODEL_JSZWY_CYIS // No PSRAM
// #define CAMERA_MODEL_JSZWY_CYIS_2 // No PSRAM
// #define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
// #define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
// #define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
// #define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
#define CAMERA_MODEL_LILYGO_T_DISPLAY_PRO // Has PSRAM
// #define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
// #define CAMERA_MODEL_TTGO_T_JOURNAL_ROBOT // No PSRAM
// #define CAMERA_MODEL_PYWIFI_ESP32_S2 // Has PSRAM
// #define CAMERA_MODEL_PYWIFI_ESP32_S3 // Has PSRAM
// #define CAMERA_MODEL_WROVER_KIT // Has PSRAM
// #define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3_ROBOT // Has PSRAM

// ** Espressif Internal Boards **
// #define CAMERA_MODEL_ESP32_CAM_BOARD
// #define CAMERA_MODEL_ESP32S2_CAM_BOARD
// #define CAMERA_MODEL_ESP32S3_CAM_LCD
// #define CAMERA_MODEL_ESP32S3_EYE

#if defined(CAMERA_MODEL_AI_THINKER)
#define CAMERA

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define LED_GPIO_NUM      33

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_AI_THINKER_ROBOT)
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

#elif defined(CAMERA_MODEL_ESP_EYE)
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

#define LED_GPIO_NUM 22

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

#define LED_GPIO_NUM 4

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

#define LED_GPIO_NUM 4

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

#define LED_GPIO_NUM 2

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
#define Y2_GPIO_NUM 32
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM 26
#define PCLK_GPIO_NUM 21

#define I2C_SDA_NUM 12
#define I2C_SCL_NUM 13

#elif defined(CAMERA_MODEL_LILYGO_T_DISPLAY_PRO)
#define CAMERA

#define PWDN_GPIO_NUM 46
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 11
#define SIOD_GPIO_NUM 5
#define SIOC_GPIO_NUM 6

#define Y9_GPIO_NUM 4
#define Y8_GPIO_NUM 10
#define Y7_GPIO_NUM 3
#define Y6_GPIO_NUM 1
#define Y5_GPIO_NUM 42
#define Y4_GPIO_NUM 40
#define Y3_GPIO_NUM 41
#define Y2_GPIO_NUM 45
#define VSYNC_GPIO_NUM 7
#define HREF_GPIO_NUM 15
#define PCLK_GPIO_NUM 2

#define LED_GPIO_NUM 38

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

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
// V6
// #define SERVO360_L_Pin 2
// #define SERVO360_R_Pin 4
// V8
#define SERVO360_REVERSE
#define SERVO360_L_Pin 4
#define SERVO360_R_Pin 2

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

#elif defined(CAMERA_MODEL_XIAO_ESP32S3)
#define CAMERA

#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     10
#define SIOD_GPIO_NUM     40
#define SIOC_GPIO_NUM     39

#define Y9_GPIO_NUM       48
#define Y8_GPIO_NUM       11
#define Y7_GPIO_NUM       12
#define Y6_GPIO_NUM       14
#define Y5_GPIO_NUM       16
#define Y4_GPIO_NUM       18
#define Y3_GPIO_NUM       17
#define Y2_GPIO_NUM       15
#define VSYNC_GPIO_NUM    38
#define HREF_GPIO_NUM     47
#define PCLK_GPIO_NUM     13

#define I2C_SDA_NUM 5
#define I2C_SCL_NUM 6

#elif defined(CAMERA_MODEL_XIAO_ESP32S3_ROBOT)
#define CAMERA

#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     10
#define SIOD_GPIO_NUM     40
#define SIOC_GPIO_NUM     39

#define Y9_GPIO_NUM       48
#define Y8_GPIO_NUM       11
#define Y7_GPIO_NUM       12
#define Y6_GPIO_NUM       14
#define Y5_GPIO_NUM       16
#define Y4_GPIO_NUM       18
#define Y3_GPIO_NUM       17
#define Y2_GPIO_NUM       15
#define VSYNC_GPIO_NUM    38
#define HREF_GPIO_NUM     47
#define PCLK_GPIO_NUM     13

#define MOTOR
#define Motor_EN_Pin 6
#define MotorR_A_Pin 1
#define MotorR_B_Pin 2
#define MotorL_A_Pin 4
#define MotorL_B_Pin 5

// #define I2C_SDA_NUM 5
// #define I2C_SCL_NUM 6
#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_ESP32_CAM_BOARD)
#define CAMERA

// The 18 pin header on the board has Y5 and Y3 swapped
#define USE_BOARD_HEADER 0 
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   33
#define XCLK_GPIO_NUM     4
#define SIOD_GPIO_NUM    18
#define SIOC_GPIO_NUM    23

#define Y9_GPIO_NUM      36
#define Y8_GPIO_NUM      19
#define Y7_GPIO_NUM      21
#define Y6_GPIO_NUM      39
#if USE_BOARD_HEADER
#define Y5_GPIO_NUM      13
#else
#define Y5_GPIO_NUM      35
#endif
#define Y4_GPIO_NUM      14
#if USE_BOARD_HEADER
#define Y3_GPIO_NUM      35
#else
#define Y3_GPIO_NUM      13
#endif
#define Y2_GPIO_NUM      34
#define VSYNC_GPIO_NUM    5
#define HREF_GPIO_NUM    27
#define PCLK_GPIO_NUM    25

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_ESP32S3_CAM_LCD)
#define CAMERA

#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     40
#define SIOD_GPIO_NUM     17
#define SIOC_GPIO_NUM     18

#define Y9_GPIO_NUM       39
#define Y8_GPIO_NUM       41
#define Y7_GPIO_NUM       42
#define Y6_GPIO_NUM       12
#define Y5_GPIO_NUM       3
#define Y4_GPIO_NUM       14
#define Y3_GPIO_NUM       47
#define Y2_GPIO_NUM       13
#define VSYNC_GPIO_NUM    21
#define HREF_GPIO_NUM     38
#define PCLK_GPIO_NUM     11

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_ESP32S2_CAM_BOARD)
#define CAMERA

// The 18 pin header on the board has Y5 and Y3 swapped
#define USE_BOARD_HEADER 0
#define PWDN_GPIO_NUM     1
#define RESET_GPIO_NUM    2
#define XCLK_GPIO_NUM     42
#define SIOD_GPIO_NUM     41
#define SIOC_GPIO_NUM     18

#define Y9_GPIO_NUM       16
#define Y8_GPIO_NUM       39
#define Y7_GPIO_NUM       40
#define Y6_GPIO_NUM       15
#if USE_BOARD_HEADER
#define Y5_GPIO_NUM       12
#else
#define Y5_GPIO_NUM       13
#endif
#define Y4_GPIO_NUM       5
#if USE_BOARD_HEADER
#define Y3_GPIO_NUM       13
#else
#define Y3_GPIO_NUM       12
#endif
#define Y2_GPIO_NUM       14
#define VSYNC_GPIO_NUM    38
#define HREF_GPIO_NUM     4
#define PCLK_GPIO_NUM     3

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#elif defined(CAMERA_MODEL_ESP32S3_EYE)
#define CAMERA

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 15
#define SIOD_GPIO_NUM 4
#define SIOC_GPIO_NUM 5

#define Y2_GPIO_NUM 11
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16

#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM 7
#define PCLK_GPIO_NUM 13

#define I2C_SDA_NUM SIOD_GPIO_NUM
#define I2C_SCL_NUM SIOC_GPIO_NUM

#endif
