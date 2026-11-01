cmake_minimum_required(VERSION 3.20)

project(MonBebeAudio
    VERSION 0.1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

# Remove resources.c from the automatically discovered sources
set(RESOURCE_SOURCE "${PROJECT_SOURCE_DIR}/src/resources.c")
list(REMOVE_ITEM APP_SOURCES "${RESOURCE_SOURCE}")

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
pkg_check_modules(WEBKIT REQUIRED IMPORTED_TARGET webkitgtk-6.0)

add_executable(app.exe ${APP_SOURCES})

target_link_libraries(app.exe PRIVATE
    PkgConfig::GTK4
    PkgConfig::WEBKIT
)

# Require and compile resources.c only in Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(NOT EXISTS "${RESOURCE_SOURCE}")
        message(FATAL_ERROR
            "Missing src/resources.c!\n"
            "Please generate the GResource source before building Release.\n"
            "Run glib-compile-resources first."
        )
    endif()

    target_sources(app.exe PRIVATE "${RESOURCE_SOURCE}")
endif()

# Define DEBUG macro when building in Debug config
target_compile_definitions(app.exe PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)
