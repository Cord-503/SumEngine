#pragma once

namespace SumEngine::Physics
{
	class CollisionShape final
	{
	public:
		CollisionShape() = default;
		~CollisionShape();

		void Terminate();

	private:
		friend class RigidBody;
		btCollisionShape* mCollisionShape = nullptr;
	};
}