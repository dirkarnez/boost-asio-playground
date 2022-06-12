cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_NO_SYSTEM_PATHS TRUE)
set(Boost_USE_STATIC_LIBS TRUE)
set(BOOST_ROOT "D:\\Code Projects\\cpp-libraries\\boost-1.78.0-windows-2019-mingw-static")

project(boost-asio-playground)

find_package(Boost REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})

  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

  add_executable(${PROJECT_NAME} 
    "src/main.cpp"
  )
  target_link_libraries(${PROJECT_NAME} 
    ${Boost_LIBRARIES} ws2_32
  )
endif()
