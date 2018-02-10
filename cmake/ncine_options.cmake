option(NCINE_BUILD_TESTS "Build the engine test programs" ON)
option(NCINE_BUILD_UNIT_TESTS "Build the engine unit tests" OFF)
option(NCINE_INSTALL_DEV_SUPPORT "Install files to support development" ON)
option(NCINE_EXTRA_OPTIMIZATION "Compile the engine with extra optimizations in release mode" ON)
option(NCINE_AUTOVECTORIZATION_REPORT "Enable report generation from compiler auto-vectorization" OFF)
option(NCINE_DYNAMIC_LIBRARY "Compile the engine as a dynamic library" ON)
option(NCINE_BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" OFF)
option(NCINE_EMBED_SHADERS "Export shader files to C strings to be included in engine sources" ON)
option(NCINE_BUILD_ANDROID "Build the Android version of the engine" ON)

set(NCINE_DATA_DIR "${PARENT_SOURCE_DIR}/nCine-data" CACHE PATH "Set the path to the engine data directory")
set(NCINE_TESTS_DATA_DIR "" CACHE STRING "Set the path to the data directory that will be embedded in test executables")
# The external Android dir is set regardless of the status of build Android flag, so that presets work even when the flag is off
set(EXTERNAL_ANDROID_DIR "${PARENT_SOURCE_DIR}/nCine-android-external" CACHE PATH "Set the path to the Android libraries directory")
if(MSVC)
	set(EXTERNAL_MSVC_DIR "${PARENT_SOURCE_DIR}/nCine-external" CACHE PATH "Set the path to the MSVC libraries directory")
endif()

if(NCINE_BUILD_DOCUMENTATION)
	option(NCINE_IMPLEMENTATION_DOCUMENTATION "Include implementation classes in the documentation" OFF)
endif()

if(NCINE_BUILD_ANDROID)
	set(NDK_DIR "" CACHE PATH "Set the path to the Android NDK")
	set(NCINE_NDK_ARCHITECTURES armeabi-v7a CACHE STRING "Set the NDK target architectures")
	option(NCINE_ASSEMBLE_APK "Assemble the Android APK of the startup test with Gradle" OFF)
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	option(NCINE_ADDRESS_SANITIZER "Enable the AddressSanitizer memory error detector of GCC and Clang" OFF)
	option(NCINE_CODE_COVERAGE "Enable gcov instrumentation for testing code coverage" OFF)
endif()
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	option(NCINE_GCC_HARDENING "Enable memory corruption mitigation methods of GCC" OFF)
endif()

set(NCINE_STARTUP_TEST "apptest_camera" CACHE STRING "Set the starting test project and default running executable")

# nCine options presets
set(NCINE_OPTIONS_PRESETS "Default" CACHE STRING "Presets for CMake options")
set_property(CACHE NCINE_OPTIONS_PRESETS PROPERTY STRINGS Default BinDist DevDist)

if(NCINE_OPTIONS_PRESETS STREQUAL BinDist OR NCINE_OPTIONS_PRESETS STREQUAL DevDist)
	message(STATUS "Options presets: ${NCINE_OPTIONS_PRESETS}")

	set(CMAKE_BUILD_TYPE Release)
	set(CMAKE_CONFIGURATION_TYPES Release)
	set(NCINE_BUILD_TESTS ON)
	set(NCINE_BUILD_UNIT_TESTS OFF)
	set(NCINE_EXTRA_OPTIMIZATION ON)
	set(NCINE_AUTOVECTORIZATION_REPORT OFF)
	set(NCINE_DYNAMIC_LIBRARY ON)
	set(NCINE_IMPLEMENTATION_DOCUMENTATION OFF)
	set(NCINE_EMBED_SHADERS ON)
	set(TESTS_DATA_DIR_DIST ON)
	set(NCINE_STARTUP_TEST apptest_camera)
	if(DEFINED NCINE_ADDRESS_SANITIZER)
		set(NCINE_ADDRESS_SANITIZER OFF)
	endif()
	if(DEFINED NCINE_CODE_COVERAGE)
		set(NCINE_CODE_COVERAGE OFF)
	endif()
	if(DEFINED NCINE_GCC_HARDENING)
		set(NCINE_GCC_HARDENING ON)
	endif()
endif()

if(NCINE_OPTIONS_PRESETS STREQUAL BinDist)
	set(NCINE_INSTALL_DEV_SUPPORT OFF)
	set(NCINE_BUILD_ANDROID OFF)
	set(NCINE_BUILD_DOCUMENTATION OFF)
elseif(NCINE_OPTIONS_PRESETS STREQUAL DevDist)
	set(NCINE_INSTALL_DEV_SUPPORT ON)
	set(NCINE_BUILD_ANDROID ON)
	set(NCINE_ASSEMBLE_APK OFF)
	set(NCINE_NDK_ARCHITECTURES armeabi-v7a arm64-v8a x86_64)
	set(NCINE_BUILD_DOCUMENTATION ON)
endif()
