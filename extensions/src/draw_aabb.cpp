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
				std::cout << collider.GetAABB().bottom << " : " << pixel2meter(screenSize).y << "\n" << "------------------------" << "\n";
				if (collider.GetAABB().right > pixel2meter(screenSize).x && velocity.x > 0 || collider.GetAABB().left < 0 && velocity.x < 0)
				{
					body->SetLinearVelocity(p2Vec2(-velocity.x, velocity.y));
					break;
				}
				if (collider.GetAABB().top > pixel2meter(screenSize).y && velocity.y > 0 || collider.GetAABB().bottom < 0 && velocity.y < 0)
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
			DrawAABBShape(body->GetAABB());
			for (auto& collider : body->GetColliders())
			{
				//std::cout << collider.GetAABB().top << ":" << collider.GetAABB().bottom << "\n";
				DrawAABBShape(collider.GetAABB());
			}
		}

		m_Graphics2DManager->DrawLine(Vec2f(0, 0), screenSize, sf::Color::Green);
	}

	void DrawAABB::DrawAABBShape(p2AABB aabb) const
	{
		//std::cout << aabb.topRight().x << ":" << aabb.topRight().y << "\n";
		m_Graphics2DManager->DrawLine(meter2pixel(p2Vec2(aabb.right, aabb.top)), meter2pixel(p2Vec2(aabb.left, aabb.top)), sf::Color::Red);
		m_Graphics2DManager->DrawLine(meter2pixel(p2Vec2(aabb.right, aabb.top)), meter2pixel(p2Vec2(aabb.right, aabb.bottom)), sf::Color::Red);
		m_Graphics2DManager->DrawLine(meter2pixel(p2Vec2(aabb.left, aabb.bottom)), meter2pixel(p2Vec2(aabb.left, aabb.top)), sf::Color::Red);
		m_Graphics2DManager->DrawLine(meter2pixel(p2Vec2(aabb.left, aabb.bottom)), meter2pixel(p2Vec2(aabb.right, aabb.bottom)), sf::Color::Red);
	}

}
