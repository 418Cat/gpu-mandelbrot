add_library(
	imgui
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui.h
	imgui/imconfig.h
	imgui/imgui_internal.h
	imgui/backends/imgui_impl_glfw.h
	imgui/backends/imgui_impl_glfw.cpp
	imgui/backends/imgui_impl_opengl3_loader.h
	imgui/backends/imgui_impl_opengl3.h
	imgui/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(
	imgui PUBLIC
	libGL.so.1
	glfw	
)

target_include_directories(
	imgui PUBLIC
	imgui
)
