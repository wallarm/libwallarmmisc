# - Try to find CUnit library
# Once done this will define
#  CUnit_FOUND - System has CUnit
#  CUnit_INCLUDE_DIRS - The CUnit include directories
#  CUnit_LIBRARIES - The libraries needed to use CUnit

find_path(CUnit_INCLUDE_DIR CUnit/Basic.h)
find_library(CUnit_LIBRARY NAMES cunit)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  CUnit DEFAULT_MSG CUnit_LIBRARY CUnit_INCLUDE_DIR)
mark_as_advanced(CUnit_INCLUDE_DIR CUnit_LIBRARY)
set(CUnit_LIBRARIES ${CUnit_LIBRARY})
set(CUnit_INCLUDE_DIRS ${CUnit_INCLUDE_DIR})
