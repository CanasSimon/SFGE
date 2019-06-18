#ifndef SFGE_EXT_EXPLOSION
#define SFGE_EXT_EXPLOSION

#include <engine/system.h>
#include <graphics/graphics2d.h>
#include "p2body.h"
#include "input/input.h"


namespace sfge
{
	struct Transform2d;
	class Transform2dManager;
	class Body2dManager;
}

namespace sfge::ext
{
	class Explosion : public System
	{
	public:
		Explosion(Engine& engine);

		void OnEngineInit() override;

		void OnUpdate(float dt) override;

		void OnFixedUpdate() override;

		void OnDraw() override;

	private:
		Transform2dManager* m_Transform2DManager;
		Body2dManager* m_Body2DManager;

		InputManager* m_InputManager;
		MouseManager* m_MouseManager;

		std::vector<Transform2d*> m_Transforms;
		std::vector<Entity> m_Entities;
		std::vector<p2Body*> m_Bodies;

	};
}

#endif
