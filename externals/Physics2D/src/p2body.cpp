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
	maxColliderCount = bodyDef->maxColliderCount;
	m_Colliders.resize(maxColliderCount);
	position = bodyDef->position;
	linearVelocity = bodyDef->linearVelocity;
	type = bodyDef->type;

	if (bodyDef->mass == 0) mass = 1;
	else { mass = bodyDef->mass; }
}

void p2Body::RebuildAABB()
{
	if (m_Colliders.empty()) return;

	for (auto& collider : m_Colliders)
	{
		collider.RebuildAABB(position);
	}
}

p2Vec2 p2Body::GetLinearVelocity() const
{
	return linearVelocity;
}

void p2Body::SetLinearVelocity(p2Vec2 velocity)
{
	linearVelocity = velocity;
}

float p2Body::GetAngularVelocity() const
{
	return angularVelocity;
}

p2Vec2 p2Body::GetPosition() const
{
	return position;
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
	linearVelocity += force / mass;
}

void p2Body::Offset(const p2Vec2 offset)
{
	position += offset;
}

void p2Body::SetPosition(const p2Vec2 position)
{
	this->position = position;
}

p2BodyType p2Body::GetType() const
{
	return type;
}

float p2Body::GetMass() const
{
	return mass;
}

p2AABB p2Body::GetAABB() const
{
	return aabb;
}

std::vector<p2Collider> p2Body::GetColliders() const
{
	return m_Colliders;
}
