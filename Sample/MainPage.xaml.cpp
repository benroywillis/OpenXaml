#include "MainPage.xaml.h"

//#include "XamlEvents/EventHandler.h"
#include <iostream>


//functionMap["MainPage.Test"] = MainPage::Test;



void MainPageInstance::Test(OpenXaml::XamlObject* source)
{
	std::cout << "Pushed\n";
}