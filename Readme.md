# ObjectListModel

*Hard fork from [QQmlModel](https://github.com/OlivierLDff/QQmlModel).*

Additional data models aimed to bring more power to QML applications by using useful C++ models in back-end.

* ObjectList : a much nicer way to expose C++ list to QML than the quick & dirty `QList<QObject*>` property . Supports all the strong model features of QAbstractListModel while showing the simple and well know API of QList.

* SharedObjectList : Same as ObjectList but with `QSharedPointer<QObject>` that allow to easily share QObject based class between different list without the need of managing ownership

## Dependencies

* [gtest](https://github.com/google/googletest) : Unit and mock testing framework

## Building with CMake

Download and install [CMake](https://cmake.org/).

First clone this repository, then create a build folder and run cmake. In source and out of source build is supported.

```bash
# Clone the repository
git clone https://github.com/OlivierLDff/ObjectListModel
# Enter the folder and create a build folder (build folder is added in .gitignore)
cd ObjectListModel && mkdir build && cd build
# Run cmake
cmake ..
```

The `CMakeLists.txt` comes with multiples options for you to override when executing with CMake. simply add `-D<option>=<value>`.

### Parameters

CMake support multiple options.

- **OBJLISTMODEL_TARGET**: The name of the target that will be generated. *Default "ObjectListModel"*.
- **OBJLISTMODEL_BUILD_SHARED**: Build a shared library. *Default "OFF". [ON/OFF]*.
- **OBJLISTMODEL_FOLDER_PREFIX**: Prefix folder for all ObjectListModel generated targets in generated project (only decorative). *Default "ObjectListModel".*
- **OBJLISTMODEL_ENABLE_TESTS**: Enable the tests. This will create a target `${OBJLISTMODEL_TESTS_PREFIX}_test`. *Default "OFF". [ON/OFF]*.
- **OBJLISTMODEL_TESTS_PREFIX**: Prefix for every tests to avoid naming clashes in superbuild. *Default "ObjectListModel".*
- **OBJLISTMODEL_ENABLE_EXAMPLES**: Enable examples. This will create a target for each examples. *Default "OFF". [ON/OFF]*.
  - `${OBJLISTMODEL_EXAMPLES_PREFIX}soak`: Run a continuous client/server communication with typical production load.
  - `${OBJLISTMODEL_EXAMPLES_PREFIX}profile`
  - `${OBJLISTMODEL_EXAMPLES_PREFIX}client`: Run a ObjectListModel client that connects to the server running on localhost
  - `${OBJLISTMODEL_EXAMPLES_PREFIX}server`: Run a ObjectListModel server on localhost on UDP port 40000.
  - `${OBJLISTMODEL_EXAMPLES_PREFIX}client_server`: Run a client and a server and test packet exchange.
- **OBJLISTMODEL_EXAMPLES_PREFIX**: Prefix for every examples to avoid naming clashes in superbuild. *Default "ObjectListModel".*
- **OBJLISTMODEL_ENABLE_INSTALL**: Enable install target. *Default "OFF". [ON/OFF]*.
- **OBJLISTMODEL_INSTALL_PREFIX**: Folder for all ObjectListModel headers in the install folder. *Default "ObjectListModel".*

If you want to enable everything:

```bash
cmake                               \
  -DOBJLISTMODEL_BUILD_SHARED=OFF      \
  -DOBJLISTMODEL_TARGET="ObjectListModel"   \
  -DOBJLISTMODEL_ENABLE_TESTS=ON       \
  -DOBJLISTMODEL_ENABLE_EXAMPLES=ON    \
  -DOBJLISTMODEL_ENABLE_INSTALL=ON     \
  ..
```
### Build

Depending on the generator use, you can use the generator specific command like `make` or `ninja` or `msbuild`. When building with a one release type generator you might need to add `-DCMAKE_BUILD_TYPE=Release` or `-DCMAKE_BUILD_TYPE=Debug`. Available config depends on your generator.

More generally you can simply use cmake build command.

```bash
## Equivalent of make all for any generator
cmake --build .
## Equivalent of "make ObjectListModel" in release mode
cmake --build . --target ObjectListModel --config Release
```

The `ObjectListModel` binary will be available in the `lib` folder of your build folder.

### Install

An install target is available for convenience and will deploy ObjectListModel on your system or any where you want.

```bash
## Specifify a custom install folder (optionnal)
cmake -DOBJLISTMODEL_ENABLE_INSTALL=ON -DCMAKE_INSTALL_PREFIX="/path/to/my/install/dir" ..
## Equivalent of "make install" (Debug)
cmake --build . --target install --config Debug
## Equivalent of "make install" (Release)
cmake --build . --target install --config Release
```

You can choose the install directory by setting **CMAKE_INSTALL_PREFIX** when configuring the project. By default on Unix system it is set to `/usr/local` and on Windows to `c:/Program Files/ObjectListModel`.

In this folder you will find an include folder ready to be included by another application. This is a copy of the `include` folder of this repository. A `lib` will be created with all the generated libraries. A `cmake` folder contain all the CMake scripts to find the package.

The installation prefix is also added to `CMAKE_SYSTEM_PREFIX_PATH` so that **find_package()**, **find_program()**, **find_library()**, **find_path()**, and **find_file()** will search the prefix for other software.

### Run Tests

To run the tests you need to compile the library as static, and set the `OBJLISTMODEL_ENABLE_TEST` flag to ON. Testing is done using CTest framework.

```bash
## Enable the tests
cmake -DOBJLISTMODEL_ENABLE_TESTS=ON ..
## Build all the tests and executables
cmake --build . --config Release
# Then run the tests
ctest -C Release
```