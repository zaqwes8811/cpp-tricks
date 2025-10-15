In the vicinity C/C++
=========

```
sudo apt-get install libgtest-dev libbenchmark-dev

# Install Gcc 15

mkdir gcc-15
cd gcc-15
git clone https://gcc.gnu.org/git/gcc.git
cd gcc

git checkout releases/gcc-15.1.0
./contrib/download_prerequisites

mkdir build
cd build
../configure --prefix=/opt/gcc-15 --enable-languages=c,c++ --disable-multilib

make -j `nproc`
sudo make install

# Install cmake

wget https://github.com/Kitware/CMake/releases/download/v4.1.2/cmake-4.1.2.tar.gz

./configure --prefix=/opt/cmake-4.1.2

# Ninja

wget https://github.com/ninja-build/ninja/archive/refs/tags/v1.13.1.tar.gz

cmake -DCMAKE_INSTALL_PREFIX=/opt/ninja-1.13.1/ ..

/opt/ninja-1.13.1/bin/ninja

# Build

mkdir build
cd build
/opt/cmake-4.1.2/bin/cmake -G Ninja \
        -DCMAKE_MAKE_PROGRAM=/opt/ninja-1.13.1/bin/ninja \
        -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..

```

- [Summary project deamonds](https://github.com/zaqwes8811/around-cc/wiki/Global-Deamonds)
- [С++ сode styles discussion](https://github.com/zaqwes8811/around-cc/wiki/CC--Code-Style)
- [Compile V8](https://github.com/zaqwes8811/around-cc/wiki/Compile-V8)
- [C code style](https://github.com/zaqwes8811/around-cc/wiki/Home)
