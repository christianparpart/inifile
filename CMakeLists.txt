cmake_minimum_required(VERSION 3.10)

project(inifile VERSION "0.0.0" LANGUAGES CXX)

set(MASTER_PROJECT OFF)
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	set(MASTER_PROJECT ON)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ${MASTER_PROJECT})

if(("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
	# Tell me all about any (useful) compiler warnings. (GCC, Clang)
	add_compile_options(-Wall)
	add_compile_options(-Wextra)
elseif(DEFINED MSVC)
	# on MSVC, min() and max() are preprocessor variables, hence, we have to disable those macros.
	add_definitions(-DNOMINMAX)
endif()

add_library(inifile INTERFACE)
target_sources(inifile INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/inifile.h)

target_include_directories(inifile INTERFACE
	$<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>)
target_compile_features(inifile INTERFACE cxx_std_17)

option(INIFILE_TESTING "inifile: Enable building testing programs." ${MASTER_PROJECT})
if (INIFILE_TESTING)
	set(CATCH_INSTALL_DOCS OFF CACHE BOOL "Catch2: Install documentation.")
	set(CATCH_INSTALL_HELPERS OFF CACHE BOOL "Catch2: Install helpers.")
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/catch2")

	enable_testing()
	add_executable(inifile_test
		test/test_main.cpp
		test/test_inifile.cpp)
	target_link_libraries(inifile_test Catch2::Catch2 inifile)
endif()
