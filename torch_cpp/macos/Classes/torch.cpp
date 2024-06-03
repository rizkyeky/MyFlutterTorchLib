#include "../../src/torch.h"
#include "../../src/torch.cpp"

// double init_Torch() {
  
//   torch::jit::script::Module module;

//   c10::Device device = c10::Device(c10::kCPU);

//   try {
//     module = torch::jit::load(this->modelPath);
//     module.to(this->device);
//     module.eval();

//     if (module.is_training())
//       module.eval();
//   }
//   catch (const c10::Error& e) {

//   }

//   torch::manual_seed(0);

//   // Create random tensors
//   torch::Tensor tensor1 = torch::rand({3, 3});
//   torch::Tensor tensor2 = torch::rand({3, 3});

//   // Multiply the tensors
//   torch::Tensor result = tensor1 * tensor2;

//   return 1.1;
// }