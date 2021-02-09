cmake_minimum_required(VERSION "3.5")


# Prepare
## change these versions below to make a version change that applies to the whole
## program
set(STARS_VERSION_DEBUG "0.1.1-beta")
set(STARS_VERSION_RELEASE "0.1.1")
project("stars" VERSION ${STARS_VERSION_RELEASE})
if (NOT CMAKE_BUILD_TYPE)
	message("Set build type to 'Debug' as none was specified.")
	set(CMAKE_BUILD_TYPE "Debug")
endif()
if (NOT STARS_LANGUAGE_SETIING)
	message("Set language to 'English' as none was specified.")
	set(STARS_LANGUAGE_SETIING "English" CACHE STRING "Choose the language of program." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE STARS_LANGUAGE_SETIING PROPERTY STRINGS "English" "Chinese")
endif ()
set(DEPENDENCIES_SOURCE_DIR "${PROJECT_SOURCE_DIR}/dependencies")


# Main building session
add_library(libjson STATIC
	${DEPENDENCIES_SOURCE_DIR}/src/json/jsoncpp.cpp
)
target_include_directories(libjson
	PUBLIC
		${DEPENDENCIES_SOURCE_DIR}/include
)

add_library(libstarboard STATIC
	src/boardAnalyse.cpp
	src/boardInterface.cpp
	src/boardRecord.cpp
	src/boardRoute.cpp
	src/boardState.cpp
	src/boardTest.cpp
	src/tools.cpp
)
target_include_directories(libstarboard
	PUBLIC
		${PROJECT_SOURCE_DIR}/include
		${DEPENDENCIES_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME} ./src/main.cpp)

target_link_libraries(${PROJECT_NAME}
	PRIVATE 
		libstarboard
		libjson
)


# Set macros
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_compile_definitions(libstarboard
		PUBLIC STARS_PLATFORM_LINUX
	)
	message (STATUS "Current platform: Linux")
	if (CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
		target_compile_definitions(libstarboard 
			PUBLIC STARS_DEBUG_INFO
		)
		add_definitions(-DSTARS_VERSION_DEBUG="${STARS_VERSION_DEBUG}")
		message (STATUS "Current version: ${STARS_VERSION_DEBUG}")
		message (STATUS "Current build type: ${CMAKE_BUILD_TYPE} (with debug info)")
	else () # release
		add_definitions(-DSTARS_VERSION_RELEASE="${STARS_VERSION_RELEASE}")
		message (STATUS "Current version: ${STARS_VERSION_RELEASE}")
		message (STATUS "Current build type: ${CMAKE_BUILD_TYPE} (without debug info)")
	endif()
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
	################## change this to make a Debug version ###################
	# set(CMAKE_BUILD_TYPE "Debug")
	################## change this to make a release version ###################
	# set(CMAKE_BUILD_TYPE "Release")
	
	############### change this to make a English version ######################
	# set(STARS_LANGUAGE_SETIING "English" CACHE STRING "Choose the language of program." FORCE)
	############### change this to make a Chinese version ######################
	# set(STARS_LANGUAGE_SETIING "Chinese" CACHE STRING "Choose the language of program." FORCE)

	target_compile_definitions(libstarboard
		PUBLIC STARS_PLATFORM_WINDOWS
	)
	if (CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
		add_definitions(-DSTARS_VERSION_DEBUG="${STARS_VERSION_DEBUG}")
		message (STATUS "Current version: ${STARS_VERSION_DEBUG}")
		target_compile_definitions(libstarboard PUBLIC STARS_DEBUG_INFO)
		message (STATUS "Current build type: ${CMAKE_BUILD_TYPE} (with debug info)")
	else () # release and such
		add_definitions(-DSTARS_VERSION_RELEASE="${STARS_VERSION_RELEASE}")
		message (STATUS "Current version: ${STARS_VERSION_RELEASE}")
		message (STATUS "Current build type: ${CMAKE_BUILD_TYPE} (without debug info)")
	endif()
	message (STATUS "Current platform: Windows")
else() # Darwin maybe
	message (STATUS "Current platform: ${CMAKE_SYSTEM_NAME}")
	target_compile_definitions(libstarboard
		PUBLIC STARS_PLATFORM_LINUX # will use linux for mac and others for now
	)
	if (CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
		target_compile_definitions(libstarboard 
			PUBLIC STARS_DEBUG_INFO
		)
		add_definitions(-DSTARS_VERSION_DEBUG="${STARS_VERSION_DEBUG}")
		message (STATUS "Current version: ${STARS_VERSION_DEBUG}")
		message (STATUS "Current build type: ${CMAKE_BUILD_TYPE} (with debug info)")
	else () # release
		add_definitions(-DSTARS_VERSION_RELEASE="${STARS_VERSION_RELEASE}")
		message (STATUS "Current version: ${STARS_VERSION_RELEASE}")
		message (STATUS "Current build type: ${CMAKE_BUILD_TYPE} (without debug info)")
		# try conditional compilation
		include(CheckCXXCompilerFlag)
		CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
		CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

		# check results and add flag
		if(COMPILER_SUPPORTS_CXX11)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
		elseif(COMPILER_SUPPORTS_CXX0X)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
		else()
			message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
		endif()
	endif()
endif ()
		
## set language
if (STARS_LANGUAGE_SETIING MATCHES "Chinese")
	target_compile_definitions(libstarboard PUBLIC STARS_LANG_CHINESE)
	message(STATUS "Program language: Chinese")
elseif (STARS_LANGUAGE_SETIING MATCHES "English")
	message(STATUS "Program language: English")
else ()
	message(WARNING "Wrong language settings")
endif()


# Install - quite a failure
install (TARGETS ${PROJECT_NAME}
	DESTINATION bin
)
