#pragma once
#include "XamlObjects/XamlObject.h"

namespace OpenXaml {
	class Frame : public XamlObject
	{
	public:
		void Draw();
		Frame();
		void Initialize();
		void Update();
		~Frame();
		void setFill(unsigned int fill);
		unsigned int getFill();
	private:
		unsigned int vertexBuffer;
		unsigned int edgeBuffer;
	protected:
		unsigned int Fill = 0xFF000000;
	};
}
