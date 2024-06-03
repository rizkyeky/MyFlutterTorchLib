import 'dart:ffi' as ffi;
// import 'package:ffi/ffi.dart' as pffi;

import 'bindings_generated.dart';

class TorchFfi {
  
  late final TorchFfiBindings _bindings;
  final String _libName = 'torch_ffi';

  TorchFfi() {
    _bindings = TorchFfiBindings(_dylib);
  }

  ffi.DynamicLibrary get _dylib {
    final libPath = '$_libName.framework/$_libName';
    // final libPath = 'lib$_libName.dylib';
    // final libPath = path.join(Directory.current.path, _libName, 'lib$_libName.dylib');
    return ffi.DynamicLibrary.open(libPath);
  }

  double cFunction(double x) => _bindings.cFunction(x);
}
