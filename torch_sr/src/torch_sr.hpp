#ifndef TORCHSR_H
#define TORCHSR_H

#include <torch/script.h>
#include <opencv2/opencv.hpp>
#include <chrono>

#define EXPORT_C extern "C"

struct ImageData {
    uint8_t* data;
    int width;
    int height;
    int channel;
};

struct ratio {
    int numerator;
    int denominator;
};

int roundToNearestMultipleOf10(int num);
int findNearestRatioOptions(int width, int height);

torch::Tensor forwardModel(torch::Tensor input);
ImageData convertTensor3D2Image(torch::Tensor x);
torch::Tensor convertImage2Tensor3D(ImageData image);

EXPORT_C struct ImageData readImage(char* path, int size = -1);
EXPORT_C void writeImage(ImageData image, char* path);
EXPORT_C struct ImageData resizeImage(ImageData image, int size);

EXPORT_C float initTensor();
EXPORT_C void initModel(char* modelPath);
EXPORT_C struct ImageData doModel(char* path);

// --- OPENCV ---

std::string type2str(int type);
cv::Size findPerfectSize(cv::Size size);

EXPORT_C void initAnimeSR(char* modelPath);
EXPORT_C void doAnimeSR(char* imagePath, char* outputPath);

#endif // TORCHSR_H
