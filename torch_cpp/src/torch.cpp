#include <vector>
#include <string>
#include <stdbool.h>

#include "torch.h"
#include "Eigen/Core"
#include "cjson/cJSON.h"
#include "onnxruntime/core/session/onnxruntime_cxx_api.h"


double init_OnnxModel() {
  Ort::Env environment(ORT_LOGGING_LEVEL_WARNING, "ONNXRuntimeExample");
  Ort::SessionOptions sessionOptions;

  sessionOptions.SetIntraOpNumThreads(1);
  sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_BASIC);

  // std::vector<int64_t> tensor1_shape = {2, 3};  // Shape: 2 rows, 3 columns
  // std::vector<float> tensor1_data = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

  return 1.1;
}

char* do_Json() {
  cJSON *root = cJSON_CreateObject();

  cJSON_AddStringToObject(root, "name", "Alice");
  cJSON_AddNumberToObject(root, "age", 25);
  cJSON_AddStringToObject(root, "city", "Los Angeles");

  char *json_string = cJSON_Print(root);
  cJSON_Delete(root);
  return json_string;
}

double do_MatrixOps(double x) {
  static const int nx = 10;
  static const int ny = 10;

  Eigen::Matrix<double, nx, ny> eXX;
  eXX.setZero();

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      eXX(i, j) = i * 2 * EIGEN_PI / nx;
    }
  }

  return x;
}

class Stack {
private:
  std::vector<int> data;

public:
  void push(int x) {
    data.push_back(x);
  }

  int pop() {
    int x = data.back();
    data.pop_back();
    return x;
  }

  bool isEmpty() {
    return data.empty();
  }

  int len() {
    return (int) data.size();
  }
};

struct Stack* stackObj;


void create_Stack() {
  if (!stackObj) {
    // printf("create_Stack\n");
    stackObj = (Stack*)malloc(sizeof(Stack));
  }
}


void push_Stack(int x) {
  if (stackObj) stackObj->push(x);
}


int pop_Stack() {
  if (stackObj) return stackObj->pop();
  else return -1;
}


void delete_Stack() {
  if (stackObj) {
    // printf("delete_Stack\n");
    stackObj = nullptr;
  }
}


bool isEmpty_Stack() {
  if (stackObj) return stackObj->isEmpty();
  else return true;
}


int len_Stack() {
  if (stackObj) return stackObj->len();
  else return -1;
}


int do_Stack(int a, int b) {
  Stack stack;
  stack.push(a);
  stack.push(b);
  stack.push(3);
  int c = 0;
  for (int i = 0; i < stack.len(); i++) {
    c += stack.pop();
  }
  return c;
}


char* add_Str(char* x) {
  std::string str = std::string(x);
  free(x);
  str.append("--");
  char* cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  return cstr;
}


int* add_Array1D(int size) {
  int* arr = (int*)malloc(size * sizeof(int));
  for (int i = 0; i < size; i++) {
      arr[i] = i + 1;
  }
  return arr;
}


int** add_Array2D(int rows, int columns) {
  int** arr = (int**)malloc(rows * sizeof(int*));
  for (int i = 0; i < rows; i++) {
    arr[i] = (int*)malloc(columns * sizeof(int));
  }

  int value = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      arr[i][j] = value;
      value++;
    }
  }
  return arr;
}


int count_Array2D(int** array, int rows, int columns) {
  int count = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      count += array[i][j];
    }
  }
  return count;
}


int count_Array1D(int* array, int rows) {
  int count = 0;
  for (int i = 0; i < rows; i++) {
    count += array[i];
  }
  return count;
}