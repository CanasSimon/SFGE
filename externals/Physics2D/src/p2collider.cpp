#include "..\include\p2collider.h"

p2Collider::p2Collider(): m_ColliderType()

{
}

void p2Collider::Init(p2ColliderDef* colliderDef)
{
	userData = colliderDef->userData;
	m_Shape = colliderDef->shape;
	m_Position = colliderDef->position;
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

			aabb.m_Vertices.resize(4);
			break;
		}
	case p2ColliderType::POLY:
		{
			const auto polyShape = dynamic_cast<p2PolygonShape*>(m_Shape);

			float right = 0;
			float top = 0;
			for (auto& point : polyShape->GetPoints())
			{
				if (point.x > right) right = point.x;
				if (point.y > top) top = point.y;
			}

			halfExtend = p2Vec2(right, top);
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
	aabb.right = p2Vec2(bodyPos.x + halfExtend.x, bodyPos.y);
	aabb.left = p2Vec2(bodyPos.x - halfExtend.x, bodyPos.y);
	aabb.top = p2Vec2(bodyPos.x, bodyPos.y + halfExtend.y);
	aabb.bottom = p2Vec2(bodyPos.x, bodyPos.y - halfExtend.y);

	switch (m_ColliderType)
	{
	case p2ColliderType::RECT:
		{
			aabb.m_Vertices[0] = bodyPos + halfExtend;
			aabb.m_Vertices[1] = bodyPos + p2Vec2(halfExtend.x, -halfExtend.y);
			aabb.m_Vertices[2] = bodyPos - halfExtend;
			aabb.m_Vertices[3] = bodyPos - p2Vec2(halfExtend.x, -halfExtend.y);
		}
		break;
	default: 
		break;
	}
}

p2Collider::p2Collider(p2ColliderDef colDef): m_ColliderType()
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

/*p2Body* p2Collider::GetBody() const
{
	return m_Body;
}*/

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
