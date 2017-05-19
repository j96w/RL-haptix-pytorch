cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

########################################
# Find the haptix_comm library
find_package(haptix-comm QUIET REQUIRED)
if (NOT haptix-comm_FOUND)
  BUILD_ERROR ("Missing: Haptix Comm (libhaptix-comm-dev)")
endif()


find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

AUX_SOURCE_DIRECTORY(. DM_SRCS1)
SET(LUA_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/DM/src/DM/lua)

INCLUDE_DIRECTORIES(${LUA_INCLUDE_PATH})

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${HAPTIX-COMM_CFLAGS}")
include_directories(${HAPTIX-COMM_INCLUDE_DIRS})
link_directories(${HAPTIX-COMM_LIBRARY_DIRS})
include_directories(${LUAJIT_INCLUDE_DIR})
link_directories(${LUAJIT_LIBRARY})


SET (LIBHELLO_SRC hx_ctl.c)


########################################
#add_executable(hx_controller hx_controller.c)
#target_link_libraries(hx_controller ${HAPTIX-COMM_LIBRARIES} m)
ADD_LIBRARY (armctl SHARED ${LIBHELLO_SRC})
target_link_libraries(armctl ${HAPTIX-COMM_LIBRARIES} m)
target_link_libraries(armctl ${GAZEBO_LIBRARIES} m)
