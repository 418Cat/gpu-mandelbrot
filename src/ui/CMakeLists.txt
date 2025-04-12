add_library(
	ui STATIC
	ui.cpp
	ui.h
)

target_include_directories(
	ui PUBLIC
	${CMAKE_SOURCE_DIR}/external/imgui
	${CMAKE_SOURCE_DIR}/external/imgui/backends
)

target_link_libraries(
	ui PUBLIC
	imgui
)
