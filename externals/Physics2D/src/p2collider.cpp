#include "..\include\p2collider.h"

p2Collider::p2Collider()
{
}

p2Collider::p2Collider(p2ColliderDef colDef): m_Shape(), m_ColliderType()
{
	colliderDefinition = colDef;

	switch (m_ColliderType)
	{
	case p2ColliderType::CIRCLE:
	{
		const auto circleShape = static_cast<p2CircleShape*>(m_Shape);
		halfExtend = p2Vec2(circleShape->GetRadius(), circleShape->GetRadius());
		//std::cout << "circle " + std::to_string(circle_shape->GetRadius()) << std::endl; //Debug
		break;
	}
	case p2ColliderType::RECT:
	{
		const auto rectShape = static_cast<p2RectShape*>(m_Shape);
		halfExtend = rectShape->GetSize();
		//std::cout << "box " + std::to_string(extend.x) << std::endl; //Debug
		break;
	}
	case p2ColliderType::POLY:
	{
		halfExtend = p2Vec2(0, 0);
		break;
	}
	default:
	{
		halfExtend = p2Vec2(0, 0);
		//std::cout << "none " + std::to_string(extend.x) << std::endl; //Debug
		break;
	}
	}
}

bool p2Collider::IsSensor() const
{
	return colliderDefinition.isSensor;
}

void * p2Collider::GetUserData() const
{
	return userData;
}

p2Shape* p2Collider::GetShape() const
{
	return colliderDefinition.shape;
}

void p2Collider::SetUserData(void* colliderData)
{
	userData = colliderData;
}

p2Vec2 p2Collider::GetHalfExtend() const
{
	return halfExtend;
}
