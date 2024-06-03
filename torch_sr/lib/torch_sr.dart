import 'dart:async';
import 'dart:ffi' as ffi;
import 'dart:io';
// import 'dart:isolate';
// import 'dart:typed_data';
import 'dart:ui';
import 'package:ffi/ffi.dart' as pffi;
import 'package:flutter/rendering.dart';
import 'package:flutter/services.dart';
// import 'package:flutter/painting.dart';
import 'package:path/path.dart' as pth;
import 'package:path_provider/path_provider.dart' as pthp;

import 'bindings_generated.dart';

extension Uint8ListBlobConversion on Uint8List {
  ffi.Pointer<ffi.Uint8> allocatePointer() {
    final blob = pffi.calloc<ffi.Uint8>(length);
    final blobBytes = blob.asTypedList(length);
    blobBytes.setAll(0, this);
    return blob;
  }
}

class TorchSR {
  
  late final TorchSRBindings _bindings;
  final String _libName = 'torch_sr';
  late final String _appCachePath;

  TorchSR() {
    _bindings = TorchSRBindings(_dylib);
  }

  ffi.DynamicLibrary get _dylib {
    // final libPath = '$_libName.framework/$_libName';
    final libPath = 'lib$_libName.dylib';
    return ffi.DynamicLibrary.open(libPath);
  }

  String get _modelPath {
    final appPath = Directory(Platform.resolvedExecutable).parent.parent.path;
    return pth.join(appPath, 'Frameworks', '$_libName.framework', 'Resources', 'animesr.onnx');
  }
 
  Future<void> initModel() {
    final completer = Completer<void>();

    try {
      _bindings.initAnimeSR(_modelPath.toNativeUtf8());
    } catch (e) {
      throw FlutterError('Error init model');
    }

    Future.delayed(const Duration(seconds: 1), () async {
      _appCachePath = (await pthp.getApplicationCacheDirectory()).path;
      completer.complete();
    });

    return completer.future;
  }

  Future<String> doModel(String imagePath) {
    final completer = Completer<String>();
    
    final baseName = pth.basename(imagePath);
    final outputPath = '$_appCachePath/$baseName';

    try {
      _bindings.doAnimeSR(imagePath.toNativeUtf8(), outputPath.toNativeUtf8());
    } catch (e) {
      throw FlutterError('Error run model');
    }

    Future.delayed(const Duration(seconds: 3), () {
      completer.complete(outputPath);
    });
    
    return completer.future;
  }

}