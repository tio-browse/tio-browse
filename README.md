
# TIO Browse

TIO Browse is a viewer for exploring the structure of TyphonIO files.

TIO Browse is writen in C++ and uses the Qt library for the user interface. It
naturally has a dependency on the TyphonIO library. The test suite is based on
Google Test.

# Building

## CMake

TIO Browse use CMake to generate the build files.

Run CMake from the build directory, for example:

```
cmake -DQt5_enabled:BOOL=ON <source-dir>
```

The build accepts various options, the most important are summarised in the
table below

| Option                  | Type | Default           | Description         |
| :---------------------- | :--- | :---------------- | :------------------ |
| `TyphonIO_ROOT`         | PATH | _NA_              | TyphonIO path       |
| `Qt4_enabled`           | BOOL | OFF               | Build Qt4 GUI       |
| `Qt5_enabled`           | BOOL | OFF               | Build Qt5 GUI       |
| `CMAKE_INSTALL_PREFIX`  | PATH | platform specific | Install path        |
| `Documentation_enabled` | BOOL | OFF               | Build documentation |
| `GTEST_ROOT`            | PATH | _NA_              | Google Test path    |
| `Example_data_enabled`  | BOOL | OFF               | Build example files |
| `Python_enabled`        | BOOL | ON                | Enable python console |

## CMake example

Here is an example of how to run CMake:

```
export CXX=g++ CC=gcc
cmake \
    -DQt5_enabled:BOOL=ON \
    -DTyphonIO_ROOT:PATH=<path-to-TyphonIO> \
    -DHDF5_ROOT:PATH=<path-to-HDF5> \
    ../tio-browse
```
and here is an example with documentation and testing enabled:

```
export CXX=g++ CC=gcc
cmake \
    -DQt5_enabled:BOOL=ON \
    -DTyphonIO_ROOT:PATH=<path-to-TyphonIO> \
    -DHDF5_ROOT:PATH=<path-to-HDF5> \
    -DDocumentation_enabled:BOOL=ON \
    -DExample_data_enabled:BOOL=ON \
    -DTesting_enabled:BOOL=ON \
    -DGTEST_ROOT:PATH=<path-to-GTest> \
    ../tio-browse
```

## Build

Build using CMake:

```
cmake --build .
```

## Build documentation

To build the documentation, use the `docs` CMake target:

```
cmake --build . --target docs
```

## Installation

To install the app, use the `install` CMake target:

```
cmake --build . --target install
```

The install directory can be set with the `CMAKE_INSTALL_PREFIX` option to CMake
during the CMake project generation step.

## Build options

The test suite makes use of the Google Test framework. To build the test
suite, it is necessary to build TIO Browse with the `Testing_enabled` option
set to `ON`.

The tests can be run using `ctest`:

```
ctest --output-on-failure
```

Alternatively, the `test_runner` app can be run directly from the
`<build-dir>/test` directory:

```
./test_runner
```

The `test_runner` app accepts Google Test options, for example ot output a
junit-compatible xml file:

```
.test_runner --gtest_output=xml
```

## Python Console Example

Here is an example of how to used the integrated python console (assumed that
you opened a file in tio-browse and used "Send to console" button to send array
to python):

```python
# Use help() to see helper functions
>>>help()
# Use dir() to see all attributes and methods in the global namespace
>>>dir()
# You should be able to see the name of your array in list output

# An example on how to quickly plot an array in python
>>>import matplotlib.pyplot as plt
>>>plt.plot(<data>)  # replace <data> with the name of your array
>>>plt.show()
```
