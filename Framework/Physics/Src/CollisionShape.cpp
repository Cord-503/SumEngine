#include "Precompiled.h"
#include "CollisionShape.h"

using namespace SumEngine;
using namespace SumEngine::Physics;

CollisionShape::~CollisionShape()
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: terminate must be called");
}

void CollisionShape::Terminate()
{
	SafeDelete(mCollisionShape);
}
