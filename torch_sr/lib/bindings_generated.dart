// AUTO GENERATED FILE, DO NOT EDIT.
//
// Generated by `package:ffigen`.
// ignore_for_file: type=lint
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as pkg_ffi;

/// Bindings for the Torch SR API
class TorchSRBindings {
  /// Holds the symbol lookup function.
  final ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
      _lookup;

  /// The symbols are looked up in [dynamicLibrary].
  TorchSRBindings(ffi.DynamicLibrary dynamicLibrary)
      : _lookup = dynamicLibrary.lookup;

  /// The symbols are looked up with [lookup].
  TorchSRBindings.fromLookup(
      ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
          lookup)
      : _lookup = lookup;

  ImageData readImage(
    ffi.Pointer<pkg_ffi.Utf8> path,
    int size,
  ) {
    return _readImage(
      path,
      size,
    );
  }

  late final _readImagePtr = _lookup<
      ffi.NativeFunction<
          ImageData Function(
              ffi.Pointer<pkg_ffi.Utf8>, ffi.Int32)>>('readImage');
  late final _readImage = _readImagePtr
      .asFunction<ImageData Function(ffi.Pointer<pkg_ffi.Utf8>, int)>();

  void writeImage(
    ImageData image,
    ffi.Pointer<pkg_ffi.Utf8> path,
  ) {
    return _writeImage(
      image,
      path,
    );
  }

  late final _writeImagePtr = _lookup<
      ffi.NativeFunction<
          ffi.Void Function(
              ImageData, ffi.Pointer<pkg_ffi.Utf8>)>>('writeImage');
  late final _writeImage = _writeImagePtr
      .asFunction<void Function(ImageData, ffi.Pointer<pkg_ffi.Utf8>)>();

  ImageData resizeImage(
    ImageData image,
    int size,
  ) {
    return _resizeImage(
      image,
      size,
    );
  }

  late final _resizeImagePtr =
      _lookup<ffi.NativeFunction<ImageData Function(ImageData, ffi.Int32)>>(
          'resizeImage');
  late final _resizeImage =
      _resizeImagePtr.asFunction<ImageData Function(ImageData, int)>();

  double initTensor() {
    return _initTensor();
  }

  late final _initTensorPtr =
      _lookup<ffi.NativeFunction<ffi.Float Function()>>('initTensor');
  late final _initTensor = _initTensorPtr.asFunction<double Function()>();

  void initModel(
    ffi.Pointer<pkg_ffi.Utf8> modelPath,
  ) {
    return _initModel(
      modelPath,
    );
  }

  late final _initModelPtr =
      _lookup<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<pkg_ffi.Utf8>)>>(
          'initModel');
  late final _initModel =
      _initModelPtr.asFunction<void Function(ffi.Pointer<pkg_ffi.Utf8>)>();

  ImageData doModel(
    ffi.Pointer<pkg_ffi.Utf8> path,
  ) {
    return _doModel(
      path,
    );
  }

  late final _doModelPtr = _lookup<
          ffi.NativeFunction<ImageData Function(ffi.Pointer<pkg_ffi.Utf8>)>>(
      'doModel');
  late final _doModel =
      _doModelPtr.asFunction<ImageData Function(ffi.Pointer<pkg_ffi.Utf8>)>();

  void initAnimeSR(
    ffi.Pointer<pkg_ffi.Utf8> modelPath,
  ) {
    return _initAnimeSR(
      modelPath,
    );
  }

  late final _initAnimeSRPtr =
      _lookup<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<pkg_ffi.Utf8>)>>(
          'initAnimeSR');
  late final _initAnimeSR =
      _initAnimeSRPtr.asFunction<void Function(ffi.Pointer<pkg_ffi.Utf8>)>();

  void doAnimeSR(
    ffi.Pointer<pkg_ffi.Utf8> imagePath,
    ffi.Pointer<pkg_ffi.Utf8> savePath,
  ) {
    return _doAnimeSR(
      imagePath,
      savePath,
    );
  }

  late final _doAnimeSRPtr = _lookup<
      ffi.NativeFunction<
          ffi.Void Function(ffi.Pointer<pkg_ffi.Utf8>,
              ffi.Pointer<pkg_ffi.Utf8>)>>('doAnimeSR');
  late final _doAnimeSR = _doAnimeSRPtr.asFunction<
      void Function(ffi.Pointer<pkg_ffi.Utf8>, ffi.Pointer<pkg_ffi.Utf8>)>();
}

final class ImageData extends ffi.Struct {
  external ffi.Pointer<ffi.Uint8> data;

  @ffi.Int32()
  external int width;

  @ffi.Int32()
  external int height;

  @ffi.Int32()
  external int channel;
}
