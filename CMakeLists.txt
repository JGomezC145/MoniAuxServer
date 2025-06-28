cmake_minimum_required(VERSION 3.28.3)
project(MoniAuxServer C)

set(CMAKE_C_STANDARD 11)


# Añadir directorio de headers
include_directories(include)

# Añadir directorio de iconos
file(GLOB ICONS "resources/*.png")


# Buscar GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(MoniAuxServer
        include/gui.h
        src/gui.c
        include/serial.h
        src/serial.c
        src/keysim.c
        include/keysim.h
        src/main.c
        include/keymap.h
        src/keymap.c
        resources/icon.png
        include/icon.h
)

# Vincular GTK
target_link_libraries(MoniAuxServer ${GTK3_LIBRARIES})
