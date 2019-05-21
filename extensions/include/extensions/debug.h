#ifndef SFGE_EXT_DEBUG_H
#define SFGE_EXT_DEBUG_H

#include <engine/system.h>
#include <graphics/graphics2d.h>
#include "p2body.h"
#include "input/input.h"
#include <p2world.h>


namespace sfge
{
	struct Transform2d;
	class Transform2dManager;
	class Body2dManager;
	class TextureManager;
	class SpriteManager;
}

namespace sfge::ext
{
	class Debug : public System
	{
	public:
		Debug(Engine& engine);

		void OnEngineInit() override;

		void OnUpdate(float dt) override;

		void OnFixedUpdate() override;

		void OnDraw() override;

	private:
		Physics2dManager* m_PhysicsManager;
		p2World* m_World;

		Transform2dManager* m_Transform2DManager;
		Body2dManager* m_Body2DManager;
		Graphics2dManager* m_Graphics2DManager;

		InputManager* m_InputManager;
		KeyboardManager* m_KeyboardManager;

		void DrawAABBShape(p2AABB aabb, sf::Color color) const;
		void DrawSATShape(p2Collider collider) const;

		std::vector<p2Vec2> axes;

		sf::Vector2f screenSize;
		std::vector<Transform2d*> transforms;
		std::vector<p2Body*> bodies;
		std::vector<Entity> entities;

		bool drawAabb = false;
		bool drawSat = false;
		bool drawQuadTree = false;
	};
}

#endif
