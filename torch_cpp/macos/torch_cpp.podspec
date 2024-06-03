#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint torch_cpp.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'torch_cpp'
  s.version          = '0.0.1'
  s.summary          = 'A new Flutter plugin project.'
  s.description      = <<-DESC
A new Flutter plugin project.
                       DESC
  s.homepage         = 'http://example.com'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'Your Company' => 'email@example.com' }

  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency 'FlutterMacOS'

  s.platform = :osx, '13.0'
  s.pod_target_xcconfig = { 
    'DEFINES_MODULE' => 'YES', 
    'EXCLUDED_ARCHS[sdk=macos*]' => 'x86_64', 
    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
  }
  s.swift_version = '5.0'

  s.osx.deployment_target = '13.0'
  
  # s.public_header_files = 'Headers/Public/**/*.h'
  
  # s.vendored_frameworks = '.framework'
  s.vendored_libraries = 'Libs/**/*.dylib', 'Libs/**/*.a'
  
  # s.libraries = 'cjson', 'onnxruntime'
end
