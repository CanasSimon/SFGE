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
		System(engine), m_Transform2DManager(), m_Body2DManager(), m_Graphics2DManager()
	{
	}

	void BodyTest::OnEngineInit()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_Body2DManager = m_Engine.GetPhysicsManager()->GetBodyManager();
		m_Graphics2DManager = m_Engine.GetGraphics2dManager();
		m_Shape2DManager = m_Graphics2DManager->GetShapeManager();

		const auto config = m_Engine.GetConfig();
		screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);
		auto* entityManager = m_Engine.GetEntityManager();

		m_Entities = entityManager->GetEntitiesWithType(ComponentType::BODY2D);
		for (auto& entity : m_Entities)
		{
			const auto transform = m_Transform2DManager->GetComponentPtr(entity);
			const auto body = m_Body2DManager->GetComponentPtr(entity);
			const auto shape = m_Shape2DManager->GetComponentPtr(entity);
			m_Transforms.push_back(transform);
			m_Bodies.push_back(body->GetBody());
			m_Shapes.push_back(shape);
		}
	}

	void BodyTest::OnUpdate(float dt)
	{
		(void)dt;
		for (auto& transform : m_Transforms)
		{
			transform->EulerAngle += 1;
		}
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
