cmake_minimum_required(VERSION "3.5")

project("Stars" VERSION "1.0")

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

set(DEPENDENCIES_SOURCE_DIR "${PROJECT_SOURCE_DIR}/dependencies")

add_library(json_lib STATIC
	${DEPENDENCIES_SOURCE_DIR}/src/json/jsoncpp.cpp
)

target_include_directories(json_lib
	PUBLIC 
		${DEPENDENCIES_SOURCE_DIR}/include
)

add_library(stars_lib STATIC
	"src/boardAnalyse.cpp"
	"src/boardInterface.cpp"
	"src/boardRecord.cpp"
	"src/boardState.cpp"
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

# Install - quite a failure
# Binaries
install (TARGETS ${PROJECT_NAME}
	DESTINATION bin
)

include(CPack)
