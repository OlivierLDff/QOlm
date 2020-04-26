---
layout: default
title: 🔨 Build & Tests
nav_order: 3
---

# 🔨 Build and Tests

- QOlm is available as a static or shared library.
- It come with unit tests `QOlm_Tests`
- It come with examples `QOlm_Examples`

## Create a static library

```bash
git clone https://github.com/OlivierLDff/QOlm
cd QOlm
mkdir build && cd build
cmake ..
cmake --build . --target QOlm --config "Release"
```

##  Create a shared library

```bash
git clone https://github.com/OlivierLDff/QOlm
cd QOlm
mkdir build && cd build
cmake -DQOLM_BUILD_SHARED=ON ..
cmake --build . --target QOlm --config "Release"
```


## CMake Integration

Add the following code snippet to your `CMakelists.txt`. This will download and build QOlm with `MyTarget`.

`CMake 3.14` is required to use `FetchContent` functions.

```cmake
# ...
include(FetchContent)
FetchContent_Declare(
    QOlm
    GIT_REPOSITORY "https://github.com/OlivierLDff/QOlm"
    GIT_TAG        "master"
)
# ...
FetchContent_MakeAvailable(QOlm)
# ...

target_link_libraries(MyTarget PUBLIC QOlm)
```

## Execute Tests

If `QOLM_ENABLE_TESTS` is on then you can execute unit test.

```bash
cmake -DQOLM_ENABLE_TESTS=ON ..
cmake --build . --target QOlm_Tests --config "Release"
ctest -C "Release" .
```

To only execute certain test in as a developer:

```
./tests/QOlm_Tests --gtest_filter=TestCaseRegEx*
```

## Execute Example

The option `QOLM_ENABLE_EXAMPLES` should be turned on.

```bash
cmake -DQOLM_ENABLE_EXAMPLES=ON ..
cmake --build . --target QOlm_Example --config "Release"
./examples/QOlm_Example
```

## 🚩 Additional CMake flags

Since CMake is using `FetchContent` functionality, you can add flags to understand what is going on. The library also require package Qt, so you need to indicate where Qt SDK is installed. Provide the path with `CMAKE_PREFIX_PATH`.

```bash
cmake
# Log output during download of dependencies
-DFETCHCONTENT_QUIET=OFF
# Avoid that dependencies source gets pulled at each cmake command
# Very useful when developping on dependencies too.
-DFETCHCONTENT_UPDATES_DISCONNECTED=ON
-DCMAKE_PREFIX_PATH=/Path/To/Qt
..
```

## Dependencies

QOlm is fetching quite a few external libraries. Source of those libraries can be customize. Variables are in form `_REPOSITORY` and `_TAG`

- Qt `Core` `Qml`.
- `EVENTPP`: Provide typed callback for insert/removed/moved operation.

### Dependencies of tests

- `GTEST` : Provide `gtest` library for unit tests.
- Qt `Tests`.
