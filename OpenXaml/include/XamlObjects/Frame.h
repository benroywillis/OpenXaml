#ifndef FRAME_H
#define FRAME_H
#include "XamlObjects/XamlObject.h"
class Frame : public XamlObject
{
public:
	void Draw();
	int Width = 640;
	int Height = 480;
	Frame();
};
#endif