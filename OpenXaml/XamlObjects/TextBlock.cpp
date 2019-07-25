#include "XamlObjects/TextBlock.h"
#include "Properties/Alignment.h"
#include "Properties/TextWrapping.h"
#include "Environment.h"
#include <sstream>
#include <algorithm>
#include <glad/glad.h>
#include <gl/GLConfig.h>

namespace OpenXaml {
	void TextBlock::Draw()
	{
		this->Update();
		glBindVertexArray(TextBlock::VAO);
		glUseProgram(GL::xamlShader);
		int vertexColorLocation = glGetUniformLocation(GL::xamlShader, "thecolor");
		int modeLoc = glGetUniformLocation(GL::xamlShader, "mode");
		glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
		glUniform1i(modeLoc, 2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeBuffer);
		for (int i = 0; i < vertexBuffers.size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
			glBindTexture(GL_TEXTURE_2D, textureMap[vertexBuffers[i]]);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		}
	}
	void TextBlock::Initialize()
	{
		glGenVertexArrays(1, &(TextBlock::VAO));
		glBindVertexArray(TextBlock::VAO);
		unsigned short indeces[] =
		{
			0,1,2,
			1,2,3
		};
		glGenBuffers(1, &edgeBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);
	}

	void TextBlock::Update()
	{
		Font* font = env.GetFont(FontProperties{ FontFamily, FontSize });
		glBindVertexArray(TextBlock::VAO);
		for (int i = 0; i < vertexBuffers.size(); i++)
		{
			glDeleteBuffers(1, &(vertexBuffers[i]));
		}
		vertexBuffers.clear();
		if (font == NULL)
		{
			return;
		}
		string text = Text;

		vector<int> widths;
		vector<int> seperators;
		static const char splitChars[] = { ' ', '-', '\t', '\n' };
		int width = 0;
		for (int i = 0; i < text.length(); i++)
		{
			width += font->operator[](text[i]).AdvanceX >> 6;
			if (find(begin(splitChars), end(splitChars), text[i]) != end(splitChars))
			{
				widths.push_back(width);
				seperators.push_back(i);
				width = 0;
			}
		}
		if (width != 0)
		{
			widths.push_back(width);
			seperators.push_back((int)text.length());
		}

		vector<int> line;
		vector<vector<int>> lines;
		float fBounds = (maxCoord.x - minCoord.x) / PixelScale.x;
		width = 0;
		for (int i = 0; i < widths.size(); i++)
		{
			char seperator = text[seperators[i]];
			width += widths[i];
			if (width > fBounds && TextWrapping == TextWrapping::Wrap)
			{
				lines.push_back(line);
				line.clear();
				width = widths[i];
			}
			line.push_back(i);
			if (seperator == '\n')
			{
				lines.push_back(line);
				line.clear();
				width = 0;
			}
		}
		lines.push_back(line);
		int height = (font->Height >> 6);
		//we now have our line seperation, so now we render each line.

		int maxWidth = 0;
		vector<int> lineWidths;
		for (int i = 0; i < lines.size(); i++)
		{
			line = lines[i];
			width = 0;
			for (int j = 0; j < line.size(); j++)
			{
				width += widths[line[j]];
			}
			lineWidths.push_back(width);
			maxWidth = std::max(width, maxWidth);
			//we now have line width and can justify accordingly
		}
		boxWidth = maxWidth;
		boxHeight = height * (int)lines.size();
		//but first we get the framing rectangle's coordinates

		coordinate frameMax = { 0,0 };
		coordinate frameMin = { 0,0 };

		float fHeight = height * PixelScale.y;
		float fWidth = maxWidth * PixelScale.x;

		switch (VerticalAlignment)
		{
		case VerticalAlignment::Bottom:
		{
			frameMin.y = minCoord.y;
			frameMax.y = min(maxCoord.y, minCoord.y + fHeight);
			break;
		}
		case VerticalAlignment::Top:
		{
			frameMax.y = maxCoord.y;
			frameMin.y = max(minCoord.y, maxCoord.y - fHeight);
			break;
		}
		case VerticalAlignment::Center:
		{
			float mean = 0.5f * (maxCoord.y + minCoord.y);
			frameMax.y = min(maxCoord.y, mean + fHeight / 2);
			frameMin.y = max(minCoord.y, mean - fHeight / 2);
			break;
		}
		case VerticalAlignment::Stretch:
		{
			frameMax.y = maxCoord.y;
			frameMin.y = minCoord.y;
			break;
		}
		}

		switch (HorizontalAlignment)
		{
		case HorizontalAlignment::Left:
		{
			frameMin.x = minCoord.x;
			frameMax.x = min(maxCoord.x, minCoord.x + fWidth);
			break;
		}
		case HorizontalAlignment::Right:
		{
			frameMax.x = maxCoord.x;
			frameMin.x = max(minCoord.x, maxCoord.x - fWidth);
			break;
		}
		case HorizontalAlignment::Center:
		{
			float mean = 0.5f * (maxCoord.x + minCoord.x);
			frameMax.x = min(maxCoord.x, mean + fWidth / 2);
			frameMin.x = max(minCoord.x, mean - fWidth / 2);
			break;
		}
		case HorizontalAlignment::Stretch:
		{
			frameMax.x = maxCoord.x;
			frameMin.x = minCoord.x;
			break;
		}
		}

		//we now have the external rectangle dimensions, so we can actually position the text
		//start by setting the pen location
		float penX = 0;
		float penY = 0;
		penY = frameMax.y - fHeight;
		for (int i = 0; i < lineWidths.size(); i++)
		{
			float lineWidth = lineWidths[i] * PixelScale.x;
			switch (TextAlignment)
			{
			case TextAlignment::Center:
			{
				penX = (frameMin.x + frameMax.x) * 0.5f - lineWidth * 0.5f;
				break;
			}
			case TextAlignment::Right:
			{
				penX = (frameMax.x) - lineWidth;
				break;
			}
			case TextAlignment::Left:
			{
				penX = frameMin.x;
				break;
			}
			}
			//we now start rendering words
			line = lines[i];
			for (int j = 0; j < line.size(); j++)
			{
				int wordIndex = line[j];
				int length;
				int startIndex;
				if (wordIndex == 0)
				{
					startIndex = 0;
					length = seperators[wordIndex] - startIndex + 1;
				}
				else
				{
					startIndex = seperators[((long)wordIndex) - 1] + 1;
					length = seperators[wordIndex] - startIndex + 1;
				}
				string word = text.substr(startIndex, length);
				for (int k = 0; k < word.length(); k++)
				{
					char toRender = word[k];
					Character ch = font->operator[](toRender);
					if (!iscntrl(toRender))
					{
						float x0, x1, y0, y1, tx0, tx1, ty0, ty1;
						float dx0, dx1, dy0, dy1;
						dx0 = penX + ch.BearingX * PixelScale.x;
						dx1 = penX + (ch.BearingX + ch.Width) * PixelScale.x;
						dy0 = penY + ch.BearingY * PixelScale.y;
						dy1 = penY + (ch.BearingY - (int)ch.Height) * PixelScale.y;

						x0 = max(dx0, minCoord.x);
						x1 = min(dx1, maxCoord.x);
						y0 = max(dy0, minCoord.y);
						y1 = min(dy1, maxCoord.y);

						float dwidth = dx1 - dx0;
						float dheight = dy1 - dy0;
						float xRatio = (x1 - x0) / dwidth;
						float yRatio = (y1 - y0) / dheight;
						if (x0 != dx0)
						{
							tx0 = 1 - xRatio;
						}
						else
						{
							tx0 = 0;
						}
						if (x1 != dx1)
						{
							tx1 = xRatio;
						}
						else
						{
							tx1 = 1;
						}
						if (y0 != dy0)
						{
							ty0 = 1 - yRatio;
						}
						else
						{
							ty0 = 0;
						}
						if (y1 != dy1)
						{
							ty1 = yRatio;
						}
						else
						{
							ty1 = 1;
						}

						GLfloat vertices[16] = {
						x0, y0, tx0,ty0,
						x1, y0, tx1,ty0,
						x0, y1, tx0,ty1,
						x1, y1, tx1,ty1
						};
						GLuint sepBuffer;
						glGenBuffers(1, &sepBuffer);
						glBindBuffer(GL_ARRAY_BUFFER, sepBuffer);
						glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
						textureMap[sepBuffer] = ch.TextureID;
						vertexBuffers.push_back(sepBuffer);
					}
					penX += (ch.AdvanceX >> 6) * PixelScale.x;
				}
			}
			penY -= fHeight;
		}
	}

	TextBlock::TextBlock()
	{
		boxHeight = 0;
		boxWidth = 0;
		edgeBuffer = 0;
	}

	TextBlock::~TextBlock()
	{
		//glBindVertexArray(TextBlock::VAO);
		//glDeleteVertexArrays(1, &TextBlock::VAO);
	}

	void TextBlock::SetBoundingBox(coordinate min, coordinate max)
	{
		minCoord = min;
		maxCoord = max;
	}

	void TextBlock::setText(string text)
	{
		this->Text = text;
	}
	string TextBlock::getText()
	{
		return this->Text;
	}
	void TextBlock::setTextWrapping(OpenXaml::TextWrapping textWrapping)
	{
		this->TextWrapping = textWrapping;
	}
	TextWrapping TextBlock::getTextWrapping()
	{
		return this->TextWrapping;
	}
	void TextBlock::setFontFamily(string family)
	{
		this->FontFamily = family;
	}
	string TextBlock::getFontFamily()
	{
		return this->FontFamily;
	}
	void TextBlock::setFontSize(float size)
	{
		this->FontSize = size;
	}
	float TextBlock::getFontSize()
	{
		return this->FontSize;
	}
	void TextBlock::setFill(unsigned int fill)
	{
		this->Fill = fill;
	}
	unsigned int TextBlock::getFill()
	{
		return this->Fill;
	}
	void TextBlock::setTextAlignment(OpenXaml::TextAlignment alignment)
	{
		this->TextAlignment = alignment;
	}
	TextAlignment TextBlock::getTextAlignment()
	{
		return this->TextAlignment;
	}
	int TextBlock::getWidth()
	{
		return max(this->Width, (int)this->boxWidth);
	}
	int TextBlock::getHeight()
	{
		return max(this->Height, (int)this->boxHeight);
	}
}