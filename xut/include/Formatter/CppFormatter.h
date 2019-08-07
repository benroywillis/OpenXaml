#pragma once
#include <string>
#include <xercesc/dom/DOMAttr.hpp>
std::string GetHeight(xercesc::DOMAttr*, bool root = false);
std::string GetFill(xercesc::DOMAttr*, bool root = false);
std::string GetWidth(xercesc::DOMAttr*, bool root = false);
std::string GetHorizontalAlignment(xercesc::DOMAttr*, bool root = false);
std::string GetVerticalAlignment(xercesc::DOMAttr*, bool root = false);
std::string GetTextWrapping(xercesc::DOMAttr*);
std::string GetTextAlignment(xercesc::DOMAttr*);
std::string GetFontFamily(xercesc::DOMAttr*);
std::string GetFontSize(xercesc::DOMAttr*);
std::string GetText(std::string input);
std::string GetText(xercesc::DOMAttr*);
std::string GetContent(xercesc::DOMAttr* input);
std::string GetContent(std::string input);
std::string GetClickSigniture(xercesc::DOMAttr* input);
std::string GetClickCall(xercesc::DOMAttr* input);
std::string FormatString(std::string input);
std::string GetGridRow(xercesc::DOMAttr* input);
std::string GetGridColumn(xercesc::DOMAttr* input);