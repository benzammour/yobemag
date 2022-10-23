# yobemag
[![build](https://github.com/benzammour/yobemag/actions/workflows/quality-control.yml/badge.svg)](https://github.com/Benzammour/yobemag)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=Benzammour_yobemag&metric=coverage)](https://sonarcloud.io/summary/new_code?id=Benzammour_yobemag)
[![License](https://img.shields.io/github/license/Benzammour/yobemag)](https://github.com/Benzammour/yobemag/blob/main/LICENSE)

## Build

```bash
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=release -DOPTIMIZE=3 -DCMAKE_C_COMPILER=<gcc|clang> ..
ninja
```

## Test
```bash
rm -rf build
mkdir build && cd build
cmake -G Ninja -DTEST=1 -DCMAKE_BUILD_TYPE=debug -DOPTIMIZE=3 -DCMAKE_C_COMPILER=<gcc|clang> ..
ninja test
cd ..
```
