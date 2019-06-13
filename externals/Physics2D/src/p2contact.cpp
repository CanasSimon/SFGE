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

/**
 * \brief Default Constructor
 * \param colliderA first collider of contact
 * \param colliderB second collider of contact
 */
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

/**
 * \brief Check if contact needs to be created, updated or deleted
 * \param bodyA first body to test contacts with
 * \param bodyB second body to test contacts with
 */
void p2ContactManager::TestContacts(p2Body& bodyA, p2Body& bodyB)
{
	auto colliderA = bodyA.GetColliders()[0];
	auto colliderB = bodyB.GetColliders()[0];
	const auto contact = new p2Contact(&colliderA, &colliderB);

	const auto index = CheckContact(*contact);

	if (index == -1)
	{
		if (CheckSat(contact))
		{
			CorrectPositions(bodyA, bodyB, contact);
		}
	}
	else
	{
		if (!p2Aabb::DoOverlapWith(bodyA.GetAabb(), bodyB.GetAabb()) || !CheckSat(contact))
		{
			contactListener->DeleteContact(contact);
			possibleContacts.erase(possibleContacts.begin() + index);
			delete contact;
		}
		else
		{
			possibleContacts[index] = contact;

			CorrectPositions(bodyA, bodyB, contact);
		}
	}
}

/**
 * \brief Check whether contact already exists or not
 * \param contact the contact to be checked
 * \return Contact index in possibleContacts if contact exist, -1 if not
 */
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

void p2ContactManager::CorrectPositions(p2Body& bodyA, p2Body& bodyB, p2Contact* contact)
{
	const auto colliderA = bodyA.GetColliders()[0];
	const auto colliderB = bodyB.GetColliders()[0];

	const auto velocityA = bodyA.GetLinearVelocity();
	const auto velocityB = bodyB.GetLinearVelocity();

	contactListener->AddContact(contact);
	possibleContacts.push_back(contact);

	if (bodyA.GetType() != p2BodyType::STATIC)
	{
		bodyA.SetLinearVelocity(
			velocityB * colliderB.restitution + p2Vec2(velocityA.x * colliderA.friction, velocityA.y * colliderA.bounce).
			GetReflection(contact->normal) * (1 - colliderB.restitution));

		bodyA.Offset(-contact->mtv);
	}
	if (bodyB.GetType() != p2BodyType::STATIC)
	{
		bodyB.SetLinearVelocity(
			velocityA * colliderA.restitution + p2Vec2(velocityB.x * colliderB.friction, velocityB.y * colliderB.bounce).
			GetReflection(contact->normal) * (1 - colliderA.restitution));

		bodyB.Offset(contact->mtv);
	}
}

/**
 * \brief Check which type of SAT to call for the colliders
 * \param contact the contact to check
 * \return if the colliders have a contact
 */
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
			return CheckCircleRectSat(contact);
		default:
			return false;
		}
	case p2ColliderType::RECT:
		switch (colliderB->GetType())
		{
		case p2ColliderType::CIRCLE:
			return CheckCircleRectSat(contact);
		case p2ColliderType::RECT:
			return CheckRectSat(contact);
		default:
			return false;
		}
	default:
		return false;
	}
}

/**
 * \brief Check SAT between two rect
 * \param contact the contact to check
 * \return if the colliders have a contact
 */
bool p2ContactManager::CheckRectSat(p2Contact* contact)
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

	p2Aabb minkowski;
	minkowski.topRight = aabbA.topRight - aabbB.bottomLeft;
	minkowski.bottomLeft = aabbA.bottomLeft - aabbB.topRight;

	auto min = std::numeric_limits<float>::infinity();
	if (abs(minkowski.bottomLeft.x) < min) {
		min = abs(minkowski.bottomLeft.x);
		contact->mtv = p2Vec2(minkowski.bottomLeft.x, 0.f);
	}
	if (abs(minkowski.topRight.x) < min) {
		min = abs(minkowski.topRight.x);
		contact->mtv = p2Vec2(minkowski.topRight.x, 0.f);
	}
	if (abs(minkowski.topRight.y) < min) {
		min = abs(minkowski.topRight.y);
		contact->mtv = p2Vec2(0.f, minkowski.topRight.y);
	}
	if (abs(minkowski.bottomLeft.y) < min) {
		min = abs(minkowski.bottomLeft.y);
		contact->mtv = p2Vec2(0.f, minkowski.bottomLeft.y);
	}

	return true;
}

/**
 * \brief Check SAT between two circles
 * \param contact the contact to check
 * \return if the colliders have a contact
 */
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

/**
 * \brief Check SAT between a circle and a rect
 * \param contact the contact to check
 * \return if the colliders have a contact
 */
bool p2ContactManager::CheckCircleRectSat(p2Contact* contact)
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
