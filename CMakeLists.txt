CMAKE_MINIMUM_REQUIRED(VERSION 3.3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
	mandelbrot_shader
	VERSION 0.1
	LANGUAGES CXX
)

add_subdirectory(external)
add_subdirectory(src)
