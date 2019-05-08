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
	class DrawAABB : public System
	{
	public:
		DrawAABB(Engine& engine);

		void OnEngineInit() override;

		void OnUpdate(float dt) override;

		void OnFixedUpdate() override;

		void OnDraw() override;

	private:
		Body2dManager* m_Body2DManager;
		Graphics2dManager* m_Graphics2DManager;


		void DrawAABBShape(p2AABB aabb) const;

		std::vector<p2Body*> bodies;
		std::vector<Entity> entities;
	};
}

#endif
