#include "ui.h"
#include "imgui.h"
#include <iostream>

GLFWwindow* UI::window;
bool 		UI::show_ui;
bool		UI::is_init = false;
bool		UI::render_started = false;
ImGuiIO*	UI::io;


// Must be called to setup glfw && ImGui context
UI_ERROR UI::ui_init()
{
	// imgui & glfw init 
	glfwSetErrorCallback(glfw_error_callback);

	if(!glfwInit())
	{
		std::cerr << "Error while calling glfwInit()" << std::endl;
		glfwTerminate();
		return ERR_INIT_GLFW;
	}

	const char* glfw_version = "#version 130";
	// OpenGL 3.3 Core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "", NULL, NULL);
	
	if(window == nullptr)
	{
		std::cerr << "Error when creating glfw window"  << std::endl;
		glfwTerminate();
		return ERR_INIT_WINDOW;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	io = &ImGui::GetIO();
	io->FontGlobalScale = 3.f;
	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	is_init = true;
	return OK;
}

UI_ERROR UI::ui_render_start()
{
	// Forgot to call ui_init()
	if(!is_init)
	{
		std::cerr
			<< "Error: ui_render_start() has been called before ui_init()"
			<< std::endl;
		return ERR_NOT_INIT;
	}

	// ui_render_start() has been called without ui_render_stop()
	if(render_started)
	{
		std::cerr
			<< "Error: ui_render_start() has been called twice without "
			<< "ui_render_stop() in between"
			<< std::endl;

		if(!UI::ui_render_stop()) render_started = false;
		return ERR_RENDER_START_TWICE;
	}
	
	// Don't show UI
	if(!show_ui) return WARN_NOT_SHOW_UI;

	render_started = true;

	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 1.f);

	return OK;
}

UI_ERROR UI::ui_render_stop()
{
	if(!is_init)
	{
		std::cerr
			<< "Error: ui_render_start() has been called before ui_init()"
			<< std::endl;
		return ERR_NOT_INIT;
	}

	// Check if there is a frame to end cleanly
	if(!show_ui)
	{
		/*
		 * ui_render_start() called correctly but `show_ui` was 
		 * set to false before this ui_render_stop() call
		 */
		if(render_started)
		{
			std::clog
				<< "Warning: `show_ui` has been set to false between "
				<< "ui_render_start() and ui_render_stop()"
				<< std::endl;
		}
		// If not, nothing to do
		else return WARN_NOT_SHOW_UI;
	}

	if(!render_started)
	{
		std::cerr
			<< "Error: ui_render_stop() has been called before "
			<< "ui_render_start()"
			<< std::endl;
		return ERR_RENDER_STOP_WITHOUT_START;
	}
	
	ImGui::Render();

	// Update display size
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);

	render_started = false;
	return OK;
}

// Release resources
UI_ERROR UI::ui_cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return OK;
}

// If the ui window is still running 
bool UI::ui_is_shown()
{
	return
		!glfwWindowShouldClose(window) &&
		is_init &&
		show_ui;
}
