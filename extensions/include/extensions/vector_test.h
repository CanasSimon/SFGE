#ifndef SFGE_EXT_VECTOR_TEST_H
#define SFGE_EXT_VECTOR_TEST_H

#include <engine/system.h>
#include <SFML/System/Vector2.hpp>
#include "p2vector.h"
#include <graphics/graphics2d.h>

namespace sfge::ext
{
	class VectorTest : public System
	{
	public:
		explicit VectorTest(Engine& engine);

		void OnEngineInit() override;

		void OnUpdate(float dt) override;

		void OnFixedUpdate() override;

		void OnDraw() override;
	private:
		Graphics2dManager* m_Graphics2DManager;

		Vec2f m_ScreenSize;
		Vec2f m_ScreenHalfPos;

		p2Vec2 m_VectorReflection{ 0, 3 };

		p2Vec2 m_Vector1{2, 0};
		p2Vec2 m_VectorGoal1{ 2, 0 };
		int m_RotationSpeed1 = 40;
		float m_LerpSpeed1 = .1;
		float m_CurrentLerp1 = 0;

		p2Vec2 m_Vector2{1, 0};
		p2Vec2 m_VectorGoal2{1, 0};
		int m_RotationSpeed2 = 80;
		float m_LerpSpeed2 = .2;
		float m_CurrentLerp2 = 0;

		p2Vec2 m_Vector3{.5, 0};
		p2Vec2 m_VectorGoal3{.5, 0};
		int m_RotationSpeed3 = 160;
		float m_LerpSpeed3 = .4;
		float m_CurrentLerp3 = 0;

		p2Vec2 m_Vector4{.25, 0};
		p2Vec2 m_VectorGoal4{.25, 0};
		int m_RotationSpeed4 = 320;
		float m_LerpSpeed4 = .8;
		float m_CurrentLerp4 = 0;
	};
}

#endif
