#include <extensions/debug.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <graphics/graphics2d.h>
#include <physics/body2d.h>
#include <physics/physics2d.h>
#include <iostream>


namespace sfge::ext
{

	Debug::Debug(Engine& engine) :
		System(engine), m_Body2DManager(), m_Graphics2DManager()
	{
	}

	void Debug::OnEngineInit()
	{
		m_PhysicsManager = m_Engine.GetPhysicsManager();
		m_World = m_PhysicsManager->GetWorld();
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_Body2DManager = m_Engine.GetPhysicsManager()->GetBodyManager();
		m_Graphics2DManager = m_Engine.GetGraphics2dManager();
		m_InputManager = m_Engine.GetInputManager();
		m_KeyboardManager = &m_InputManager->GetKeyboardManager();

		m_QuadTree = m_World->GetQuadTree();

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

		p2AABB quadTreeBounds;
		quadTreeBounds.bottomLeft = p2Vec2(0, 0);
		quadTreeBounds.topLeft = p2Vec2(0, pixel2meter(screenSize).y);
		quadTreeBounds.topRight = pixel2meter(screenSize);
		quadTreeBounds.bottomRight = p2Vec2(pixel2meter(screenSize).x, 0);
		m_QuadTree->SetBounds(quadTreeBounds);
	}

	void Debug::OnUpdate(float dt)
	{
		(void)dt;

		if (m_KeyboardManager->IsKeyDown(sf::Keyboard::Key::A)) drawAabb = !drawAabb;
		if (m_KeyboardManager->IsKeyDown(sf::Keyboard::Key::S)) drawSat = !drawSat;
		if (m_KeyboardManager->IsKeyDown(sf::Keyboard::Key::Q)) drawQuadTree = !drawQuadTree;

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

	void Debug::OnFixedUpdate()
	{
		rmt_ScopedCPUSample(DrawAABBFixedUpdate, 0);

		transforms[0]->EulerAngle += 1;
	}

	void Debug::OnDraw()
	{
		rmt_ScopedCPUSample(DrawAABBDraw, 0);
		axes.clear();

		if(drawSat)
		{
			for (auto& body : bodies)
			{
				for (auto& collider : body->GetColliders())
				{
					for (auto& edge : collider.GetAABB().edges)
					{
						axes.push_back(edge.GetNormal());
					}
				}
			}
		}

		if (drawSat || drawAabb || drawQuadTree)
		{
			if (drawQuadTree)
			{
				DrawAABBShape(m_QuadTree->GetBounds(), sf::Color::Red);
				for (auto& child : m_QuadTree->GetChildren())
				{
					if(child == nullptr) break;
					DrawAABBShape(child->GetBounds(), sf::Color::Red);
				}
			}

			for (auto& body : bodies)
			{
				if (drawAabb) DrawAABBShape(body->GetAABB(), sf::Color::Red);
				for (auto& collider : body->GetColliders())
				{
					if (drawAabb) DrawAABBShape(collider.GetAABB(), sf::Color::Green);
					if (drawSat) DrawSATShape(collider);
				}
			}
		}
	}

	void Debug::DrawAABBShape(p2AABB aabb, sf::Color color) const
	{
		//std::cout << m_Aabb.topRight().x << ":" << m_Aabb.topRight().y << "\n";
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.topRight), meter2pixel(aabb.topLeft), color);
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.topRight), meter2pixel(aabb.bottomRight), color);
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.bottomLeft), meter2pixel(aabb.topLeft), color);
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.bottomLeft), meter2pixel(aabb.bottomRight), color);
	}

	void Debug::DrawSATShape(p2Collider collider) const
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
				m_Graphics2DManager->DrawVector(meter2pixel(axis), meter2pixel(axis * 5));
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
		const auto temp2 = p2Vec2(4.9, 4.9);
		const auto vector1 = p2Vec2().GetVectorFrom(temp1, temp1 + offset);
		const auto vector2 = p2Vec2().GetVectorFrom(temp2, temp2 + offset);
		m_Graphics2DManager->DrawLine(meter2pixel(temp1), meter2pixel(temp1 + offset), sf::Color::Magenta);
		m_Graphics2DManager->DrawLine(meter2pixel(temp2), meter2pixel(temp2 + offset), sf::Color::Green);

		if (p2Vec2().DoOverlap(temp1, temp1 + offset, temp2, temp2 + offset))
			m_Graphics2DManager->DrawLine(meter2pixel(temp1 + p2Vec2(1, 1)),
				meter2pixel(temp2 + p2Vec2(1, 1)), sf::Color::Red);
	}
}
