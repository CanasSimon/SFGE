#include <extensions/draw_aabb.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <graphics/graphics2d.h>
#include <physics/body2d.h>
#include <physics/physics2d.h>
#include <iostream>


namespace sfge::ext
{

	DrawAABB::DrawAABB(Engine& engine) :
		System(engine), m_Body2DManager(), m_Graphics2DManager()
	{
	}

	void DrawAABB::OnEngineInit()
	{
		m_Body2DManager = m_Engine.GetPhysicsManager()->GetBodyManager();
		m_Graphics2DManager = m_Engine.GetGraphics2dManager();

		auto* entityManager = m_Engine.GetEntityManager();

		entities = entityManager->GetEntitiesWithType(ComponentType::BODY2D);
		for (auto& entity : entities)
		{
			const auto body = m_Body2DManager->GetComponentPtr(entity);
			bodies.push_back(body->GetBody());
		}
	}

	void DrawAABB::OnUpdate(float dt)
	{
		(void)dt;
	}

	void DrawAABB::OnFixedUpdate()
	{
		rmt_ScopedCPUSample(DrawAABBFixedUpdate, 0);

	}

	void DrawAABB::OnDraw()
	{
		rmt_ScopedCPUSample(DrawAABBDraw, 0);
		for (auto& body : bodies)
		{
			DrawAABBShape(body->GetAABB());
		}
	}

	void DrawAABB::DrawAABBShape(p2AABB aabb) const
	{
		std::cout << aabb.GetTopRight().x << ":" << aabb.GetTopRight().y << "\n";
		m_Graphics2DManager->DrawLine(meter2pixel(p2Vec2(aabb.GetTopRight())), meter2pixel(p2Vec2(aabb.GetTopLeft())), sf::Color::Red);
		m_Graphics2DManager->DrawLine(meter2pixel(p2Vec2(aabb.GetTopRight())), meter2pixel(p2Vec2(aabb.GetBottomRight())), sf::Color::Red);
		m_Graphics2DManager->DrawLine(meter2pixel(p2Vec2(aabb.GetBottomLeft())), meter2pixel(p2Vec2(aabb.GetTopLeft())), sf::Color::Red);
		m_Graphics2DManager->DrawLine(meter2pixel(p2Vec2(aabb.GetBottomLeft())), meter2pixel(p2Vec2(aabb.GetBottomRight())), sf::Color::Red);
	}

}
