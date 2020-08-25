# prepare
cmake_minimum_required(VERSION "3.5")
set(STARS_VERSION_DEBUG "0.1.1-beta")
set(STARS_VERSION_RELEASE "0.1.1")
project("Stars" VERSION ${STARS_VERSION_RELEASE})
set(DEPENDENCIES_SOURCE_DIR "${PROJECT_SOURCE_DIR}/dependencies")

# main building session
add_library(json_lib STATIC
	${DEPENDENCIES_SOURCE_DIR}/src/json/jsoncpp.cpp
)
target_include_directories(json_lib
	PUBLIC 
		${DEPENDENCIES_SOURCE_DIR}/include
)

add_library(stars_lib STATIC
	src/boardAnalyse.cpp
	src/boardInterface.cpp
	src/boardRecord.cpp
	src/boardRoute.cpp
	src/boardState.cpp
	src/shortList.cpp
)
target_include_directories(stars_lib
	PUBLIC 
		${PROJECT_SOURCE_DIR}/include
		${DEPENDENCIES_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME}
	main.cpp
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE 
		stars_lib
		json_lib
)

# set macros
## if windows, set or unset debug info by hand
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	message (STATUS "Current platform: Linux")
	if (NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Debug")
	endif()
	if (CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
		target_compile_definitions(stars_lib 
			PUBLIC STARS_DEBUG_INFO
		)
		add_definitions(-DSTARS_VERSION_DEBUG="${STARS_VERSION_DEBUG}")
		message (STATUS "Current version: ${STARS_VERSION_DEBUG}")
		message (STATUS "Current build type: (with debug info)")
	else () # release
		add_definitions(-DSTARS_VERSION_RELEASE="${STARS_VERSION_RELEASE}")
		message (STATUS "Current version: ${STARS_VERSION_RELEASE}")
		message (STATUS "Current build type: (without debug info)")
	endif()
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
	add_definitions(-DSTARS_VERSION_DEBUG="${STARS_VERSION_DEBUG}")
	message(STATUS "Current version: ${STARS_VERSION_DEBUG}")
	target_compile_definitions(stars_lib 
		PUBLIC STARS_DEBUG_INFO
	)
	message (STATUS "Current platform: Windows")
	message (STATUS "Current build type: (with debug info)")
endif ()

# Install - quite a failure
# Binaries
install (TARGETS ${PROJECT_NAME}
	DESTINATION bin
)
