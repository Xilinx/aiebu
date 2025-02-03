# --- version settings ---

set(AIEBU_INSTALL_DIR "aiebu")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(AIEBU_VERSION_RELEASE 202510)
SET(AIEBU_VERSION_MAJOR 1)
SET(AIEBU_VERSION_MINOR 0)

# Standard code snippet to identify parent project if we are a submodule
set(AIEBU_GIT_SUBMODULE FALSE)

find_package(Git)
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --show-superproject-working-tree
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_SUPERPROJECT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )

  if(GIT_SUPERPROJECT)
    message("-- Building AIEBU as a submodule of ${GIT_SUPERPROJECT}")
    set(AIEBU_GIT_SUBMODULE TRUE)
  endif()
endif()

set(AIEBU_SPECIFICATION_INSTALL_DIR "aiebu/share/specification")

if (DEFINED $ENV{AIEBU_VERSION_PATCH})
  SET(AIEBU_VERSION_PATCH $ENV{AIEBU_VERSION_PATCH})
else(DEFINED $ENV{AIEBU_VERSION_PATCH})
  SET(AIEBU_VERSION_PATCH 0)
endif(DEFINED $ENV{AIEBU_VERSION_PATCH})

# Also update cache to set version for external plug-in .so
set(AIEBU_SOVERSION ${AIEBU_VERSION_MAJOR} CACHE INTERNAL "")
set(AIEBU_VERSION_STRING ${AIEBU_VERSION_MAJOR}.${AIEBU_VERSION_MINOR}.${AIEBU_VERSION_PATCH} CACHE INTERNAL "")

# Some later versions of boost spews warnings form property_tree
# Default embedded boost is 1.74.0 which does spew warnings so
# making this defined global
add_compile_options("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")
