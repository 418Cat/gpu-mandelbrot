// Enable operator overloading
// on Imgui types
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "imgui.h"

#include <GLES3/gl3.h>
#include "ui/ui.h"
#include "shader.h"

double scroll_offset = 0;

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	scroll_offset = y_offset;
}

int setup_opengl(unsigned int* VAO)
{
	glfwSetScrollCallback(UI::window, scroll_callback);

	float vertices[]
	{
		 1.f,  1.f,   1.f, 1.f,
		 1.f, -1.f,   1.f, 0.f,
		-1.f, -1.f,   0.f, 0.f,
		-1.f,  1.f,   0.f, 1.f,
	};

	unsigned int indices[]
	{
		0, 1, 3,
		1, 2, 3,
	};

	unsigned int VBO, EBO;
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	return 0;
}

int main()
{
	UI::ui_init();
	UI::show_ui = true;

	unsigned int VAO;

	setup_opengl(&VAO);

	std::cout << "Building shaders" << std::endl;

	Shader shaders[] = 
	{
		Shader("src/shaders/vs.glsl", "src/shaders/fs.glsl"),
		Shader("src/shaders/vs.glsl", "src/shaders/fs_double.glsl"),
		Shader("src/shaders/vs.glsl", "src/shaders/fs_square_z_check.glsl"),
		Shader("src/shaders/vs.glsl", "src/shaders/fs_no_square_root.glsl"),
		Shader("src/shaders/vs.glsl", "src/shaders/fs_derivative.glsl"),
		Shader("src/shaders/vs.glsl", "src/shaders/fs_derivative_2.glsl"),
		Shader("src/shaders/vs.glsl", "src/shaders/fs_continuous.glsl"),
	};

	const char* shader_descriptions[] = 
	{
		"float precision",
		"double precision",
		"Square check if z in +-2",
		"No square root check if |z| < 2",
		"Derivative bailout",
		"Other derivative bailout",
		"Continuous coloring"
	};

	int shader_index = 0;

	shaders[shader_index].use();

	std::cout << "Shaders built" << std::endl;

	bool frametime_killswitch = true;

	int window_x, window_y;
	float aspect_ratio;

	float max_iter = 10.f;

	double zoom = 1.;
	double center_x, center_y;
	center_x = -.5;
	center_y = 0.00000000000000000000000000001;

	ImVec2 mouse_coords = ImVec2(0., 0.);
	int mouse_state = 0;

	ImVec2 u_z = ImVec2(0., 0.);

	float bailout_radius = 2.;

	// If set to true, 4 floats will be
	// passed to represent x&y center
	// instead of 2
	bool double_precision = false;

	while(UI::ui_is_shown())
	{
		glfwSetTime(0.);

		UI::ui_render_start();

		shaders[shader_index].use();

		double m_x, m_y;
		glfwGetCursorPos(UI::window, &m_x, &m_y);
		glfwGetFramebufferSize(UI::window, &window_x, &window_y);
		aspect_ratio = (float)window_x/(float)window_y;

		int mouse_state_now = glfwGetMouseButton(UI::window, GLFW_MOUSE_BUTTON_LEFT);
		ImVec2 m_now = ImVec2(m_x, m_y);

		// zoom = +- 10% of current zoom
		zoom *= 1. + .1*scroll_offset;
		scroll_offset = 0.;

		// Add Δmouse divided by zoom and normalized by aspect ratio
		// (and x2 for some reason)
		if(mouse_state_now == GLFW_PRESS && !UI::io->WantCaptureMouse)
		{
			center_x += (mouse_coords.x - m_now.x) * 2. / (zoom*window_x);
			center_y -= (mouse_coords.y - m_now.y) * 2. / (zoom*(double)aspect_ratio*window_y);
		}
		mouse_state = mouse_state_now;
		mouse_coords = m_now;

		ImGui::Begin("Settings");
		ImGui::BeginTabBar("aaaaa");
		
		if(ImGui::BeginTabItem("Options"))
		{
			ImGui::InputDouble("Zoom", &zoom);

			ImGui::InputDouble("Center x", &center_x);
			ImGui::InputDouble("Center y", &center_y);

			ImGui::Spacing();
			ImGui::Spacing();
			
			ImGui::DragFloat("Max iterations", &max_iter, 1.f, 1.f);
			ImGui::DragFloat2("Z0", &u_z.x, 0.01f, -2.f, 2.f, "%.2f");
			ImGui::DragFloat("Bailout radius", &bailout_radius);

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Combo("Shader type", &shader_index, shader_descriptions, sizeof(shader_descriptions)/8);

			ImGui::Checkbox("Double precision", &double_precision);
			ImGui::SameLine();
			ImGui::Checkbox("Frametime killswitch", &frametime_killswitch);

			if(ImGui::Button("Reset view"))
			{
				zoom = 1.;
				center_x = -.5;
				// Needs to not be 0 because of log
				center_y = 0.00000000000000000000000000001;
			}
			ImGui::EndTabItem();
		}

		float center_x_0 = floor(log(abs(center_x))); 
		float center_x_1 = center_x / pow(10., center_x_0);

		float center_y_0 = floor(log(abs(center_y))); 
		float center_y_1 = center_y / pow(10., center_y_0);

		if(ImGui::BeginTabItem("Debug"))
		{
			ImGui::Text("Center:   %f    %f", center_x, center_y);
			
			ImGui::Separator();
			ImGui::Text("Double Precision: separate exponant and mantissa");
			ImGui::Text("Center x:  %f  x10^  %f", center_x_1, center_x_0);
			ImGui::Text("Log10(%f) = %f", center_x, center_x_0);
			ImGui::Text("Center y:  %f  x10^  %f", center_y_1, center_y_0);
			
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();


		// set Uniforms
		shaders[shader_index].setFloat("u_aspect_ratio", aspect_ratio);

		// Tells the shader if we're using the vec center_x and y
		shaders[shader_index].setBool("u_double_precision", double_precision);

		shaders[shader_index].setVec2("u_center_x", center_x_0, center_x_1);
		shaders[shader_index].setVec2("u_center_y", center_y_0, center_y_1);

		shaders[shader_index].setVec2("u_center", center_x, center_y); 
		shaders[shader_index].setFloat("u_zoom", zoom);

		shaders[shader_index].setFloat("u_bailout_radius", bailout_radius);
		shaders[shader_index].setFloat("u_max_iter", max_iter);
		shaders[shader_index].setVec2("u_z", u_z.x, u_z.y);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		double frametime = glfwGetTime();

		// Reset view to avoid freezing with bad settings
		if(frametime_killswitch)
		{
			if(frametime > .2)
			{
				max_iter = 10.;
				shader_index = 0;
			}
		}

		ImGui::Text("FPS: %.1f", 1./frametime);

		ImGui::End();
		UI::ui_render_stop();
	}

	UI::ui_cleanup();
	
	return 0;
}
