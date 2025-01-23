cmake_minimum_required(VERSION 3.23)
project(extractor)

set(CMAKE_BUILD_TYPE Release CACHE STRING "set build type to Release")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3")

add_compile_options(-O3)
add_compile_options(-static)
#add_compile_options(-fopenmp)

include_directories(src/model/parallel_hashmap)
include_directories(src/features)
include_directories(src/model)
include_directories(src/pb)
include_directories(src/tables/context/)
include_directories(src/tables/cross/)
include_directories(src/tables/ctxitem/)
include_directories(src/tables/item/)
include_directories(src/tables/query/)
include_directories(src/tables/user/)

include_directories(src/application/rcmd)
include_directories(src/application/search)
include_directories(src/application/recall)
include_directories(src/application/relative)
include_directories(src/application/outer)
include_directories(src/application)

include_directories(/Library/Java/JavaVirtualMachines/jdk1.8.0_341.jdk/Contents/Home/include/)
include_directories(/Library/Java/JavaVirtualMachines/jdk1.8.0_341.jdk/Contents/Home/include/darwin)

include_directories(/Users/zhouyao/Library/Java/JavaVirtualMachines/openjdk-18.0.2.1/Contents/Home/include)
include_directories(/Users/zhouyao/Library/Java/JavaVirtualMachines/openjdk-18.0.2.1/Contents/Home/include/darwin)

include_directories(/usr/lib/jvm/java/include)
include_directories(/usr/lib/jvm/java/include/linux)

include_directories(/usr/local/protobuf/include)
include_directories(/usr/local/include)

#linux protobuf位置
link_directories(/usr/local/lib/)
link_directories(/usr/local/protobuf/lib/)
link_libraries(pthread)
link_libraries(protobuf)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCE "src/*")
add_executable(extractor ${SOURCE})

#动态链接库
#linux so/mac dylib/win dll
ADD_LIBRARY(extract SHARED ${SOURCE})


# openMP settings
#FIND_PACKAGE(OpenMP REQUIRED)
#if (OPENMP_FOUND)
#    message("OPENMP FOUND")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#endif ()

#FIND_PACKAGE(OpenMP REQUIRED)
#if(OPENMP_FOUND)
#    message("OPENMP FOUND")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()