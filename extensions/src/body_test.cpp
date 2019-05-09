#include <extensions/body_test.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <graphics/graphics2d.h>
#include <physics/body2d.h>
#include <physics/physics2d.h>
#include <iostream>


namespace sfge::ext
{

	BodyTest::BodyTest(Engine& engine) :
		System(engine), m_Transform2DManager(), m_Body2DManager(), m_TextureManager(), m_SpriteManager(), m_Graphics2DManager()
	{
	}

	void BodyTest::OnEngineInit()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_Body2DManager = m_Engine.GetPhysicsManager()->GetBodyManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
		m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();
		m_Graphics2DManager = m_Engine.GetGraphics2dManager();


		const auto config = m_Engine.GetConfig();
		screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);
		auto* entityManager = m_Engine.GetEntityManager();
	}

	void BodyTest::OnUpdate(float dt)
	{
		(void)dt;
	}


	void BodyTest::OnFixedUpdate()
	{
		rmt_ScopedCPUSample(BodyTestFixedUpdate, 0);

	}

	void BodyTest::OnDraw()
	{
		rmt_ScopedCPUSample(BodyTestDraw, 0);
	}
}
