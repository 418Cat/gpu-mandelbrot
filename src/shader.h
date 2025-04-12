#ifndef SHADER_H_
#define SHADER_H_

#include <GLES3/gl3.h>
#include <fstream>
#include <iostream>

class Shader
{
	private:
		std::string get_shader(const char* filepath)
		{
			std::ifstream file(filepath);
	
			if(!file.is_open())
			{
				std::cerr << "Failed to get shader file \""
					<< filepath << "\"\n";
				return std::string();
			}

			std::string shader_file;
			std::string shader_line;

			while(std::getline(file, shader_line))
			{
				shader_file.append(shader_line);
				shader_file.append("\n");
			}

			file.close();

			return shader_file;
		}

	public:
		unsigned int ID;

		Shader(const char* vs_path, const char* fs_path)
		{
			// Get the shaders' files in a string
			std::string vs_code = get_shader(vs_path);
			std::string fs_code = get_shader(fs_path);

			// Convert it to a C-style string
			const char* vs_string = vs_code.c_str();
			const char* fs_string = fs_code.c_str();

			unsigned int vertex, fragment;
			int success;
			char infoLog[512];

			vertex = glCreateShader(GL_VERTEX_SHADER);
			fragment = glCreateShader(GL_FRAGMENT_SHADER);

			// Compile
			glShaderSource(vertex, 1, &vs_string, NULL);
			glCompileShader(vertex);

			// Get shader compile status
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				// Get shader compile log
				glGetShaderInfoLog(vertex, 512, NULL, infoLog);
				std::cerr
					<< "ERROR: Vertex shader compilation failed:\n"
					<< infoLog << std::endl;
			}

			// Compile
			glShaderSource(fragment, 1, &fs_string, NULL);
			glCompileShader(fragment);

			// Get compile status and print error if failed
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(fragment, 512, NULL, infoLog);
				std::cerr
					<< "ERROR: Fragment shader compilation failed:\n"
					<< infoLog << std::endl;
			}


			// Create the shader program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);

			// Get linking result and print error if needed
			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if(!success)
			{
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				std::cerr
					<< "ERROR: Shader program linkinparkg failed\n"
					<< infoLog
					<< std::endl;
			}

			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}

		void use()
		{
			glUseProgram(ID);
		}

		void setBool(const char* name, bool value) const
		{
			glUniform1i(glGetUniformLocation(ID, name), value);
		}

		void setInt(const char* name, int value) const
		{
			glUniform1i(glGetUniformLocation(ID, name), value);
		}

		void setFloat(const char* name, float value) const
		{
			glUniform1f(glGetUniformLocation(ID, name), value);
		}

		void setVec2(const char* name, float x, float y) const
		{
			glUniform2f(glGetUniformLocation(ID, name), x, y);
		}

		void setDvec2(const char* name, double x, double y) const
		{
			//glUniform2d(glGetUniformLocation(ID, name), x, y);
		}
};

#endif
