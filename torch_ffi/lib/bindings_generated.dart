// AUTO GENERATED FILE, DO NOT EDIT.
//
// Generated by `package:ffigen`.
// ignore_for_file: type=lint
import 'dart:ffi' as ffi;

/// Bindings for the Torch FFI API
class TorchFfiBindings {
  /// Holds the symbol lookup function.
  final ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
      _lookup;

  /// The symbols are looked up in [dynamicLibrary].
  TorchFfiBindings(ffi.DynamicLibrary dynamicLibrary)
      : _lookup = dynamicLibrary.lookup;

  /// The symbols are looked up with [lookup].
  TorchFfiBindings.fromLookup(
      ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
          lookup)
      : _lookup = lookup;

  double cFunction(
    double x,
  ) {
    return _cFunction(
      x,
    );
  }

  late final _cFunctionPtr =
      _lookup<ffi.NativeFunction<ffi.Double Function(ffi.Double)>>('cFunction');
  late final _cFunction = _cFunctionPtr.asFunction<double Function(double)>();
}
