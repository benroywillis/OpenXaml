#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "XamlObjects/XamlObject.h"
class Rectangle : public XamlObject
{
public:
	int Width = 100;
	int Height = 100;
	void Draw(float xmin, float xmax, float ymin, float ymax);
	Rectangle();
};
#endif