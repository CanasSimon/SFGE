#include "..\include\p2collider.h"
#include <iostream>

p2Collider::p2Collider(): m_Shape(), m_ColliderType()

{
}

void p2Collider::Init(p2ColliderDef* colliderDef)
{
	userData = colliderDef->userData;
	m_Shape = colliderDef->shape;
	m_ColliderType = colliderDef->m_ColliderType;
	colliderDefinition = *colliderDef;

	switch (m_ColliderType)
	{
	case p2ColliderType::CIRCLE:
		{
			const auto circleShape = dynamic_cast<p2CircleShape*>(m_Shape);
			halfExtend = p2Vec2(circleShape->GetRadius(), circleShape->GetRadius());
			break;
		}
	case p2ColliderType::RECT:
		{
			const auto rectShape = dynamic_cast<p2RectShape*>(m_Shape);
			halfExtend = rectShape->GetSize();
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
			break;
		}
	}
}

void p2Collider::RebuildAABB(p2Vec2 bodyPos)
{
	aabb.right = bodyPos.x + halfExtend.x;
	aabb.left = bodyPos.x - halfExtend.x;
	aabb.top = bodyPos.y + halfExtend.y;
	aabb.bottom = bodyPos.y - halfExtend.y;
}

p2Collider::p2Collider(p2ColliderDef colDef): m_Shape(), m_ColliderType(), colliderDefinition()
{
}

bool p2Collider::IsSensor() const
{
	return colliderDefinition.isSensor;
}

void* p2Collider::GetUserData() const
{
	return userData;
}

p2Shape* p2Collider::GetShape() const
{
	return m_Shape;
}

p2ColliderType p2Collider::GetType() const
{
	return m_ColliderType;
}

void p2Collider::SetUserData(void* colliderData)
{
	userData = colliderData;
}

p2Vec2 p2Collider::GetHalfExtend() const
{
	return halfExtend;
}

p2AABB p2Collider::GetAABB() const
{
	return aabb;
}
