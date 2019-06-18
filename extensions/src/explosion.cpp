#include <extensions/explosion.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <graphics/graphics2d.h>
#include <physics/body2d.h>
#include <physics/physics2d.h>


namespace sfge::ext
{

	Explosion::Explosion(Engine& engine) :
		System(engine), m_Transform2DManager(), m_Body2DManager()
	{
	}

	void Explosion::OnEngineInit()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_Body2DManager = m_Engine.GetPhysicsManager()->GetBodyManager();

		m_InputManager = m_Engine.GetInputManager();
		m_MouseManager = &m_InputManager->GetMouseManager();

		const auto config = m_Engine.GetConfig();
		auto* entityManager = m_Engine.GetEntityManager();

		m_Entities = entityManager->GetEntitiesWithType(ComponentType::BODY2D);
		for (auto& entity : m_Entities)
		{
			const auto transform = m_Transform2DManager->GetComponentPtr(entity);
			const auto body = m_Body2DManager->GetComponentPtr(entity);
			m_Transforms.push_back(transform);
			m_Bodies.push_back(body->GetBody());
		}
	}

	void Explosion::OnUpdate(float dt)
	{
		(void)dt;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			const auto mousePosition = pixel2meter(m_MouseManager->GetPosition());
			for (auto& body : m_Bodies)
			{
				auto newVelocity = p2Vec2::GetVectorFrom(mousePosition, body->GetPosition());
				body->ApplyForceToCenter(newVelocity.Normalized() / newVelocity.GetMagnitude());
			}
		}
	}


	void Explosion::OnFixedUpdate()
	{
		rmt_ScopedCPUSample(ExplosionFixedUpdate, 0);

	}

	void Explosion::OnDraw()
	{
		rmt_ScopedCPUSample(ExplosionDraw, 0);
	}
}
