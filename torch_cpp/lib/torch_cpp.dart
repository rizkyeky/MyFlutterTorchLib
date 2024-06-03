import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as pffi;

import 'bindings_generated.dart';

class TorchCpp {
  
  final TorchCppBindings _bindings;

  TorchCpp() : 
    _bindings = TorchCppBindings(ffi.DynamicLibrary.executable());

  double initOnnxModel() => _bindings.init_OnnxModel();
  double doMatrixOps(double x) => _bindings.do_MatrixOps(x);
  int doStack(int a,int b) => _bindings.do_Stack(a, b);
  void createStack() => _bindings.create_Stack();
  void pushStack(int a) => _bindings.push_Stack(a);
  int popStack() => _bindings.pop_Stack();
  void deleteStack() => _bindings.delete_Stack();
  bool isEmptyStack() => _bindings.isEmpty_Stack();
  int lenStack() => _bindings.len_Stack();

  String addStr(String x) {
    final xPtr = x.toNativeUtf8();
    final resultPtr = _bindings.add_Str(xPtr);
    return resultPtr.toDartString();
  }

  String doJson() {
    final resultPtr = _bindings.do_Json();
    return resultPtr.toDartString();
  }

  List<int> addArray1D(int size) {
    final resultPtr = _bindings.add_Array1D(size);
    final ls = resultPtr.asTypedList(size);
    final arr = List<int>.from(ls);
    pffi.calloc.free(resultPtr);
    return arr;
  }

  List<List<int>> addArray2D(int rows, int columns) {
    final resultPtr = _bindings.add_Array2D(rows, columns);
    final resultList = <List<int>>[];
    for (int i = 0; i < rows; i++) {
      final innerPointer = resultPtr.elementAt(i).value;
      final innerList = innerPointer.asTypedList(columns);
      final ls = List<int>.from(innerList);
      resultList.add(ls);
      pffi.calloc.free(innerPointer);
    }
    pffi.calloc.free(resultPtr);
    return resultList;
  }

  int countArray1D(List<int> list) {
    if (list.isNotEmpty) {
      final lsPtr = pffi.malloc.allocate<ffi.Int32>(ffi.sizeOf<ffi.Int32>() * list.length);
      for (int i = 0; i < list.length; i++) {
        lsPtr.elementAt(i).value = list[i];
      }
      return _bindings.count_Array1D(lsPtr, list.length);
    } else {
      return 0;
    }
  }

  int countArray2D(List<List<int>> list) {
    if (list.isNotEmpty) {
      final lsPtr = pffi.malloc.allocate<ffi.Pointer<ffi.Int32>>(ffi.sizeOf<ffi.Pointer<ffi.Int32>>() * list.length);
      for (int i = 0; i < list.length; i++) {
        final innerList = list[i];
        final innerPtr = pffi.malloc.allocate<ffi.Int32>(ffi.sizeOf<ffi.Int32>() * innerList.length);
        for (int j = 0; j < innerList.length; j++) {
          innerPtr.elementAt(j).value = innerList[j];
        }
        lsPtr.elementAt(i).value = innerPtr;
      }
      return _bindings.count_Array2D(lsPtr, list.length, list[0].length);
    } else {
      return 0;
    }
  } 
}
