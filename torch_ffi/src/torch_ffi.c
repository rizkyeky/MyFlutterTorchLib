#include "torch_ffi.h"

FFI_PLUGIN_EXPORT double cFunction(double x) {
  return 1 + 2 + x;
}