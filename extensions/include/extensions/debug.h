#ifndef SFGE_EXT_DEBUG_H
#define SFGE_EXT_DEBUG_H

#include <engine/system.h>
#include <graphics/graphics2d.h>
#include "p2body.h"
#include "input/input.h"
#include <p2world.h>


namespace sfge
{
	struct Transform2d;
	class Transform2dManager;
	class Body2dManager;
	class TextureManager;
	class SpriteManager;
}

namespace sfge::ext
{
	class Debug : public System
	{
	public:
		explicit Debug(Engine& engine);

		void OnEngineInit() override;

		void OnUpdate(float dt) override;

		void OnFixedUpdate() override;

		void OnDraw() override;

		std::vector<p2Vec2> m_Contacts;
	private:
		Physics2dManager* m_PhysicsManager;
		p2World* m_World;

		Transform2dManager* m_Transform2DManager;
		Body2dManager* m_Body2DManager;
		Graphics2dManager* m_Graphics2DManager;

		InputManager* m_InputManager;
		KeyboardManager* m_KeyboardManager;

		void DrawAabb(p2Aabb aabb, sf::Color color) const;
		void DrawSat(const p2Body* body1, const p2Body* body2) const;
		void DrawQuadTree(p2QuadTree * quadTree, sf::Color color) const;

		p2QuadTree* m_QuadTree;
		std::vector<p2Vec2> m_Axes;

		sf::Vector2f m_ScreenSize;
		std::vector<Transform2d*> m_Transforms;
		std::vector<p2Body*> m_Bodies;
		std::vector<Entity> m_Entities;

		bool m_DrawAabb = false;
		bool m_DrawSat = false;
		bool m_DrawQuadTree = false;
	};
}

#endif
