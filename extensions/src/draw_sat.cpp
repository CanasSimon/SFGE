#include <extensions/draw_sat.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <graphics/graphics2d.h>
#include <physics/body2d.h>
#include <physics/physics2d.h>

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
		axes.clear();
		for (auto body : bodies)
		{
			for (auto& collider : body->GetColliders())
			{
				for (auto& edge : collider.GetAABB().edges)
				{
					axes.push_back(edge.GetNormal());
				}
			}
		}

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
			m_Graphics2DManager->DrawLine(meter2pixel(collider.position), meter2pixel(p2Vec2(aabb.topRight.x, 0)),
			                              sf::Color::Blue);
			break;
		case p2ColliderType::RECT:
			{
				for (auto& axis : axes)
				{
					for (auto& vertex : aabb.vertices)
					{
						m_Graphics2DManager->DrawLine(meter2pixel(aabb.GetCenter()), meter2pixel(vertex.GetProjectionOn(axis)),
							sf::Color::Blue);
						m_Graphics2DManager->DrawVector(meter2pixel(aabb.edges[0]), meter2pixel(vertex));
					}
				}
				
			}
			break;
		default:
			break;
		}

		const auto offset = p2Vec2(1, 1);
		const auto temp1 = p2Vec2(4, 4);
		const auto temp2 = p2Vec2(5.1, 5.1);
		const auto vector1 = p2Vec2().GetVectorFrom(temp1, temp1 + offset);
		const auto vector2 = p2Vec2().GetVectorFrom(temp2, temp2 + offset);
		m_Graphics2DManager->DrawLine(meter2pixel(temp1), meter2pixel(temp1 + offset), sf::Color::Magenta);
		m_Graphics2DManager->DrawLine(meter2pixel(temp2), meter2pixel(temp2 + offset), sf::Color::Green);

		if (p2Vec2().DoOverlap(temp1, temp1 + offset, temp2, temp2 + offset))
			m_Graphics2DManager->DrawLine(meter2pixel(temp1 + p2Vec2(1, 1)), 
				meter2pixel(temp2 + p2Vec2(1, 1)), sf::Color::Red);
	}
}
