#include "../include/p2collider.h"

p2Collider::p2Collider() = default;

void p2Collider::Init(p2ColliderDef* colliderDef)
{
	m_UserData = colliderDef->userData;
	m_Shape = colliderDef->shape;
	position = colliderDef->position;
	offset = colliderDef->offset;
	m_ColliderType = colliderDef->colliderType;
	m_ColliderDefinition = *colliderDef;

	switch (m_ColliderType)
	{
	case p2ColliderType::CIRCLE:
		{
			const auto circleShape = dynamic_cast<p2CircleShape*>(m_Shape);
			m_HalfExtend = p2Vec2(circleShape->GetRadius(), circleShape->GetRadius());

			m_Aabb.vertices.resize(4);
			m_Aabb.edges.resize(4);
			break;
		}
	case p2ColliderType::RECT:
		{
			const auto rectShape = dynamic_cast<p2RectShape*>(m_Shape);
			m_HalfExtend = rectShape->GetSize();

			m_Aabb.vertices.resize(4);
			m_Aabb.edges.resize(4);
			break;
		}
	case p2ColliderType::POLY:
		{
			const auto polyShape = static_cast<p2PolygonShape*>(m_Shape);

			p2Vec2 extend;
			for (auto& point : polyShape->GetPoints())
			{
				if (point.x > extend.x) extend.x = point.x;
				if (point.y > extend.y) extend.y = point.y;
			}

			m_HalfExtend = extend;
			break;
		}
	default:
		{
			m_HalfExtend = p2Vec2(0, 0);
			break;
		}
	}


}

void p2Collider::RebuildAabb(const p2Vec2& bodyPos, float bodyRot)
{
	m_Aabb.topRight = bodyPos + (offset + m_HalfExtend).Rotate(bodyRot);
	m_Aabb.bottomLeft = bodyPos + (offset - m_HalfExtend).Rotate(bodyRot);

	switch (m_ColliderType)
	{
	case p2ColliderType::CIRCLE:
		{
			m_Aabb.vertices[0] = bodyPos + offset + m_HalfExtend;
			m_Aabb.vertices[1] = bodyPos + offset + p2Vec2(m_HalfExtend.x, -m_HalfExtend.y);
			m_Aabb.vertices[2] = bodyPos + offset - m_HalfExtend;
			m_Aabb.vertices[3] = bodyPos + offset + p2Vec2(-m_HalfExtend.x, m_HalfExtend.y);

			m_Aabb.edges[0] = p2Vec2::GetVectorFrom(m_Aabb.vertices[0], m_Aabb.vertices[1]);
			m_Aabb.edges[1] = p2Vec2::GetVectorFrom(m_Aabb.vertices[1], m_Aabb.vertices[2]);
			m_Aabb.edges[2] = p2Vec2::GetVectorFrom(m_Aabb.vertices[2], m_Aabb.vertices[3]);
			m_Aabb.edges[3] = p2Vec2::GetVectorFrom(m_Aabb.vertices[3], m_Aabb.vertices[0]);
		}
		break;
	case p2ColliderType::RECT:
		{
			m_Aabb.vertices[0] = bodyPos + (offset + m_HalfExtend).Rotate(bodyRot);
			m_Aabb.vertices[1] = bodyPos + (offset + p2Vec2(m_HalfExtend.x, -m_HalfExtend.y)).Rotate(bodyRot);
			m_Aabb.vertices[2] = bodyPos + (offset - m_HalfExtend).Rotate(bodyRot);
			m_Aabb.vertices[3] = bodyPos + (offset + p2Vec2(-m_HalfExtend.x, m_HalfExtend.y)).Rotate(bodyRot);

			m_Aabb.edges[0] = p2Vec2::GetVectorFrom(m_Aabb.vertices[1], m_Aabb.vertices[2]);
			m_Aabb.edges[1] = p2Vec2::GetVectorFrom(m_Aabb.vertices[2], m_Aabb.vertices[3]);
			m_Aabb.edges[2] = p2Vec2::GetVectorFrom(m_Aabb.vertices[3], m_Aabb.vertices[0]);
			m_Aabb.edges[3] = p2Vec2::GetVectorFrom(m_Aabb.vertices[0], m_Aabb.vertices[1]);
		}
		break;
	default: 
		break;
	}
}

bool p2Collider::IsSensor() const
{
	return m_ColliderDefinition.isSensor;
}

void* p2Collider::GetUserData() const
{
	return m_UserData;
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
	m_UserData = colliderData;
}

p2Vec2 p2Collider::GetHalfExtend() const
{
	return m_HalfExtend;
}

p2Aabb p2Collider::GetAabb() const
{
	return m_Aabb;
}
