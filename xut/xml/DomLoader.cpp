#include "xml/DomLoader.h"
#include <sstream>

using namespace xercesc;
using namespace OpenXaml;

XamlObject* LoadXaml(DOMElement* element)
{
	const XMLCh* xmlString = element->getTagName();
	string name = XMLString::transcode(xmlString);
	XamlObject* result;
	if (name == "Frame")
	{
		result = DomToFrame(element);
	}
	else if (name == "Button")
	{
		result = DomToButton(element);
	}
	else if (name == "Rectangle")
	{
		result = DomToRectangle(element);
	}
	else if (name == "TextBlock")
	{
		result = DomToTextBlock(element);
	}
	else
	{
		throw 2;
	}
}

Frame* DomToFrame(DOMElement* element)
{
	Frame* result = new Frame();
	DOMNamedNodeMap* attributes = element->getAttributes();
	for (int i = 0; i < attributes->getLength(); i++)
	{
		DOMNode* item = attributes->item(i);
		const XMLCh* nameXML = item->getNodeName();
		const XMLCh* valXML = item->getNodeValue();
		string name = XMLString::transcode(nameXML);
		string value = XMLString::transcode(valXML);
		if (name == "Fill")
		{
			std::istringstream iss(value.substr(1, value.size()));
			unsigned int val;
			iss >> std::hex >> val;
			result->setFill(val);
		}
		else if (name == "Height")
		{
			result->setHeight(stoi(value));
		}
		else if (name == "Width")
		{
			result->setWidth(stoi(value));
		}
	}
	size_t childCount = element->getChildElementCount();
	auto children = element->getChildNodes();
	for (int i = 0; i < childCount; i++)
	{
		auto child = children->item(i);
		result->Children.push_back(LoadXaml((DOMElement*)child));
	}
	return result;
}

Button* DomToButton(DOMElement* element)
{
	Button* result = new Button();

	DOMNamedNodeMap* attributes = element->getAttributes();
	for (int i = 0; i < attributes->getLength(); i++)
	{
		DOMNode* item = attributes->item(i);
		const XMLCh* nameXML = item->getNodeName();
		const XMLCh* valXML = item->getNodeValue();
		string name = XMLString::transcode(nameXML);
		string value = XMLString::transcode(valXML);
		if (name == "Height")
		{
			result->setHeight(stoi(value));
		}
		else if (name == "Width")
		{
			result->setWidth(stoi(value));
		}
		else if (name == "HorizontalAlignment")
		{
			if (value == "Right")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Right);
			}
			else if (value == "Left")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Left);
			}
			else if (value == "Center")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Center);
			}
			else if (value == "Stretch")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Stretch);
			}
		}
		else if (name == "VerticalAlignment")
		{
			if (value == "Top")
			{
				result->setVerticalAlignment(VerticalAlignment::Top);
			}
			else if (value == "Bottom")
			{
				result->setVerticalAlignment(VerticalAlignment::Bottom);
			}
			else if (value == "Center")
			{
				result->setVerticalAlignment(VerticalAlignment::Center);
			}
			else if (value == "Stretch")
			{
				result->setVerticalAlignment(VerticalAlignment::Stretch);
			}
		}
		else if (name == "Fill")
		{
			std::istringstream iss(value.substr(1, value.size()));
			unsigned int fill;
			iss >> std::hex >> fill;
			result->setFill(fill);
		}
		else if (name == "Content")
		{
			result->setContent(value);
		}
	}

	auto text = element->getTextContent();
	result->setContent(XMLString::transcode(text));
	size_t childCount = element->getChildElementCount();
	auto children = element->getChildNodes();
	for (int i = 0; i < childCount; i++)
	{
		auto child = children->item(i);
		result->Children.push_back(LoadXaml((DOMElement*)child));
	}
	return result;
}

OpenXaml::Rectangle* DomToRectangle(DOMElement* element)
{
	OpenXaml::Rectangle* result = new OpenXaml::Rectangle();

	DOMNamedNodeMap* attributes = element->getAttributes();
	for (int i = 0; i < attributes->getLength(); i++)
	{
		DOMNode* item = attributes->item(i);
		const XMLCh* nameXML = item->getNodeName();
		const XMLCh* valXML = item->getNodeValue();
		string name = XMLString::transcode(nameXML);
		string value = XMLString::transcode(valXML);
		if (name == "Fill")
		{
			std::istringstream iss(value.substr(1, value.size()));
			unsigned int fill;
			iss >> std::hex >> fill;
			result->setFill(fill);
		}
		else if (name == "Height")
		{
			result->setHeight(stoi(value));
		}
		else if (name == "Width")
		{
			result->setWidth(stoi(value));
		}
		else if (name == "HorizontalAlignment")
		{
			if (value == "Right")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Right);
			}
			else if (value == "Left")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Left);
			}
			else if (value == "Center")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Center);
			}
			else if (value == "Stretch")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Stretch);
			}
		}
		else if (name == "VerticalAlignment")
		{
			if (value == "Top")
			{
				result->setVerticalAlignment(VerticalAlignment::Top);
			}
			else if (value == "Bottom")
			{
				result->setVerticalAlignment(VerticalAlignment::Bottom);
			}
			else if (value == "Center")
			{
				result->setVerticalAlignment(VerticalAlignment::Center);
			}
			else if (value == "Stretch")
			{
				result->setVerticalAlignment(VerticalAlignment::Stretch);
			}
		}
	}

	size_t childCount = element->getChildElementCount();
	auto children = element->getChildNodes();
	for (int i = 0; i < childCount; i++)
	{
		auto child = children->item(i);
		result->Children.push_back(LoadXaml((DOMElement*)child));
	}
	return result;
}

OpenXaml::TextBlock* DomToTextBlock(xercesc::DOMElement* element)
{
	TextBlock* result = new TextBlock();

	DOMNamedNodeMap* attributes = element->getAttributes();
	for (int i = 0; i < attributes->getLength(); i++)
	{
		DOMNode* item = attributes->item(i);
		const XMLCh* nameXML = item->getNodeName();
		const XMLCh* valXML = item->getNodeValue();
		string name = XMLString::transcode(nameXML);
		string value = XMLString::transcode(valXML);

		if (name == "Height")
		{
			result->setHeight(stoi(value));
		}
		else if (name == "Width")
		{
			result->setWidth(stoi(value));
		}
		else if (name == "HorizontalAlignment")
		{
			if (value == "Right")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Right);
			}
			else if (value == "Left")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Left);
			}
			else if (value == "Center")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Center);
			}
			else if (value == "Stretch")
			{
				result->setHorizontalAlignment(HorizontalAlignment::Stretch);
			}
		}
		else if (name == "VerticalAlignment")
		{
			if (value == "Top")
			{
				result->setVerticalAlignment(VerticalAlignment::Top);
			}
			else if (value == "Bottom")
			{
				result->setVerticalAlignment(VerticalAlignment::Bottom);
			}
			else if (value == "Center")
			{
				result->setVerticalAlignment(VerticalAlignment::Center);
			}
			else if (value == "Stretch")
			{
				result->setVerticalAlignment(VerticalAlignment::Stretch);
			}
		}
		else if (name == "TextWrapping")
		{
			if (value == "None")
			{
				result->setTextWrapping(TextWrapping::None);
			}
			else if (value == "Wrap")
			{
				result->setTextWrapping(TextWrapping::Wrap);
			}
		}
		else if (name == "FontFamily")
		{
			result->setFontFamily(value);
		}
		else if (name == "FontSize")
		{
			result->setFontSize(stof(value));
		}
		else if (name == "Fill")
		{
			std::istringstream iss(value.substr(1, value.size()));
			unsigned int fill;
			iss >> std::hex >> fill;
			result->setFill(fill);
		}
		else if (name == "TextAlignment")
		{
			if (value == "Left")
			{
				result->setTextAlignment(TextAlignment::Left);
			}
			else if (value == "Right")
			{
				result->setTextAlignment(TextAlignment::Right);
			}
			else if (value == "Center")
			{
				result->setTextAlignment(TextAlignment::Center);
			}
		}
	}

	auto text = element->getTextContent();
	result->setText(XMLString::transcode(text));

	size_t childCount = element->getChildElementCount();
	auto children = element->getChildNodes();
	for (int i = 0; i < childCount; i++)
	{
		auto child = children->item(i);
		result->Children.push_back(LoadXaml((DOMElement*)child));
	}

	return result;
}