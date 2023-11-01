cmake_minimum_required(VERSION 3.10)

add_library(ltc_wavetables INTERFACE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CXX_EXTENSIONS ON)
set(ltc_wavetables_VERSION_MAJOR 0)
set(ltc_wavetables_VERSION_MINOR 1)
set(VERSION {ltc_wavetables_VERSION_MAJOR}.{ltc_wavetables_VERSION_MINOR})

configure_file(inc/cmakeconfig.h.in inc/cmakeconfig.h)

target_include_directories(ltc_wavetables
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
