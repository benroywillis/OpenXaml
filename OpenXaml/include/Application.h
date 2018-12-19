#ifndef APPLICATION_H
#define APPLICATION_H
#include "GL/Font.h"
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <GLFW/glfw3.h>
#include "XamlObjects/Frame.h"
#include "Parser/Parser.h"
#include "XamlObjects/Coordinate.h"

namespace OpenXaml
{
	class Application
	{
	public:
		Application();
		~Application();
		void Run();
		void InitializeComponent();
		void InitializeComponent(string file);
	private:
		FT_Library fontLibrary;
		void GetFonts();
		map<string, vector<string>> fontFileMap;
		GLFWwindow *window;
		shared_ptr<OpenXaml::Frame> frame;
		coordinate PixelScale;
	};
}

#endif