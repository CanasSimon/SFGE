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

		const auto config = m_Engine.GetConfig();
		screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);

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

		for (auto& body : bodies)
		{
			const auto velocity = body->GetLinearVelocity();

			for (auto& collider : body->GetColliders())
			{
				if (collider.GetAABB().topRight.x > pixel2meter(screenSize).x && velocity.x > 0 || collider.GetAABB().topLeft.x < 0 && velocity.x < 0)
				{
					body->SetLinearVelocity(p2Vec2(-velocity.x, velocity.y));
					break;
				}
				if (collider.GetAABB().topRight.y > pixel2meter(screenSize).y && velocity.y > 0 || collider.GetAABB().bottomRight.y < 0 && velocity.y < 0)
				{
					body->SetLinearVelocity(p2Vec2(velocity.x, -velocity.y));
					break;
				}
			}
		}
	}

	void DrawAABB::OnFixedUpdate()
	{
		rmt_ScopedCPUSample(DrawAABBFixedUpdate, 0);

	}

	void DrawAABB::OnDraw()
	{
		rmt_ScopedCPUSample(DrawAABBDraw, 0);
		for (auto body : bodies)
		{
			DrawAABBShape(body->GetAABB(), sf::Color::Red);
			for (auto& collider : body->GetColliders())
			{
				//std::cout << collider.GetAABB().top << ":" << collider.GetAABB().bottom << "\n";
				DrawAABBShape(collider.GetAABB(), sf::Color::Green);
			}
		}

		m_Graphics2DManager->DrawLine(Vec2f(0, 0), screenSize, sf::Color::Green);
	}

	void DrawAABB::DrawAABBShape(p2AABB aabb, sf::Color color) const
	{
		//std::cout << aabb.topRight().x << ":" << aabb.topRight().y << "\n";
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.topRight), meter2pixel(aabb.topLeft), color);
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.topRight), meter2pixel(aabb.bottomRight), color);
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.bottomLeft), meter2pixel(aabb.topLeft), color);
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.bottomLeft), meter2pixel(aabb.bottomRight), color);
	}

}
