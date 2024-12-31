git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg.exe install freeglut
cd ..
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --target install
cd ..
