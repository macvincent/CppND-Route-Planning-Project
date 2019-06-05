# Route Planning Project Starter Code

## building the `IO2D`library
The `IO2D's` library's `CMake` script expects `cairo` and `graphicsmagick` to be installed. So before cloning the `IO2D` library we run
```
sudo apt update
sudo apt install build-essential
sudo apt install cmake
sudo apt install libcairo2-dev
sudo apt install libgraphicsmagick1-dev
sudo apt install libpng-dev
```
we then go ahead to clone and build the `IO2D` library
```
$ git clone --recurse-submodules https://github.com/cpp-io2d/P0267_RefImpl
$ cd P0267_RefImpl
$ mkdir Debug
$ cd Debug
$ cmake --config Debug "-DCMAKE_BUILD_TYPE=Debug" ..
$ cmake --build .
```
## updating `cmake`
The Ubuntu package for `Cmake` is usually an earlier version and won't be able to run our project. So we remove it
```
$ sudo apt-get purge cmake
```
And install a more current version from https://cmake.org/download/ such as cmake-3.14.3.tar.gz
```
$ tar -xzvf cmake-3.14.3.tar.gz
$ cd cmake-3.14.3
$ ./bootstrap
$ make
$ sudo make install 
```
Now you can clone this project
## Cloning

When cloning this project, be sure to use the `--recurse-submodules` flag. Using HTTPS:
```
git clone https://github.com/udacity/CppND-Route-Planning-Project.git --recurse-submodules
```
or with SSH:
```
git clone git@github.com:udacity/CppND-Route-Planning-Project.git --recurse-submodules
```

## Compiling and Running

### Compiling
To compile the project, first, create a `build` directory and change to that directory:
```
mkdir build && cd build
```
From within the `build` directory, then run `cmake` and `make` as follows:
```
cmake ..
make
```
### Running
The executables will be placed in the `bin` directory. From within `build`, you can run the project as follows:
```
../bin/CppND-Route-Planning-Project -f ../map.osm
```
After selecting the start and end positons you should have something similar to this
<p align="center">
  <img src = "https://github.com/macvincent/CppND-Route-Planning-Project/blob/master/instructions/map.png">
</p>
