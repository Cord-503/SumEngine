#include "ShapeState.h"

using namespace SumEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appname = L"Hello Shapes";

	App& myApp = MainApp();
	myApp.AddState<SquareShapeState>("SquareShapeState");
	myApp.AddState<TriforceShapeState>("TriforceShapeState");
	myApp.AddState<DiamondShapeState>("DiamondShapeState");
	myApp.AddState<TreeShapeState>("TreeShapeState");
	myApp.Run(config);
	return(0);
}


