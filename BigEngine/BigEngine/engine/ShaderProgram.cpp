#include "ShaderProgram.h"
#include "../files/LogHandler.h"
#include "../helpers/FileHelper.h"
#include <GL/glew.h>
#include <assert.h>

Big::ShaderProgram::ShaderProgram()
{
}


Big::ShaderProgram::~ShaderProgram()
{
}

bool Big::ShaderProgram::Create()
{
	handle = glCreateProgram();
	return true;
}

void Big::ShaderProgram::Destroy()
{
	glDeleteProgram(handle);
	handle = 0;
}

void Big::ShaderProgram::Use()
{
	assert(handle > 0);
	glUseProgram(handle);
}

bool Big::ShaderProgram::LoadShader(const std::string& uri, ShaderType shaderType)
{
	bool success = true;
	std::string source;
	GLuint shaderHandle = 0;
	
	if (FileHelper::GetFileContent(uri, source))
	{
		switch (shaderType)
		{
		case Big::ShaderProgram::Vertex:
			shaderHandle = glCreateShader(GL_VERTEX_SHADER);
			break;
		case Big::ShaderProgram::Fragment:
			shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			return false;
			break;
		}

		const GLchar* glSource = source.c_str();
		glShaderSource(shaderHandle, 1, &glSource, nullptr);
		glCompileShader(shaderHandle);
		if (!CheckShaderError(handle))
		{
			glAttachShader(handle, shaderHandle);
			glLinkProgram(handle);
		}
	}

	return success;
}

bool Big::ShaderProgram::CheckShaderError(unsigned int shader)
{
	GLint status;
	glGetShaderiv(handle, GL_LINK_STATUS, &status);

	if (!status)
	{
		GLint messageLength = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &messageLength);
		std::string errorMessage(messageLength, ' ');
		glGetShaderInfoLog(handle, messageLength, &messageLength, &errorMessage[0]);
		LogHandler::DoLog("Shader failed to compile: " + errorMessage, LogFile::LogType::Error);
		return false;
	}

	return true;
}

bool Big::ShaderProgram::CheckProgramError()
{
	GLint status;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);
	
	if (!status)
	{
		GLint messageLength = 0;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &messageLength);
		std::string errorMessage(messageLength, ' ');
		glGetProgramInfoLog(handle, messageLength, &messageLength, &errorMessage[0]);
		LogHandler::DoLog("Shader program failed to link shader: " + errorMessage, LogFile::LogType::Error);
		return false;
	}

	return true;
}
