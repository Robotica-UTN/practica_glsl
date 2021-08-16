set(GLAD_SOURCES
  glad.c
  glad.h)
add_library(glad STATIC
  ${GLAD_SOURCES})
target_include_directories(glad
  PUBLIC .)
target_link_libraries(glad
GL X11 pthread Xrandr Xi dl)