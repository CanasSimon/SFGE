#include <extensions/draw_sat.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <graphics/graphics2d.h>
#include <physics/body2d.h>
#include <physics/physics2d.h>
#include <iostream>


namespace sfge::ext
{

	DrawSAT::DrawSAT(Engine& engine) :
		System(engine), m_Body2DManager(), m_Graphics2DManager()
	{
	}

	void DrawSAT::OnEngineInit()
	{
		m_Body2DManager = m_Engine.GetPhysicsManager()->GetBodyManager();
		m_Graphics2DManager = m_Engine.GetGraphics2dManager();

		auto* entityManager = m_Engine.GetEntityManager();

		const auto config = m_Engine.GetConfig();
		screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);

		entities = entityManager->GetEntitiesWithType(ComponentType::BODY2D);
		for (auto& entity : entities)
		{
			const auto body = m_Body2DManager->GetComponentPtr(entity);
			bodies.push_back(body->GetBody());
		}
	}

	void DrawSAT::OnUpdate(float dt)
	{
		(void)dt;
	}

	void DrawSAT::OnFixedUpdate()
	{
		rmt_ScopedCPUSample(DrawSATFixedUpdate, 0);

	}

	void DrawSAT::OnDraw()
	{
		rmt_ScopedCPUSample(DrawSATDraw, 0);
		for (auto body : bodies)
		{
			for (auto& collider : body->GetColliders())
			{
				DrawSATShape(collider);
			}
		}

		m_Graphics2DManager->DrawLine(Vec2f(0, 0), screenSize, sf::Color::Green);
	}

	void DrawSAT::DrawSATShape(p2Collider collider) const
	{
		switch (collider.GetType())
		{
		case p2ColliderType::RECT:
			{
			const auto aabb = collider.GetAABB();
				for (int i = 0; i < aabb.m_Vertices.size(); ++i)
				{
					p2Vec2 edge;
					if(i + 1 < aabb.m_Vertices.size())
					{
						edge = p2Vec2().GetVectorFrom(aabb.m_Vertices[i], aabb.m_Vertices[i + 1]);
						m_Graphics2DManager->DrawLine(meter2pixel(aabb.m_Vertices[i]),
						                              meter2pixel(aabb.m_Vertices[i + 1]), sf::Color::Blue);
					}
					else
					{
						edge = p2Vec2().GetVectorFrom(aabb.m_Vertices[i], aabb.m_Vertices[0]);
						m_Graphics2DManager->DrawLine(meter2pixel(aabb.m_Vertices[i]),
						                              meter2pixel(aabb.m_Vertices[0]), sf::Color::Blue);
					}

				}
			}
			break;
		default:
			break;
		}
	}
}
