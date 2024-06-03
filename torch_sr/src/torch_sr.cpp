#include "torch_sr.hpp"

EXPORT_C float initTensor() {
  c10::Device device = c10::Device(c10::kCPU);
  torch::Tensor a = torch::randn({10, 10, 10});
  torch::Tensor b = torch::randn({10, 10, 10});
  torch::Tensor tensor = torch::mul(a, b);
  float total = tensor.sum().item<float>();
  return total;
}

EXPORT_C ImageData readImage(char* path, int size) {
  cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
  if (image.empty()) {
    std::cerr << "Error could not open or find the image" << std::endl;
    throw std::runtime_error("Error could not open or find the image");
  }

  cv::Mat rgbImage;
  cv::cvtColor(image, rgbImage, cv::COLOR_BGR2RGB);
  
  uint8_t* data = rgbImage.data;
  int height = rgbImage.size().height;
  int width = rgbImage.size().width;
  int channel = rgbImage.channels();

  std::cout << "Image size: " << height << "x" << width << "x" << channel << std::endl;
  
  ImageData imageData = {data, width, height, channel};
  
  return resizeImage(imageData, size);
}

EXPORT_C void writeImage(ImageData image, char* path) {
  cv::Mat mat(image.height, image.width, CV_8UC3, image.data);

  cv::Mat rgbImage;
  cv::cvtColor(mat, rgbImage, cv::COLOR_RGB2BGR);

  cv::imwrite(path, rgbImage);
}

EXPORT_C ImageData resizeImage(ImageData image, int size) {
  if (size < 0) {
    return image;
  } else {
    cv::Mat mat(image.height, image.width, CV_8UC3, image.data);
    
    int width = image.width;
    int height = image.height;

    if (width > height) {
      height = int(float(size) / width * height);
      width = size;
    } else {
      width = int(float(size) / height * width);
      height = size;
    }

    cv::Mat resizedImage;
    cv::resize(mat, resizedImage, cv::Size(width, height));

    ImageData resizedImageData = {resizedImage.data, width, height, image.channel};
    return resizedImageData;
  }
}

torch::jit::script::Module model;
c10::Device device = c10::Device(c10::kCPU);

EXPORT_C void initModel(char* modelPath) {
  std::filesystem::path currentPath = std::filesystem::current_path();
  std::cout << "Current path: " << currentPath << std::endl;
  std::cout << "Model path: " << modelPath << std::endl;
  try {
    model = torch::jit::load(modelPath);
    model.to(device);
    model.eval();
  }
  catch (const c10::Error& e) {
    std::cerr << "Error loading the model\n";
    std::cerr << e.msg() << std::endl;
    return;
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Runtime error loading the model\n";
    std::cerr << e.what() << std::endl;
    return;
  }
  catch (const std::exception& e) {
    std::cerr << "Other Exception loading the model\n";
    std::cerr << e.what() << std::endl;
    return;
  }
  std::cout << "Model loaded" << std::endl;  
}

EXPORT_C ImageData doModel(char* path) {
  
  std::cout << "Image path: " << path << std::endl;
  
  auto start = std::chrono::high_resolution_clock::now();

  cv::Mat bgrImage = cv::imread(path, cv::IMREAD_COLOR);

  cv::Mat rgbImage;
  cv::cvtColor(bgrImage, rgbImage, cv::COLOR_BGR2RGB);

  int width = roundToNearestMultipleOf10(rgbImage.size().width);
  int height = roundToNearestMultipleOf10(rgbImage.size().height);
  ratio ratioOptions[] = {
    {1,1}, // 1
    {4,3}, // 1,3333
    {3,2}, // 1,5
    {5,3}, // 1,6
    {16,9} // 1,777
  };
  float newRatio = 1;

  if (width > height) {
    // Landscape
    int newIndex = findNearestRatioOptions(width, height);
    ratio newRatio = ratioOptions[newIndex];
    height = int((width / (float) newRatio.numerator) * newRatio.denominator);
    height = height % 2 == 0 ? height : height+1;
    height += 2;
    height = height % 10 == 0 ? height+2 : height+4;
  } else if (width < height) {
    // Portrait
    int newIndex = findNearestRatioOptions(height, width);
    ratio newRatio = ratioOptions[newIndex];
    width = int((height / (float) newRatio.numerator) * newRatio.denominator);
    width = width % 2 == 0 ? width : width+1;
  }

  cv::Mat resizedImage;
  cv::resize(rgbImage, resizedImage, cv::Size(width, height), cv::INTER_LINEAR_EXACT);

  torch::Tensor tensorImg = torch::from_blob(resizedImage.data,
    {resizedImage.size().height, resizedImage.size().width, resizedImage.channels()}, torch::kUInt8)
    .to(torch::kFloat32);
  tensorImg = tensorImg.div(225);
  tensorImg = tensorImg.permute({2, 0, 1});

  torch::Tensor tensorResult = forwardModel(tensorImg);

  torch::Tensor newTensor = tensorResult;
  newTensor = newTensor.mul(255).clamp(0, 255).to(torch::kUInt8);
  newTensor = newTensor.permute({1, 2, 0});

  int heightTensor = newTensor.size(0);
  int widthTensor = newTensor.size(1);
  int channel = newTensor.size(2);
  
  newTensor = newTensor.reshape({widthTensor * heightTensor * 3});
  uint8_t* data = newTensor.data_ptr<uint8_t>();

  ImageData imageData = {data, widthTensor, heightTensor, channel};

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Model forward time: " << duration.count() << "ms" << std::endl;

  return imageData;
}

torch::Tensor forwardModel(torch::Tensor input) {
  std::cout << "Input size: " << input.sizes() << std::endl;
  
  auto start = std::chrono::high_resolution_clock::now();
  
  c10::InferenceMode guard;
  torch::Tensor output;
  try {
    input = input.to(device);
    model.to(device);
    output = model.forward({input}).toTensor();
  }
  catch (const c10::Error& e) {
    std::cerr << "Error forward image to the model\n";
    std::cerr << e.msg() << std::endl;
    output = input;
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Runtime error forward image to the model\n";
    std::cerr << e.what() << std::endl;
    output = input;
  }
  catch (const std::exception& e) {
    std::cerr << "Other exception forward image to the model\n";
    std::cerr << e.what() << std::endl;
    output = input;
  }
  std::cout << "Output size: " << output.sizes() << std::endl;
  
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Model forward time: " << duration.count() << "ms" << std::endl;
  
  return output;
}

ImageData convertTensor3D2Image(torch::Tensor x) {
  torch::Tensor tensor = x.mul(255).clamp(0, 255).to(torch::kU8);
  tensor = tensor.detach().permute({1, 2, 0});

  int height = tensor.size(0);
  int width = tensor.size(1);
  int channel = tensor.size(2);
  
  tensor = tensor.reshape({width * height * 3});
  uint8_t* data = tensor.data_ptr<uint8_t>();

  ImageData imageData = {data, width, height, channel};

  return imageData;
}

torch::Tensor convertImage2Tensor3D(ImageData image) {
  
  torch::Tensor tensor = torch::from_blob(image.data, 
    {image.height, image.width, image.channel}, torch::kByte).to(torch::kFloat);
  tensor = tensor.div(225);
  tensor = tensor.permute({2, 0, 1});

  return tensor;
}

int roundToNearestMultipleOf10(int num) {
  int remainder = num % 10;
  return num - remainder;
}

int findNearestRatioOptions(int first, int second) {
  
  int index = -1;

  float targetRatio = first / (float) second;
  float aspectRatios[] = {
    1.0, // 1
    4.0/3.0, // 1,3333
    3.0/2.0, // 1,5
    5.0/3.0, // 1,666
    16.0/9.0 // 1,777
  };
  
  float minDifference = std::abs(targetRatio - aspectRatios[0]);
  
  for (int i = 0; i < 5; i++) {
    float distance = abs(targetRatio - aspectRatios[i]);
    if (distance < minDifference) {
      minDifference = distance;
      index = i;
    }
  }
  return index;
}

cv::Size findPerfectSize(cv::Size size) {
  int width = roundToNearestMultipleOf10(size.width);
  int height = roundToNearestMultipleOf10(size.height);
  ratio ratioOptions[] = {
    {1,1}, // 1
    {4,3}, // 1,3333
    {3,2}, // 1,5
    {5,3}, // 1,6
    {16,9} // 1,777
  };
  float newRatio = 1;

  if (width > height) {
    // Landscape
    int newIndex = findNearestRatioOptions(width, height);
    ratio newRatio = ratioOptions[newIndex];
    height = int((width / (float) newRatio.numerator) * newRatio.denominator);
    height = height % 2 == 0 ? height : height+1;
    height += 2;
    height = height % 10 == 0 ? height+2 : height+4;
  } else if (width < height) {
    // Portrait
    int newIndex = findNearestRatioOptions(height, width);
    ratio newRatio = ratioOptions[newIndex];
    width = int((height / (float) newRatio.numerator) * newRatio.denominator);
    width = width % 2 == 0 ? width : width+1;
  }

  return cv::Size(width, height);
}

std::string type2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

cv::dnn::Net onnxModel;

void initAnimeSR(char* modelPath) {
  onnxModel = cv::dnn::readNetFromONNX(modelPath);
}

void doAnimeSR(char* imagePath, char* outputPath) {

  auto start = std::chrono::high_resolution_clock::now();
  
  cv::Mat inputImage = cv::imread(imagePath, cv::IMREAD_COLOR);

  int oriWidth = inputImage.size().width;
  int oriHeight = inputImage.size().height;

  cv::Mat blob = cv::dnn::blobFromImage(inputImage, 1.0/255, cv::Size(320,320), cv::Scalar(), true);

  onnxModel.setInput(blob);
  cv::Mat output = onnxModel.forward();

  cv::Mat outputImage;
  cv::transposeND(output, {1, 2, 0}, outputImage);

  cv:: Mat uint8Image;
  outputImage.convertTo(uint8Image, CV_8UC1, 255);

  cv::Mat convertedImage(uint8Image.size().height, uint8Image.size().width, CV_8UC3);
  for (int row = 0; row < uint8Image.size().height; ++row) {
    for (int col = 0; col < uint8Image.size().width; ++col) {
      cv::Vec3b channels;
      for (int chan = 0; chan < 3; ++chan) {
        uint8_t value = uint8Image.at<uint8_t>(row, col, chan);
        channels[chan] = value;
      }
      convertedImage.at<cv::Vec3b>(row, col) = channels;
    }
  }

  cv::Mat bgrOutput;
  cv::cvtColor(convertedImage, bgrOutput, cv::COLOR_RGB2BGR);

  cv::Mat finalImageOuput;
  cv::resize(bgrOutput, finalImageOuput, cv::Size(oriWidth*4, oriHeight*4), cv::INTER_LINEAR_EXACT);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Model forward time: " << duration.count() << "ms" << std::endl;

  cv::imwrite(outputPath, finalImageOuput);
}