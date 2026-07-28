#pragma once

namespace sm
{
	class GAS_World;

	class GameplaySystem
	{
	public:
		virtual void Update([[maybe_unused]] float dt) {};

	public:
		GAS_World* _world = nullptr;
	};
}