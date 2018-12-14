#include "Engine.h"
#include "Window.h"
#include <assert.h>
#include "../files/LogHandler.h"
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"

Big::Engine::Engine()
{
}


Big::Engine::~Engine()
{
}

bool Big::Engine::Create()
{
	bool success = true;
	LogHandler* logHandler = LogHandler::CreateInstance(logPath);
	
	success &= logHandler->CreateLog(engineLog);
	if (!success)
	{

		return false;
	}

	success &= Window::InitializeRenderSystem();
	if (!success)
		return false;

	Window::Settings windowSettings;
	windowSettings.width = 1024;
	windowSettings.height = 768;
	windowSettings.title = "Big Engine";
	windowSettings.backgroundColor = Color(1.f, 0.5f, 0.f);

	assert(window == nullptr);
	window = new Window();
	success &= window->Create(windowSettings);
	if (!success)
	{
		LogHandler::DoLog("Failed to create window.", LogFile::LogType::Error);
		return false;
	}

	shaderProgram = new ShaderProgram();
	success &= shaderProgram->Create();
	if (success)
	{
		success &= shaderProgram->LoadShader(defaultVertexShader, ShaderProgram::ShaderType::Vertex);
		success &= shaderProgram->LoadShader(defaultFragmentShader, ShaderProgram::ShaderType::Fragment);
	}

	return success;
}

void Big::Engine::Destroy()
{
	if (shaderProgram)
	{
		shaderProgram->Destroy();
		delete shaderProgram;
		shaderProgram = nullptr;
	}

	if (window)
	{
		window->Destroy();
		delete window;
		window = nullptr;
	}

	Window::DeInitializeRenderSystem();
	LogHandler::DestroyInstance();
}

void Big::Engine::Run()
{
	GLfloat vertices[] =
	{
		//Position				Color
		//x		y		z	|	r		g		b
		0.0f,	0.5f,	0.0f,	1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,	0.0f,	0.0f,	1.0f,	0.0f,
		-0.5,	-0.5,	0.0f,	0.0f,	0.0f,	1.0f
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, nullptr);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	

	while (!window->Closing())
	{
		window->BeginRender();

		//Render stuff here
		shaderProgram->Use();
		glBindVertexArray(vao);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		window->EndRender();

		//Todo: Create Input Handler.
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
