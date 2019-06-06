#include <iostream>
#include <extensions/debug.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <graphics/graphics2d.h>
#include <physics/body2d.h>
#include <physics/physics2d.h>
#include "imgui.h"


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

		if (m_KeyboardManager->IsKeyHeld(sf::Keyboard::Key::Up)) m_Bodies[0]->ApplyForceToCenter(p2Vec2(0, -.1));
		if (m_KeyboardManager->IsKeyHeld(sf::Keyboard::Key::Down)) m_Bodies[0]->ApplyForceToCenter(p2Vec2(0, .1));
		if (m_KeyboardManager->IsKeyHeld(sf::Keyboard::Key::Right)) m_Bodies[0]->ApplyForceToCenter(p2Vec2(.1, 0));
		if (m_KeyboardManager->IsKeyHeld(sf::Keyboard::Key::Left)) m_Bodies[0]->ApplyForceToCenter(p2Vec2(-.1, 0));

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
		rmt_ScopedCPUSample(DebugFixedUpdate, 0);
	}

	void Debug::OnDraw()
	{
		rmt_ScopedCPUSample(DebugDraw, 0);
		ImGui::Begin("Debug");
		if (ImGui::Button("AABB / OBB")) m_DrawAabb = !m_DrawAabb;
		if (ImGui::Button("QuadTree")) m_DrawQuadTree = !m_DrawQuadTree;
		ImGui::End();

		for (auto& contact : m_World->GetContactManager().possibleContacts)
		{
			m_Graphics2DManager->DrawVector(meter2pixel(-contact->mtv), Vec2f(400, 400), sf::Color::Yellow);
			m_Graphics2DManager->DrawVector(meter2pixel(contact->mtv), Vec2f(400,400), sf::Color::Yellow);
		}

		if (m_DrawQuadTree)
		{
			DrawQuadTree(m_QuadTree, sf::Color::Red);
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
