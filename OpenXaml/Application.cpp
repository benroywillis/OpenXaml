#include "Application.h"
#include <filesystem>
#include <iostream>
namespace OpenXaml
{
	Application::Application()
	{
		FT_Init_FreeType(&(Application::fontLibrary));
		GetFonts();
	}
	Application::~Application()
	{
		FT_Done_FreeType(Application::fontLibrary);
	}

	void Application::GetFonts()
	{
		vector<string> fontDirectories;
		vector<string> fontFiles;
#if _WIN32
		fontDirectories.push_back("C:\\Windows\\Fonts");
#else
		cerr << "Unrecognized platform. Loading no system fonts.\n";
#endif

		for (int i = 0; i < fontDirectories.size(); i++)
		{
			for (const auto & entry : std::filesystem::directory_iterator(fontDirectories[i]))
			{
				fontFiles.push_back(entry.path().string());
			}
		}
		fontFileMap.clear();
		for (int i = 0; i < fontFiles.size(); i++)
		{
			string file = fontFiles[i];
			vector<string> validExtensions;
			validExtensions.push_back(".ttf");
			validExtensions.push_back(".TTF");
			validExtensions.push_back(".ttc");
			validExtensions.push_back(".otf");
			bool correct = false;
			for (int j = 0; j < validExtensions.size(); j++)
			{
				string extension = validExtensions[j];
				correct = 0 == file.compare(file.length() - extension.length(), extension.length(), extension);
				if (correct)
				{
					break;
				}
			}
			if (correct)
			{
				FT_Face face;
				auto error = FT_New_Face(Application::fontLibrary, file.c_str(), 0, &face);
				if (error)
				{
					std::cerr << "Failed to open " << file << "\n";
				}
				fontFileMap[face->family_name].push_back(file);
				FT_Done_Face(face);
			}
			
		}
	}
}