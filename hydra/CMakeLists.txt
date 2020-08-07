if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Hydra)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(Hydra
  main.cpp
  alignment.cpp
  alignment.hpp
  cluster.cpp
  cluster.hpp
  cmd_parser.cpp
  cmd_parser.hpp
  correspondence.cpp
  correspondence.hpp
  edge.cpp
  edge.hpp
  envelope.cpp
  envelope.hpp
  grid.cpp
  grid.hpp
  json.hpp
  msms_validation.cpp
  msms_validation.hpp
  peptide.cpp
  peptide.hpp
  writer.cpp
  writer.hpp
  )

install(
  TARGETS Hydra
  DESTINATION bin
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# Using GNU compiler, link filesystem library
	TARGET_LINK_LIBRARIES(Hydra PRIVATE -lstdc++fs)
endif()
