cmake_minimum_required(VERSION 4.2)
project(6502)
project(Tests)

include_directories(include/)

set(CMAKE_CXX_STANDARD 20)

# Emulator
add_executable(6502 src/main.cpp
        src/mos_6502.cpp
        include/mos_6502.hpp

        src/opcodes.cpp
	)

add_executable(Tests src/tests.cpp
        src/mos_6502.cpp
        include/mos_6502.hpp

        src/opcodes.cpp

		include/nlohmann/json.hpp
		include/nlohmann/json_fwd.hpp
		include/nlohmann/json_literals.hpp
		include/nlohmann/json_view.hpp
	)
