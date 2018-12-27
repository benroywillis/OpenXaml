#include "Environment.h"
#include "GL/Font.h"
#include <filesystem>
#include <iostream>
using namespace OpenXaml;
using namespace std;
Environment::Environment()
{
    FT_Init_FreeType(&(Environment::fontLibrary));
    LoadFonts();
}

Environment::~Environment()
{
	for (auto const&[key, val] : fontMap)
	{
		delete val;
	}
    FT_Done_FreeType(Environment::fontLibrary);
}

void Environment::LoadFonts()
{
    vector<string> fontDirectories;
    vector<string> fontFiles;
#if _WIN32
    fontDirectories.push_back("C:\\Windows\\Fonts");
#elif __linux__
    fontDirectories.push_back("/usr/share/fonts");
    fontDirectories.push_back("/usr/local/share/fonts");
#else
    cerr << "Unrecognized platform. Loading no system fonts.\n";
#endif

    for (int i = 0; i < fontDirectories.size(); i++)
    {
        for (const auto &entry : std::filesystem::directory_iterator(fontDirectories[i]))
        {
            fontFiles.push_back(entry.path().string());
        }
    }
    fontFileMap.clear();
    vector<string> validExtensions;
    validExtensions.push_back(".ttf");
    validExtensions.push_back(".TTF");
    validExtensions.push_back(".ttc");
    validExtensions.push_back(".otf");
    for (int i = 0; i < fontFiles.size(); i++)
    {
        string file = fontFiles[i];
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
            auto error = FT_New_Face(fontLibrary, file.c_str(), 0, &face);
            if (error)
            {
                std::cerr << "Failed to open " << file << "\n";
            }
			fontFileMap[{
				face->family_name,
				(face->style_flags & FT_STYLE_FLAG_BOLD) == FT_STYLE_FLAG_BOLD,
				(face->style_flags & FT_STYLE_FLAG_ITALIC) == FT_STYLE_FLAG_ITALIC
			}].push_back(file);
			
            FT_Done_Face(face);
        }
    }
}

Font* Environment::GetFont(FontProperties prop)
{
	auto index = fontMap.find(prop);
	if (index == fontMap.end())
	{
		vector<string> fontFiles = fontFileMap[{prop.name, prop.bold, prop.italic}];
		fontMap[prop] = new Font(fontLibrary, fontFiles[0], prop.size);
	}	
	return fontMap[prop];
}