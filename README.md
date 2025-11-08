# brick breaker

A brick breaker game using OpenGL and c programing language

third-party library:
```
glad
glfw
cglm
FreeType
```

## Compile
1. installed vcpkg
2. execute the following command
```
vcpkg install glfw3 glad cglm FreeType Stb

cmake -B build -DCMAKE_TOOLCHAIN_FILE=your_vcpkg_dir/scripts/buildsystems/vcpkg.cmake

cd build

./brickTest1.exe
```