#ifndef SFGE_EXT_DRAW_AABB_H
#define SFGE_EXT_DRAW_AABB_H

#include <engine/system.h>
#include <graphics/graphics2d.h>
#include "p2body.h"


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
	class BodyTest : public System
	{
	public:
		BodyTest(Engine& engine);

		void OnEngineInit() override;

		void OnUpdate(float dt) override;

		void OnFixedUpdate() override;

		void OnDraw() override;

	private:
		Transform2dManager* m_Transform2DManager;
		Body2dManager* m_Body2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;
		Graphics2dManager* m_Graphics2DManager;

		const size_t entitiesNmb = 10;

		sf::Vector2f screenSize;
		std::vector<p2Body*> bodies;
		std::vector<Entity> entities;
	};
}

#endif
