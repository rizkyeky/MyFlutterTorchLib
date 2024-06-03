#include <stdint.h>

struct ImageData {
    uint8_t* data;
    int width;
    int height;
    int channel;
};

struct ImageData readImage(char* path, int size);
void writeImage(struct ImageData image, char* path);
struct ImageData resizeImage(struct ImageData image, int size);

float initTensor();
void initModel(char* modelPath);
struct ImageData doModel(char* path);

void initAnimeSR(char* modelPath);
void doAnimeSR(char* imagePath, char* savePath, int size);
