# yobemag

## Build

```bash
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=release -DOPTIMIZE=3 -DCMAKE_C_COMPILER=<gcc|clang> ..
ninja
```
