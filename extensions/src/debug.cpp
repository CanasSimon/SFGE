#include <extensions/debug.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <graphics/graphics2d.h>
#include <physics/body2d.h>
#include <physics/physics2d.h>


namespace sfge::ext
{

	Debug::Debug(Engine& engine) :
		System(engine), m_PhysicsManager(), m_World(), m_Transform2DManager(), m_Body2DManager(),
		m_Graphics2DManager(), m_Shape2DManager(), m_InputManager(), m_KeyboardManager(), m_QuadTree()
	{
	}

	void Debug::OnEngineInit()
	{
		m_PhysicsManager = m_Engine.GetPhysicsManager();
		m_World = m_PhysicsManager->GetWorld();
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_Body2DManager = m_Engine.GetPhysicsManager()->GetBodyManager();
		m_Graphics2DManager = m_Engine.GetGraphics2dManager();
		m_Shape2DManager = m_Graphics2DManager->GetShapeManager();
		m_InputManager = m_Engine.GetInputManager();
		m_KeyboardManager = &m_InputManager->GetKeyboardManager();

		m_QuadTree = m_World->GetQuadTree();

		auto* entityManager = m_Engine.GetEntityManager();

		const auto config = m_Engine.GetConfig();
		m_ScreenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);

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

		p2Aabb quadTreeBounds;
		quadTreeBounds.bottomLeft = p2Vec2(0, 0);
		quadTreeBounds.topRight = pixel2meter(m_ScreenSize);
		m_QuadTree->SetBounds(quadTreeBounds);
	}

	void Debug::OnUpdate(float dt)
	{
		(void)dt;

		if (m_KeyboardManager->IsKeyDown(sf::Keyboard::Key::A)) m_DrawAabb = !m_DrawAabb;
		if (m_KeyboardManager->IsKeyDown(sf::Keyboard::Key::Q)) m_DrawQuadTree = !m_DrawQuadTree;
		if (m_KeyboardManager->IsKeyDown(sf::Keyboard::Key::C)) m_QuadTree->Clear();

		if (m_KeyboardManager->IsKeyHeld(sf::Keyboard::Key::Up)) m_Bodies[0]->ApplyForceToCenter(p2Vec2(0, -1));
		if (m_KeyboardManager->IsKeyHeld(sf::Keyboard::Key::Down)) m_Bodies[0]->ApplyForceToCenter(p2Vec2(0, 1));
		if (m_KeyboardManager->IsKeyHeld(sf::Keyboard::Key::Right)) m_Bodies[0]->ApplyForceToCenter(p2Vec2(1, 0));
		if (m_KeyboardManager->IsKeyHeld(sf::Keyboard::Key::Left)) m_Bodies[0]->ApplyForceToCenter(p2Vec2(-1, 0));

		m_Bodies[0]->SetLinearVelocity(m_Bodies[0]->GetLinearVelocity() / 1.2);

		for (auto& body : m_Bodies)
		{
			const auto velocity = body->GetLinearVelocity();

			for (auto& collider : body->GetColliders())
			{
				for (auto& vertex : collider.GetAabb().vertices)
				{
					if(vertex.x > pixel2meter(m_ScreenSize).x && velocity.x > 0 || vertex.x < 0 && velocity.x < 0)
					{
						body->SetLinearVelocity(p2Vec2(-velocity.x, velocity.y));
						break;
					}

					if (vertex.y > pixel2meter(m_ScreenSize).y && velocity.y > 0 || vertex.y < 0 && velocity.y < 0)
					{
						body->SetLinearVelocity(p2Vec2(velocity.x, -velocity.y));
						break;
					}
				}
			}
		}
	}

	void Debug::OnFixedUpdate()
	{
		rmt_ScopedCPUSample(DrawAABBFixedUpdate, 0);

		m_Transforms[0]->EulerAngle += 1;
	}

	void Debug::OnDraw()
	{
		rmt_ScopedCPUSample(DrawAABBDraw, 0);

		if(CheckSat(&m_Bodies[0]->GetColliders()[0], &m_Bodies[1]->GetColliders()[0]))
		{
			m_Shapes[0]->SetFillColor(sf::Color::Green);
			m_Shapes[1]->SetFillColor(sf::Color::Green);
		}
		else
		{
			m_Shapes[0]->SetFillColor(sf::Color::Red);
			m_Shapes[1]->SetFillColor(sf::Color::Red);
		}

		if (m_DrawQuadTree)
		{
			DrawQuadTree(m_QuadTree, sf::Color::Red);

			for (auto& bodies : m_QuadTree->Retrieve(m_Bodies[0]))
			{
				m_Graphics2DManager->DrawLine(meter2pixel(bodies->GetPosition()), Vec2f(0, 0), sf::Color::White);
			}
		}

		if (m_DrawAabb)
		{
			for (auto& body : m_Bodies)
			{
				DrawAabb(body->GetAabb(), sf::Color::Red);
				for (auto& collider : body->GetColliders())
				{
					DrawAabb(collider.GetAabb(), sf::Color::Green);
				}
			}
		}
	}

	void Debug::DrawAabb(const p2Aabb aabb, const sf::Color color) const
	{
		//std::cout << m_Aabb.topRight().x << ":" << m_Aabb.topRight().y << "\n";
		for (size_t i = 0; i < aabb.vertices.size(); ++i)
		{
			m_Graphics2DManager->DrawLine(meter2pixel(aabb.vertices[i]), meter2pixel(aabb.vertices[i + 1 < aabb.vertices.size() ? i + 1 : 0]), color);
		}
	}

	bool Debug::CheckSat(const p2Collider* bodyA, const p2Collider* bodyB)
	{
		switch (bodyA->GetType())
		{
		case p2ColliderType::CIRCLE:
			switch (bodyB->GetType())
			{
			case p2ColliderType::CIRCLE:
				return CheckCircleSat(bodyA, bodyB);
			case p2ColliderType::RECT:
				return CheckCircleBoxSat(bodyA, bodyB);
			default:
				return false;
			}
		case p2ColliderType::RECT:
			switch (bodyB->GetType())
			{
			case p2ColliderType::CIRCLE:
				return CheckCircleBoxSat(bodyB, bodyA);
			case p2ColliderType::RECT:
				return CheckBoxSat(bodyB, bodyA);
			default:
				return false;
			}
		default:
			return false;
		}
	}

	bool Debug::CheckBoxSat(const p2Collider* bodyA, const p2Collider* bodyB)
	{
		//Code edited from https://gist.github.com/nyorain/dc5af42c6e83f7ac6d831a2cfd5fbece
		std::vector<p2Vec2> axes;
		const auto aabbA = bodyA->GetAabb();
		const auto aabbB = bodyB->GetAabb();
		axes.reserve(aabbA.edges.size() / 2 + aabbB.edges.size() / 2);

		for (auto i = 0u; i < aabbA.edges.size() / 2; ++i)
		{
			axes.push_back(aabbA.edges[i].GetNormal());
		}

		for (auto i = 0u; i < aabbB.edges.size() / 2; ++i)
		{
			axes.push_back(aabbB.edges[i].GetNormal());
		}

		for (auto& axis : axes)
		{
			auto aMaxProj = -std::numeric_limits<float>::infinity();
			auto aMinProj = std::numeric_limits<float>::infinity();
			auto bMaxProj = -std::numeric_limits<float>::infinity();
			auto bMinProj = std::numeric_limits<float>::infinity();

			for (auto& vertex : aabbA.vertices)
			{
				const auto proj = p2Vec2::Dot(axis, vertex);
				if (proj < aMinProj) aMinProj = proj;
				if (proj > aMaxProj) aMaxProj = proj;
			}

			for (auto& vertex : aabbB.vertices)
			{
				const auto proj = p2Vec2::Dot(axis, vertex);
				if (proj < bMinProj) bMinProj = proj;
				if (proj > bMaxProj) bMaxProj = proj;
			}

			if (aMaxProj < bMinProj || bMaxProj < aMinProj) return false;
		}

		return true;
	}

	bool Debug::CheckCircleSat(const p2Collider* bodyA, const p2Collider* bodyB)
	{
		const auto distance = bodyA->position.GetDistance(bodyB->position);

		return distance <= bodyA->GetHalfExtend().x + bodyB->GetHalfExtend().x;
	}

	bool Debug::CheckCircleBoxSat(const p2Collider* bodyA, const p2Collider* bodyB)
	{
		const auto aabbA = bodyA->GetAabb();
		const auto aabbB = bodyB->GetAabb();

		for (auto i = 0u; i < aabbB.vertices.size() / 2; ++i)
		{
			if (aabbA.GetCenter().LineSide(aabbB.vertices[i], aabbB.vertices[i + 1]) == 1 &&
				aabbA.GetCenter().LineSide(aabbB.vertices[i + 2], aabbB.vertices[(i + 3) % aabbB.vertices.size()]) == 1)
			{
				goto InZone;
			}
		}
		goto NotInZone;

	InZone:
		std::cout << "In zone" << "\n";
		for (auto i = 0u; i < aabbB.edges.size() / 2; ++i)
		{
			const auto aPoint1 = aabbA.GetCenter() - bodyA->GetHalfExtend();
			const auto aPoint2 = aabbA.GetCenter() + bodyA->GetHalfExtend();
			const auto aMaxProj = p2Vec2::Dot(aabbA.edges[i], aPoint1);
			const auto aMinProj = p2Vec2::Dot(aabbA.edges[i], aPoint2);
			auto bMaxProj = -std::numeric_limits<float>::infinity();
			auto bMinProj = std::numeric_limits<float>::infinity();

			for (auto& vertex : aabbB.vertices)
			{
				const auto proj = p2Vec2::Dot(aabbA.edges[i], vertex);
				if (proj < bMinProj) bMinProj = proj;
				if (proj > bMaxProj) bMaxProj = proj;
			}

			if (aMaxProj < bMinProj || bMaxProj < aMinProj)
			{
				return false;
			}
		}

		return true;

	NotInZone:
		std::cout << "Not in zone" << "\n";
		auto dist = std::numeric_limits<float>::infinity();
		for (auto& vertex : aabbB.vertices)
		{
			if (vertex.GetDistance(aabbA.GetCenter()) < dist)
			{
				dist = vertex.GetDistance(aabbA.GetCenter());
			}
		}

		return dist < bodyA->GetHalfExtend().x;
	}

	void Debug::DrawQuadTree(p2QuadTree * quadTree, const sf::Color color) const
	{
		const auto aabb = quadTree->GetBounds();
		const auto extend = aabb.GetExtends();
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.topRight), meter2pixel(p2Vec2(aabb.topRight.x - extend.x, aabb.topRight.y)), color);
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.topRight), meter2pixel(p2Vec2(aabb.topRight.x, aabb.topRight.y - extend.y)), color);
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.bottomLeft), meter2pixel(p2Vec2(aabb.topRight.x - extend.x, aabb.topRight.y)), color);
		m_Graphics2DManager->DrawLine(meter2pixel(aabb.bottomLeft), meter2pixel(p2Vec2(aabb.topRight.x, aabb.topRight.y - extend.y)), color);
		if(!quadTree->GetChildren().empty())
		{
			for (auto& child : quadTree->GetChildren())
			{
				DrawQuadTree(child, color);
			}
		}

		for (auto& obj : quadTree->GetObjects())
		{
			m_Graphics2DManager->DrawLine(meter2pixel(obj->GetPosition()), meter2pixel(aabb.bottomLeft), sf::Color::Green);
		}
	}
}
