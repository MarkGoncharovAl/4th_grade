files = Dir.glob("../**/*.cpp").concat Dir.glob("../**/*.c").concat Dir.glob("../**/*.h").concat Dir.glob("../**/*.h").concat Dir.glob("../**/*.hpp")

files.each { |file|
  %x(clang-format -i #{file} -style=llvm)
}