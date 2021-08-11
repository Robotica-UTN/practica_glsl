set(GLAD_SOURCES
  glad.c
  glad.h)
add_library(glad STATIC
  ${GLAD_SOURCES})
target_include_directories(glad
  PUBLIC .)