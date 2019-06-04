#include "../../include/engine/system.h"
#include "../../include/graphics/graphics2d.h"
#include "../../include/engine/vector.h"
#include "../../externals/Physics2D/include/p2vector.h"
#include "../../externals/Remotery/Remotery.h"
#include "../../include/physics/physics2d.h"

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

		p2Vec2 m_Vector1{ 2, 0 };
		p2Vec2 m_VectorGoal1{ 2, 0 };
		int m_RotationSpeed1 = 40;
		float m_LerpSpeed1 = .1;
		float m_CurrentLerp1 = 0;

		p2Vec2 m_Vector2{ 1, 0 };
		p2Vec2 m_VectorGoal2{ 1, 0 };
		int m_RotationSpeed2 = 80;
		float m_LerpSpeed2 = .2;
		float m_CurrentLerp2 = 0;

		p2Vec2 m_Vector3{ .5, 0 };
		p2Vec2 m_VectorGoal3{ .5, 0 };
		int m_RotationSpeed3 = 160;
		float m_LerpSpeed3 = .4;
		float m_CurrentLerp3 = 0;

		p2Vec2 m_Vector4{ .25, 0 };
		p2Vec2 m_VectorGoal4{ .25, 0 };
		int m_RotationSpeed4 = 320;
		float m_LerpSpeed4 = .8;
		float m_CurrentLerp4 = 0;
	};
}

namespace sfge::ext
{

	VectorTest::VectorTest(Engine& engine) :
		System(engine)
	{
	}

	void VectorTest::OnEngineInit()
	{
		m_Graphics2DManager = m_Engine.GetGraphics2dManager();

		const auto config = m_Engine.GetConfig();
		m_ScreenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);
		m_ScreenHalfPos = Vec2f(m_ScreenSize.x / 2, m_ScreenSize.y / 2);
	}

	void VectorTest::OnUpdate(float dt)
	{
		(void)dt;
		m_Vector1 = p2Vec2::Lerp(p2Vec2::ZERO, m_VectorGoal1, m_CurrentLerp1);
		m_VectorGoal1 = m_VectorGoal1.Rotate(m_RotationSpeed1 * dt);
		m_CurrentLerp1 >= 1 ? m_LerpSpeed1 *= -1 : m_LerpSpeed1 *= 1;
		m_CurrentLerp1 += m_LerpSpeed1 * dt;

		m_Vector2 = p2Vec2::Lerp(p2Vec2::ZERO, m_VectorGoal2, m_CurrentLerp2);
		m_VectorGoal2 = m_VectorGoal2.Rotate(m_RotationSpeed2 * dt);
		m_CurrentLerp2 >= 1 ? m_LerpSpeed2 *= -1 : m_LerpSpeed2 *= 1;
		m_CurrentLerp2 += m_LerpSpeed2 * dt;

		m_Vector3 = p2Vec2::Lerp(p2Vec2::ZERO, m_VectorGoal3, m_CurrentLerp3);
		m_VectorGoal3 = m_VectorGoal3.Rotate(m_RotationSpeed3 * dt);
		m_CurrentLerp3 >= 1 ? m_LerpSpeed3 *= -1 : m_LerpSpeed3 *= 1;
		m_CurrentLerp3 += m_LerpSpeed3 * dt;

		m_Vector4 = p2Vec2::Lerp(p2Vec2::ZERO, m_VectorGoal4, m_CurrentLerp4);
		m_VectorGoal4 = m_VectorGoal4.Rotate(m_RotationSpeed4 * dt);
		m_CurrentLerp4 >= 1 ? m_LerpSpeed4 *= -1 : m_LerpSpeed4 *= 1;
		m_CurrentLerp4 += m_LerpSpeed4 * dt;
	}


	void VectorTest::OnFixedUpdate()
	{
		rmt_ScopedCPUSample(VectorTestFixedUpdate, 0);

	}

	void VectorTest::OnDraw()
	{
		rmt_ScopedCPUSample(VectorTestDraw, 0);
		m_Graphics2DManager->DrawVector(meter2pixel(m_VectorReflection), m_ScreenHalfPos + meter2pixel(m_Vector1), sf::Color::Yellow);

		m_Graphics2DManager->DrawVector(meter2pixel(m_Vector1), m_ScreenHalfPos);
		m_Graphics2DManager->DrawVector(meter2pixel(m_Vector2), m_ScreenHalfPos + meter2pixel(m_Vector1), sf::Color::Green);
		m_Graphics2DManager->DrawVector(meter2pixel(m_Vector3), m_ScreenHalfPos + meter2pixel(m_Vector1 + m_Vector2), sf::Color::Green);
		m_Graphics2DManager->DrawVector(meter2pixel(m_Vector4), m_ScreenHalfPos + meter2pixel(m_Vector1 + m_Vector2 + m_Vector3), sf::Color::Green);

		m_Graphics2DManager->DrawVector(meter2pixel(m_Vector1.GetReflection(m_VectorReflection)), m_ScreenHalfPos + meter2pixel(m_Vector1));
	}
}
