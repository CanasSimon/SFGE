#ifndef SFGE_EXT_BODY_TEST_H
#define SFGE_EXT_BODY_TEST_H

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
		Graphics2dManager* m_Graphics2DManager;
		ShapeManager* m_Shape2DManager;

		sf::Vector2f screenSize;
		std::vector<Transform2d*> m_Transforms;
		std::vector<Entity> m_Entities;
		std::vector<p2Body*> m_Bodies;
		std::vector<Shape*> m_Shapes;

	};
}

#endif
