#ifndef EXAMPLE_CONFIG_H
#define EXAMPLE_CONFIG_H

#ifdef BOARD_EVKIT_V1
// Enable TFT by default on EVKIT
#define TFT_ENABLE
#endif

//#define USE_SAMPLEDATA
// ^ Uncomment this to use static sample data.
// ^ Comment this out to use live camera data.

#define CAMERA_FREQ (10 * 1000 * 1000)
#define TFT_BUFF_SIZE 10 // TFT buffer size

#define IMAGE_SIZE_X 74
#define IMAGE_SIZE_Y 74

#define TFT_OFFSET_X 80
#define TFT_OFFSET_Y 40
#define IMG_SCALE 2
#define SCALE 2

#endif