cmake_minimum_required (VERSION 2.8.8 FATAL_ERROR)
cmake_policy (VERSION 2.8)

project (libwallarmmisc C)

set (CMAKE_C_FLAGS "-Wall -std=gnu99 -O3 -g ${CMAKE_C_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

include(GNUInstallDirs)
include(CheckIncludeFile)
include(CTest)

check_include_file(sys/tree.h HAVE_SYS_TREE_H)

set (HEADERS_DIR include/wallarm)
set (CONFIG_HEADERS_DIR ${PROJECT_BINARY_DIR}/include/wallarm)

configure_file(
  "${HEADERS_DIR}/config.h.in" "${CONFIG_HEADERS_DIR}/config.h"
  ESCAPE_QUOTES)

install(DIRECTORY ${HEADERS_DIR} ${CONFIG_HEADERS_DIR}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING REGEX "^[^#]+\\.h$")

add_subdirectory(test)
