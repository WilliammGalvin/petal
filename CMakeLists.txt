cmake_minimum_required(VERSION 3.23)

project(petal
    VERSION 0.1.0
    DESCRIPTION "Pretty CLI library for C++23"
    LANGUAGES CXX
)

option(PETAL_BUILD_EXAMPLES
  "Build the example programs" ${PROJECT_IS_TOP_LEVEL})

option(PETAL_INSTALL
  "Generate install and export rules" ${PROJECT_IS_TOP_LEVEL})

option(PETAL_WARNINGS_AS_ERRORS
  "Treat compiler warnings as errors" OFF)

if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
  endif()
endif()

add_subdirectory(src)

if(PETAL_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(PETAL_INSTALL)
  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)

  set(PETAL_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/petal")

  install(TARGETS petal
        EXPORT petalTargets
        FILE_SET HEADERS
    )

  install(EXPORT petalTargets
        NAMESPACE petal::
        DESTINATION "${PETAL_INSTALL_CMAKEDIR}"
    )

  configure_package_config_file(
        cmake/petal_config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/petal-config.cmake"
        INSTALL_DESTINATION "${PETAL_INSTALL_CMAKEDIR}"
    )

  write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/petal-config-version.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

  install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/petal-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/petal-config-version.cmake"
        DESTINATION "${PETAL_INSTALL_CMAKEDIR}"
    )
endif()
