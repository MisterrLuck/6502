cmake_minimum_required(VERSION 4.2)
project(6502)

include_directories(include/)

set(CMAKE_CXX_STANDARD 20)

# Emulator
add_executable(6502 src/main.cpp
        src/mos_6502.cpp
        include/mos_6502.hpp

        src/opcodes.cpp
	)
