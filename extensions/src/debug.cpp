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
		m_Graphics2DManager(), m_InputManager(), m_KeyboardManager(), m_QuadTree()
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

		m_QuadTree->Split();
	}

	void Debug::OnUpdate(float dt)
	{
		(void)dt;

		if (m_KeyboardManager->IsKeyDown(sf::Keyboard::Key::A)) m_DrawAabb = !m_DrawAabb;
		if (m_KeyboardManager->IsKeyDown(sf::Keyboard::Key::S)) m_DrawSat = !m_DrawSat;
		if (m_KeyboardManager->IsKeyDown(sf::Keyboard::Key::Q)) m_DrawQuadTree = !m_DrawQuadTree;
		if (m_KeyboardManager->IsKeyDown(sf::Keyboard::Key::C)) m_QuadTree->Clear();

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
		m_Axes.clear();

		if(m_DrawSat)
		{
			for (auto& body : m_Bodies)
			{
				for (auto& collider : body->GetColliders())
				{
					for (auto& edge : collider.GetAabb().edges)
					{
						m_Axes.push_back(edge.GetNormal());
					}
				}
			}
		}

		if (m_DrawSat) DrawSat(m_Bodies[0], m_Bodies[1]);

		if (m_DrawQuadTree)
		{
			DrawQuadTree(m_QuadTree, sf::Color::Red);

			for (auto& bodies : m_QuadTree->Retrieve(m_Bodies[0]))
			{
				m_Graphics2DManager->DrawLine(meter2pixel(bodies->GetPosition()), Vec2f(0, 0), sf::Color::White);
			}
		}

		if (m_DrawSat || m_DrawAabb)
		{
			for (auto& body : m_Bodies)
			{
				if (m_DrawAabb) DrawAabb(body->GetAabb(), sf::Color::Red);
				for (auto& collider : body->GetColliders())
				{
					if (m_DrawAabb) DrawAabb(collider.GetAabb(), sf::Color::Green);
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

	void Debug::DrawSat(const p2Body* bodyA, const p2Body* bodyB) const
	{
		std::vector<p2Vec2> axes;
		const auto aabbA = bodyA->GetColliders()[0].GetAabb();
		const auto aabbB = bodyB->GetColliders()[0].GetAabb();

		for (auto& edge : aabbA.edges)
		{
			axes.push_back(edge.GetNormal());

			const auto linePos = aabbA.GetCenter() + edge * 2;
			m_Graphics2DManager->DrawLine(meter2pixel(linePos + edge.GetNormal() * 500), meter2pixel(linePos - edge.GetNormal() * 500), sf::Color::Blue);
		}

		for (auto& edge : aabbB.edges)
		{
			axes.push_back(edge.GetNormal());

			const auto linePos = aabbB.GetCenter() + edge * 2;
			m_Graphics2DManager->DrawLine(meter2pixel(linePos + edge.GetNormal() * 500), meter2pixel(linePos - edge.GetNormal() * 500), sf::Color::Blue);
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

				m_Graphics2DManager->DrawLine(meter2pixel(vertex.GetProjectionOn(axis)), meter2pixel(vertex), sf::Color::Magenta);
			}

			for (auto& vertex : aabbB.vertices)
			{
				const auto proj = p2Vec2::Dot(axis, vertex);
				if (proj < bMinProj) bMinProj = proj;
				if (proj > bMaxProj) bMaxProj = proj;
			}

			if (aMaxProj < bMinProj || bMaxProj < aMinProj)
			{
				return;
			}
		}

		m_Graphics2DManager->DrawLine(meter2pixel(bodyA->GetPosition()), meter2pixel(bodyB->GetPosition()),
			sf::Color::Green);
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
