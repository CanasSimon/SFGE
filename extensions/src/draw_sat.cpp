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
		System(engine), m_Transform2DManager(), m_Body2DManager(), m_Graphics2DManager()
	{
	}

	void DrawSAT::OnEngineInit()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_Body2DManager = m_Engine.GetPhysicsManager()->GetBodyManager();
		m_Graphics2DManager = m_Engine.GetGraphics2dManager();

		auto* entityManager = m_Engine.GetEntityManager();

		const auto config = m_Engine.GetConfig();
		screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);

		entities = entityManager->GetEntitiesWithType(ComponentType::BODY2D);
		for (auto& entity : entities)
		{
			const auto transform = m_Transform2DManager->GetComponentPtr(entity);
			const auto body = m_Body2DManager->GetComponentPtr(entity);
			bodies.push_back(body->GetBody());
			transforms.push_back(transform);
		}
	}

	void DrawSAT::OnUpdate(float dt)
	{
		(void)dt;
	}

	void DrawSAT::OnFixedUpdate()
	{
		rmt_ScopedCPUSample(DrawSATFixedUpdate, 0);

		transforms[0]->EulerAngle += 1;
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
	}

	void DrawSAT::DrawSATShape(p2Collider collider) const
	{
		const auto aabb = collider.GetAABB();
		switch (collider.GetType())
		{
		case p2ColliderType::CIRCLE:
			m_Graphics2DManager->DrawLine(meter2pixel(collider.m_Position), meter2pixel(p2Vec2(aabb.topRight.x, 0)), sf::Color::Blue);
			break;
		case p2ColliderType::RECT:
			{
				p2Vec2 edge;
				p2Vec2 edgePos;
				for (int i = aabb.m_Vertices.size() - 1; i >= 0; --i)
				{
					if (i - 1 >= 0)
					{
						edge = p2Vec2().GetVectorFrom(aabb.m_Vertices[i], aabb.m_Vertices[i - 1]);
						edgePos = (aabb.m_Vertices[i] + aabb.m_Vertices[i - 1]) / 2;
					}
					else
					{
						edge = p2Vec2().GetVectorFrom(aabb.m_Vertices[i], aabb.m_Vertices[aabb.m_Vertices.size() - 1]);
						edgePos = (aabb.m_Vertices[i] + aabb.m_Vertices[aabb.m_Vertices.size() - 1]) / 2;
					}

					m_Graphics2DManager->DrawLine(meter2pixel(edgePos), meter2pixel(edgePos + edge.GetNormal()), sf::Color::Blue);
				}
			}
			break;
		default:
			break;
		}
	}
}
