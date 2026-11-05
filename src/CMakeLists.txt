add_library(petal
    petal.cpp
    input/args_parser.cpp
)
add_library(petal::petal ALIAS petal)

target_sources(petal
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
        FILES
            "${PROJECT_SOURCE_DIR}/include/petal/petal.hpp"
            "${PROJECT_SOURCE_DIR}/include/petal/input/args_parser.hpp"
            "${PROJECT_SOURCE_DIR}/include/petal/input/console_input.hpp"
            "${PROJECT_SOURCE_DIR}/include/petal/output/ansi.hpp"
            "${PROJECT_SOURCE_DIR}/include/petal/output/print.hpp"
            "${PROJECT_SOURCE_DIR}/include/petal/output/text.hpp"
            "${PROJECT_SOURCE_DIR}/include/petal/ui/bool_input.hpp"
            "${PROJECT_SOURCE_DIR}/include/petal/ui/dialog_input.hpp"
            "${PROJECT_SOURCE_DIR}/include/petal/ui/multi_input.hpp"
)

target_compile_features(petal PUBLIC cxx_std_23)

target_compile_options(petal PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Wshadow -Wconversion>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
)
if(PETAL_WARNINGS_AS_ERRORS)
  target_compile_options(petal PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/WX>
    )
endif()

set_target_properties(petal PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME petal
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)
