#pragma once
#ifndef GLSHADER_H
#define GLSHADER_H

#include"Math.h"
#include<glad/glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

using namespace jkMath;

class glShader
{
public:
	//program ID;
	unsigned int ID;

	//construct Shader;
	glShader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = nullptr)
	{
		//step 1. get shader code in file path.
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;

		//exception for ifstream;
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderFile.open(vertexShaderPath);
			fShaderFile.open(fragmentShaderPath);
			std::stringstream vShaderStream, fShaderStream;
			//read file buffer into streams.
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			//get code string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			//if geometryShader is present.
			if (geometryShaderPath != nullptr)
			{
				gShaderFile.open(geometryShaderPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//step 2. compile.
		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);//param 3, const ptr point to (const char*).
		glCompileShader(vertex);
		checkCompileError(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileError(fragment, "FRAGMENT");

		unsigned int geometry;
		if (geometryShaderPath != nullptr)
		{
			const char* gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileError(geometry, "GEOMETRY");
		}

		//Shader program.
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		if (geometryShaderPath != nullptr)
		{
			glAttachShader(ID, geometry);
		}
		glLinkProgram(ID);
		checkCompileError(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geometryShaderPath != nullptr)
		{
			glDeleteShader(geometry);
		}
	}

	//use program;
	void use()
	{
		glUseProgram(ID);
	}

	//uniform set tools;
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setUInt(const std::string& name, unsigned int value) const
	{
		glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setMat4(const std::string& name, const MAT4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, &(mat.m[0][0]));// For GL column major, transpose.
	}
	void setVec3(const std::string& name, const VEC3& vec) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, vec.m);
	}
	void setVec3(const std::string& name, const float x, const float y, const float z)
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

private:
	void checkCompileError(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[512];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_FAILED of type: " << type << "\n" << infoLog << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 512, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_FAILED of type: " << type << "\n" << infoLog << std::endl;
			}
		}
	}
};
#endif // !GLSHADER_H
