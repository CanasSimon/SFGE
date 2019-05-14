#ifndef SFGE_EXT_DRAW_SAT_H
#define SFGE_EXT_DRAW_SAT_H

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
	class DrawSAT : public System
	{
	public:
		DrawSAT(Engine& engine);

		void OnEngineInit() override;

		void OnUpdate(float dt) override;

		void OnFixedUpdate() override;

		void OnDraw() override;

	private:
		Body2dManager* m_Body2DManager;
		Graphics2dManager* m_Graphics2DManager;

		void DrawSATShape(p2Collider collider) const;

		sf::Vector2f screenSize;
		std::vector<p2Body*> bodies;
		std::vector<Entity> entities;
	};
}

#endif
