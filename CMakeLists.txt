cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project (DomoticSystem)

include_directories(include, src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSION false)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_library(Device
   include/Device.h
   src/Device.cpp
)

add_library(DeviceFixedCycle
   include/DeviceFixedCycle.h
   src/DeviceFixedCycle.cpp
)

add_library(DeviceManual
   include/DeviceManual.h
   src/DeviceManual.cpp
)

add_library(Network
   include/Network.h
   src/Network.cpp
)

add_library(Time
   include/Time.h
   src/Time.cpp
)

add_library(Timer
   include/Timer.h
   src/Timer.cpp
)

add_library(LoggerTime
   include/LoggerTime.h
   src/LoggerTime.cpp
)

add_executable(main
   src/main.cpp
   src/Device.cpp
   src/DeviceManual.cpp
   src/DeviceFixedCycle.cpp
   src/Network.cpp
   src/Time.cpp
   src/Timer.cpp
   src/LoggerTime.cpp
)

target_link_libraries(main
   Device
   DeviceFixedCycle
   DeviceManual
   Network
   Time
   Timer
   LoggerTime
)
