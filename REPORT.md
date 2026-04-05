# Отчет по лабораторной работе №3

## Тема
Изучение CMake

## Цель
Освоить автоматизацию сборки C++ проектов с CMake

## Версии утилит
```bash
g++ --version    # g++ 11.4.0
cmake --version  # cmake 3.22.1
git --version    # git 2.34.1
ar --version     # GNU ar 2.38
nm --version     # GNU nm 2.38

# Настройка репозитория
export GITHUB_USERNAME=Makhdi1
git clone https://github.com/${GITHUB_USERNAME}/lab02.git projects/lab03
cd projects/lab03
git remote remove origin
git remote add origin https://github.com/${GITHUB_USERNAME}/lab03.git

# Ручная сборка
g++ -std=c++11 -I./include -c sources/print.cpp
ar rvs print.a print.o
g++ -std=c++11 -I./include -c examples/example1.cpp
g++ example1.o print.a -o example1
./example1                    # hello

g++ -std=c++11 -I./include -c examples/example2.cpp
g++ example2.o print.a -o example2
./example2
cat log.txt                   # hello

# Очистка
rm -rf *.o print.a example1 example2 log.txt

# CMakeLists.txt
cat > CMakeLists.txt <<EOF
cmake_minimum_required(VERSION 3.4)
project(print)
set(CMAKE_CXX_STANDARD 11)
add_library(print STATIC \${CMAKE_CURRENT_SOURCE_DIR}/sources/print.cpp)
include_directories(\${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(example1 \${CMAKE_CURRENT_SOURCE_DIR}/examples/example1.cpp)
add_executable(example2 \${CMAKE_CURRENT_SOURCE_DIR}/examples/example2.cpp)
target_link_libraries(example1 print)
target_link_libraries(example2 print)
EOF

# Сборка CMake
cmake -H. -B_build
cmake --build _build
_build/example1               # hello

# Установка
cmake -H. -B_build -DCMAKE_INSTALL_PREFIX=_install
cmake --build _build --target install
tree _install
# _install/
# ├── include/print.hpp
# └── lib/libprint.a

# Git
git add CMakeLists.txt
git commit -m "added CMakeLists.txt"
git push origin main
