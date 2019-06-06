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

#include <p2contact.h>
#include <iostream>

p2Contact::p2Contact(p2Collider* colliderA, p2Collider* colliderB)
{
	m_ColliderA = colliderA;
	m_ColliderB = colliderB;
}

p2Collider* p2Contact::GetColliderA() const
{
	return m_ColliderA;
}

p2Collider* p2Contact::GetColliderB() const
{
	return m_ColliderB;
}

void p2ContactManager::TestContacts(p2Body& bodyA, p2Body& bodyB)
{
	auto colliderA = bodyA.GetColliders()[0];
	auto colliderB = bodyB.GetColliders()[0];
	const auto contact = new p2Contact(&colliderA, &colliderB);
	const auto index = CheckContact(*contact);
	contact->normal = p2Vec2(0, 1).Rotate(bodyB.GetRotation());

	const auto velocityA = bodyA.GetLinearVelocity().GetReflection(contact->normal);
	const auto velocityB = bodyB.GetLinearVelocity().GetReflection(contact->normal);

	const auto friction = 0.8;
	const auto bounce = 0.8;

	if(index == -1)
	{
		if (CheckSat(contact))
		{
			contactListener->AddContact(contact);
			possibleContacts.push_back(contact);

			if(bodyA.GetType() != p2BodyType::STATIC)
			{
				bodyA.SetLinearVelocity(p2Vec2(velocityA.x * friction, velocityA.y * bounce));
				bodyA.Offset(-contact->mtv);
			}
			if (bodyB.GetType() != p2BodyType::STATIC)
			{
				bodyB.SetLinearVelocity(p2Vec2(velocityB.x * friction, velocityB.y * bounce));
				bodyB.Offset(contact->mtv);
			}
		}
	}
	else
	{
		if (!CheckSat(contact))
		{
			contactListener->DeleteContact(contact);
			possibleContacts.erase(possibleContacts.begin() + index);
		}
		else
		{
			if (bodyA.GetType() != p2BodyType::STATIC)
			{
				bodyA.SetLinearVelocity(p2Vec2(velocityA.x * friction, velocityA.y * bounce));
				bodyA.Offset(-contact->mtv);
			}
			if (bodyB.GetType() != p2BodyType::STATIC)
			{
				bodyB.SetLinearVelocity(p2Vec2(velocityB.x * friction, velocityB.y * bounce));
				bodyB.Offset(contact->mtv);
			}
		}
	}
}

int p2ContactManager::CheckContact(p2Contact contact)
{
	for (auto i = 0u; i < possibleContacts.size(); ++i)
	{
		if (possibleContacts[i]->GetColliderA() == contact.GetColliderA() && 
			possibleContacts[i]->GetColliderB() == contact.GetColliderB() || 
			possibleContacts[i]->GetColliderA() == contact.GetColliderB() &&
			possibleContacts[i]->GetColliderB() == contact.GetColliderA()) return i;
	}

	return -1;
}

bool p2ContactManager::CheckSat(p2Contact* contact)
{
	const auto colliderA = contact->GetColliderA();
	const auto colliderB = contact->GetColliderB();

	switch (colliderA->GetType())
	{
	case p2ColliderType::CIRCLE:
		switch (colliderB->GetType())
		{
		case p2ColliderType::CIRCLE:
			return CheckCircleSat(contact);
		case p2ColliderType::RECT:
			return CheckCircleBoxSat(contact);
		default:
			return false;
		}
	case p2ColliderType::RECT:
		switch (colliderB->GetType())
		{
		case p2ColliderType::CIRCLE:
			return CheckCircleBoxSat(contact);
		case p2ColliderType::RECT:
			return CheckBoxSat(contact);
		default:
			return false;
		}
	default:
		return false;
	}
}

bool p2ContactManager::CheckBoxSat(p2Contact* contact)
{
	//Code edited from https://gist.github.com/nyorain/dc5af42c6e83f7ac6d831a2cfd5fbece

	const auto colliderA = contact->GetColliderA();
	const auto colliderB = contact->GetColliderB();

	std::vector<p2Vec2> axes;
	const auto aabbA = colliderA->GetAabb();
	const auto aabbB = colliderB->GetAabb();
	axes.reserve(aabbA.edges.size() / 2 + aabbB.edges.size() / 2);

	switch (colliderA->GetType())
	{
	case p2ColliderType::RECT:
		axes.insert(axes.end(), aabbA.edges.begin(), aabbA.edges.begin() + aabbA.edges.size() / 2);
		switch (colliderA->GetType())
		{
		case p2ColliderType::RECT:
			axes.insert(axes.end(), aabbB.edges.begin(), aabbB.edges.begin() + aabbB.edges.size() / 2);
			break;
		case p2ColliderType::POLY:
			axes.insert(axes.end(), aabbB.edges.begin(), aabbB.edges.end());
			break;
		default:
			std::cerr << "Error. Collider A is of invalid type" << std::endl;
			break;
		}
		break;
	case p2ColliderType::POLY:
		axes.insert(axes.end(), aabbA.edges.begin(), aabbA.edges.end());
		switch (colliderA->GetType())
		{
		case p2ColliderType::RECT:
			axes.insert(axes.end(), aabbB.edges.begin(), aabbB.edges.begin() + aabbB.edges.size() / 2);
			break;
		case p2ColliderType::POLY:
			axes.insert(axes.end(), aabbB.edges.begin(), aabbB.edges.end());
			break;
		default:
			std::cerr << "Error. Collider A is of invalid type" << std::endl;
			break;
		}
		break;
	default:
		std::cerr << "Error. Collider A is of invalid type" << std::endl;
		break;
	}

	for (auto& axis : axes)
	{
		axis = axis.GetNormal();

		auto aMaxProj = -std::numeric_limits<float>::infinity();
		auto aMinProj = std::numeric_limits<float>::infinity();
		auto bMaxProj = -std::numeric_limits<float>::infinity();
		auto bMinProj = std::numeric_limits<float>::infinity();

		for (auto& vertex : aabbA.vertices)
		{
			const auto proj = p2Vec2::Dot(axis, vertex);
			if (proj < aMinProj) aMinProj = proj;
			if (proj > aMaxProj) aMaxProj = proj;
		}

		for (auto& vertex : aabbB.vertices)
		{
			const auto proj = p2Vec2::Dot(axis, vertex);
			if (proj < bMinProj) bMinProj = proj;
			if (proj > bMaxProj) bMaxProj = proj;
		}

		if (aMaxProj < bMinProj || bMaxProj < aMinProj) return false;
	}

	return true;
}

bool p2ContactManager::CheckCircleSat(p2Contact* contact)
{
	const auto colliderA = contact->GetColliderA();
	const auto colliderB = contact->GetColliderB();

	const auto distance = colliderA->position.GetDistance(colliderB->position);
	contact->normal = p2Vec2::GetVectorFrom(colliderA->position, colliderB->position).Normalized();
	const auto vector1 = p2Vec2::GetVectorFrom(colliderA->position, colliderA->position + p2Vec2(colliderA->GetHalfExtend().x, 0));
	const auto vector2 = p2Vec2::GetVectorFrom(colliderB->position, colliderB->position + p2Vec2(colliderB->GetHalfExtend().x, 0));
	contact->mtv = contact->normal * (colliderA->GetHalfExtend().x + colliderB->GetHalfExtend().x - distance);

	return distance <= colliderA->GetHalfExtend().x + colliderB->GetHalfExtend().x;
}

bool p2ContactManager::CheckCircleBoxSat(p2Contact* contact)
{
	auto colliderA = contact->GetColliderA();
	auto colliderB = contact->GetColliderB();

	if (colliderA->GetType() == p2ColliderType::RECT)
	{
		std::swap(colliderA, colliderB);
	}

	const auto aabbA = colliderA->GetAabb();
	const auto aabbB = colliderB->GetAabb();

	for (auto i = 0u; i < aabbB.vertices.size() / 2; ++i)
	{
		if (aabbA.GetCenter().LineSide(aabbB.vertices[i], aabbB.vertices[i + 1]) == 1 &&
			aabbA.GetCenter().LineSide(aabbB.vertices[i + 2], aabbB.vertices[(i + 3) % aabbB.vertices.size()]) == 1)
		{
			goto InZone;
		}
	}
	goto NotInZone;

InZone:
	for (auto i = 0u; i < aabbB.edges.size() / 2; ++i)
	{
		const auto aPoint1 = aabbA.GetCenter() - colliderA->GetHalfExtend();
		const auto aPoint2 = aabbA.GetCenter() + colliderA->GetHalfExtend();

		const auto aMaxProj = p2Vec2::Dot(aabbA.edges[i], aPoint1);
		const auto aMinProj = p2Vec2::Dot(aabbA.edges[i], aPoint2);
		auto bMaxProj = -std::numeric_limits<float>::infinity();
		auto bMinProj = std::numeric_limits<float>::infinity();

		for (auto& vertex : aabbB.vertices)
		{
			const auto proj = p2Vec2::Dot(aabbA.edges[i], vertex);
			if (proj < bMinProj) bMinProj = proj;
			if (proj > bMaxProj) bMaxProj = proj;
		}

		//contact->mtv = vector2 - vector1;

		if (aMaxProj < bMinProj || bMaxProj < aMinProj)
		{
			return false;
		}
	}

	contact->normal = aabbB.edges[0].GetNormal().Normalized();

	return true;

NotInZone:
	auto dist = std::numeric_limits<float>::infinity();
	for (auto& vertex : aabbB.vertices)
	{
		if (vertex.GetDistance(aabbA.GetCenter()) < dist)
		{
			dist = vertex.GetDistance(aabbA.GetCenter());
			const auto vector1 = p2Vec2::GetVectorFrom(aabbA.GetCenter(), vertex);
			const auto vector2 = p2Vec2::GetVectorFrom(aabbA.GetCenter(), vertex).Normalized() * colliderA->GetHalfExtend().x;
			contact->mtv = vector2 - vector1;
			contact->normal = vector1.Normalized();
		}
	}
	return dist < colliderA->GetHalfExtend().x;
}
