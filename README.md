# yobemag

[![build](https://github.com/benzammour/yobemag/actions/workflows/sonarcloud.yml/badge.svg?branch=main)](https://github.com/Benzammour/yobemag/actions)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=Benzammour_yobemag&metric=coverage)](https://sonarcloud.io/summary/new_code?id=Benzammour_yobemag)
[![License](https://img.shields.io/github/license/Benzammour/yobemag)](https://github.com/Benzammour/yobemag/blob/main/LICENSE)

## Build

```shell
rm -rf build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE -DOPTIMIZE=3 -DCMAKE_C_COMPILER=<gcc|clang> ..
make
```

### CMake Options

Use these options with `-D<Option>=<Value>`.

| Option             | Values                                                   | Explanation                                                                                                                                       | Requires         |
|--------------------|----------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------|------------------|
| `CMAKE_BUILD_TYPE` | `DEBUG`,`RELEASE`                                        | `DEBUG` adds the `-g` compiler flag and the `YOBEMAG_DEBUG` compile definition                                                                    | -                |
| `OPTIMIZE`         | `0`, `1`, `2`, `3`, `fast`, `s`, `z`                     | See, e.g., [GCC optimization options](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)                                                   | Compiler support |
| `CMAKE_C_COMPILER` | Supported: `gcc` (11), `clang` (11, 12, 13, 14)          | The supported versions are used by our pipeline and guarantee compatibility                                                                       | -                |
| `TEST`             | `0`, `1`                                                 | Disables/Enables building tests                                                                                                                   | -                |
| `COVERAGE`         | `0`, `1`                                                 | Removes/Adds instrumentation required for coverage reports                                                                                        | `TEST=1`         |
| `SANITIZE`         | gcc: `valgrind`, clang: `address`, `memory`, `undefined` | For `valgrind`, it runs the test executable with valgrind.<br>For the rest, the tests are instrumented with sanitizers but just run as executable | `clang` OR `gcc` |

### Build Targets

Use these targets with `make <Target>`

| Target     | Explanation                                                                                                                    |
|------------|--------------------------------------------------------------------------------------------------------------------------------|
|            | Default target, builds the `yobemag` executable                                                                                |
| `test`     | Builds the `yobemag_test` executable that runs unit tests from [`test/`](https://github.com/Benzammour/yobemag/tree/main/test) |
| `sanitize` | Runs `yobemag` or `yobemag_test` (depending on `TEST=<0/1>`) with the specified sanitizer (see [`OPTIMIZE`](###CMake-Options)) |
| `install`  | Builds the default target and copies it to `~/.local/bin/yobemag`. You can uninstall by just removing the binary.              |

## Test

```shell
rm -rf build
mkdir build && cd build
cmake -DTEST=1 -DCMAKE_BUILD_TYPE=DEBUG -DOPTIMIZE=3 -DCMAKE_C_COMPILER=<gcc|clang> ..
make test
```

## Run yobemag

```shell
yobemag [-l <0..4>] <ROM_PATH>
```

| Arguments  | Required | Explanation                                                       |
|------------|----------|-------------------------------------------------------------------|
| `-l`       | no       | Set the log level                                                 |
| `ROM_PATH` | yes      | Provide relative path (w.r.t. executable) or absolute path to rom |

## Contributing

If you have a suggestion that would make this better, please fork the repo and create a pull request.
You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/cool-feature`)
3. Implement your desired Feature!
3. Commit your Changes (`git commit -am 'Add some cool-feature'`)
4. Push to the Branch (`git push origin feature/cool-feature`)
5. Create a new [Pull Request](https://github.com/benzammour/yobemag/pulls)


### Pre-commit hooks

We strongly encourage you to install pre-commit hooks that make sure your commit is clean.
The hook runs the following things:

- YAML lint
- new line at EOF check
- merge conflict check
- clang-format (in-place)

You can install the hook as follows:

```shell
pip install pre-commit
pre-commit install
```
