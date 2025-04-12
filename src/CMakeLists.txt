
add_executable(
	main
	main.cpp
)

add_subdirectory(ui)

target_link_libraries(
	main PUBLIC
	glfw
	libGL.so
	ui
)

file(
	COPY shaders
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
