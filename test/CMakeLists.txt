find_package(CUnit REQUIRED)

set (TEST_TARGET "tree_test")

set (SOURCES
  test.c
)

include_directories(
  ../include
  ${PROJECT_BINARY_DIR}/include
  ${CUnit_INCLUDE_DIRS}
)

add_executable(${TEST_TARGET} ${SOURCES})
target_link_libraries(${TEST_TARGET} ${TARGET} ${CUnit_LIBRARIES})

add_test(test ${TEST_TARGET})
