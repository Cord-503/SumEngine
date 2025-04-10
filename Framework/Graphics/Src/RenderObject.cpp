#include "Precompiled.h"
#include "RenderObject.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
}