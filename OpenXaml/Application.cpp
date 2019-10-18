#include "OpenXaml/Application.h"
#include "OpenXaml/XamlEvents/XamlEvent.h"
#include "OpenXaml/XamlEvents/XamlEvents.h"
#include "OpenXaml/GL/GLConfig.h"
#include "OpenXaml/Environment/Environment.h"
#include "OpenXaml/Environment/Window.h"
#include <glad/glad.h>
#include <filesystem>
#include <iostream>
#include <codecvt>
#ifdef VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
namespace OpenXaml
{
	GLFWwindow* window;
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			if (action == GLFW_PRESS)
			{
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				Events::ClickEvent* e = new Events::ClickEvent(x, y);
				Events::EventQueue.push_back(e);
			}
		}
	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		
	}

	void textCallback(GLFWwindow* window, unsigned int codepoint)
	{
		char32_t unicodeInput = (char32_t)codepoint;
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
		std::string text = converter.to_bytes(unicodeInput);
		Events::TextEvent* e = new Events::TextEvent(text);
		Events::EventQueue.push_back(e);
	}

	Application::Application()
	{
		PixelScale = { 0.0f, 0.0f };
		Environment::window = new Environment::Window();
		frame = NULL;
		if (!glfwInit())
			throw 2;
		glfwWindowHint(GLFW_VISIBLE, 0);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		window = glfwCreateWindow(640, 480, "My Window", NULL, NULL);
		
		if (!window)
		{
			glfwTerminate();
			throw 2;
		}
		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) /* There was an error initilizing GLAD */
		{
			glfwTerminate();
			throw 2;
		}
		Environment::LoadEnvironment();
	}
	Application::~Application()
	{
		delete frame;
		glfwTerminate();
		Environment::ClearEnvironment();
	}

	void Application::Run()
	{
		glfwShowWindow(window);
		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			frame->Draw();
			glfwSwapBuffers(window);

			int width, height;
			glfwGetWindowSize(window, &width, &height);

			glViewport(0, 0, width, height);
			glfwPollEvents();
			Events::HandleEvents();
		}
		glfwHideWindow(window);
	}

	void Application::InitializeComponent(Objects::Frame* input)
	{
		frame = input;
		glfwSetWindowSize(window, frame->getWidth(), frame->getHeight());
		Environment::window->xScale = 2.0f / frame->getWidth();
		Environment::window->yScale = 2.0f / frame->getHeight();
		GL::LoadShaders();
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetKeyCallback(window, keyCallback);
		glfwSetCharCallback(window, textCallback);
		frame->Initialize();
	}
}