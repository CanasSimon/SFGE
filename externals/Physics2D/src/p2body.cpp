/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <p2body.h>

void p2Body::Init(p2BodyDef* bodyDef)
{
	m_BodyDefinition = *bodyDef;
	m_MaxColliderCount = bodyDef->maxColliderCount;
	m_Colliders.resize(m_MaxColliderCount);
	m_Position = bodyDef->position;
	m_Rotation = bodyDef->rotation;
	m_LinearVelocity = bodyDef->linearVelocity;
	m_Type = bodyDef->type;

	m_Aabb.topRight = m_Aabb.bottomLeft = m_Position;

	if (bodyDef->mass == 0) m_Mass = 1;
	else { m_Mass = bodyDef->mass; }

	m_Aabb.vertices.resize(4);
	m_Aabb.edges.resize(4);
	RebuildAabb();
}

void p2Body::RebuildAabb()
{
	if (m_Colliders.empty()) return;

	m_Aabb.topRight = m_Aabb.bottomLeft = m_Position;
	for (auto& collider : m_Colliders)
	{
		collider.RebuildAabb(m_Position, m_Rotation);

		const auto colliderAabb = collider.GetAabb();
		for (auto& vertex : colliderAabb.vertices)
		{
			if(vertex.x > m_Aabb.topRight.x)
			{
				m_Aabb.topRight.x = vertex.x;
			}
			if (vertex.x < m_Aabb.bottomLeft.x)
			{
				m_Aabb.bottomLeft.x = vertex.x;
			}

			if (vertex.y > m_Aabb.topRight.y)
			{
				m_Aabb.topRight.y = vertex.y;
			}
			if (vertex.y < m_Aabb.bottomLeft.y)
			{
				m_Aabb.bottomLeft.y = vertex.y;
			}
		}
	}

	m_Aabb.vertices[0] = m_Aabb.topRight;
	m_Aabb.vertices[1] = m_Aabb.topRight - p2Vec2(0, m_Aabb.GetExtends().y);
	m_Aabb.vertices[2] = m_Aabb.bottomLeft;
	m_Aabb.vertices[3] = m_Aabb.bottomLeft + p2Vec2(0, m_Aabb.GetExtends().y);

	m_Aabb.edges[0] = p2Vec2::GetVectorFrom(m_Aabb.vertices[0], m_Aabb.vertices[1]);
	m_Aabb.edges[1] = p2Vec2::GetVectorFrom(m_Aabb.vertices[1], m_Aabb.vertices[2]);
	m_Aabb.edges[2] = p2Vec2::GetVectorFrom(m_Aabb.vertices[2], m_Aabb.vertices[3]);
	m_Aabb.edges[3] = p2Vec2::GetVectorFrom(m_Aabb.vertices[3], m_Aabb.vertices[0]);
}

p2Vec2 p2Body::GetLinearVelocity() const
{
	return m_LinearVelocity;
}

void p2Body::SetLinearVelocity(p2Vec2 velocity)
{
	m_LinearVelocity = velocity;
}

float p2Body::GetAngularVelocity() const
{
	return m_AngularVelocity;
}

p2Vec2 p2Body::GetPosition() const
{
	return m_Position;
}

float p2Body::GetRotation() const
{
	return m_Rotation;
}

p2Collider* p2Body::CreateCollider(p2ColliderDef* colliderDef)
{
	p2Collider& collider = m_Colliders[m_ColliderIndex];
	collider.Init(colliderDef);
	m_ColliderIndex++;
	return &collider;
}

void p2Body::ApplyForceToCenter(const p2Vec2& force)
{
	m_LinearVelocity += force / m_Mass;
}

void p2Body::Offset(const p2Vec2 offset)
{
	m_Position += offset;
	for (auto& collider : m_Colliders)
	{
		collider.position = m_Position + collider.offset;
	}
}

void p2Body::SetPosition(const p2Vec2 position)
{
	this->m_Position = position;
}

void p2Body::SetRotation(const float rotation)
{
	this->m_Rotation = rotation;
}

p2BodyType p2Body::GetType() const
{
	return m_Type;
}

float p2Body::GetMass() const
{
	return m_Mass;
}

p2Aabb p2Body::GetAabb() const
{
	return m_Aabb;
}

std::vector<p2Collider> p2Body::GetColliders() const
{
	return m_Colliders;
}
