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
#include <p2world.h>
#include <p2quadtree.h>
#include <iostream>


p2World::p2World(const p2Vec2& gravity): m_Gravity(gravity), m_ContactListener()
{
	m_Bodies.resize(MAX_BODY_LEN);

	m_QuadTreeBounds.bottomLeft = p2Vec2(0, 0);
	m_QuadTreeBounds.topRight = p2Vec2(1, 1);

	m_QuadTree = new p2QuadTree(0, m_QuadTreeBounds);
}

void p2World::Step(const float dt)
{
	m_QuadTree->Clear();

	for (auto& body : m_Bodies)
	{
		if (body.GetType() == p2BodyType::NONE) continue;

		m_QuadTree->Insert(&body);
		if (body.GetType() == p2BodyType::DYNAMIC)
		{
			body.Offset(body.GetLinearVelocity() * dt);
			body.ApplyForceToCenter(m_Gravity * dt);
		}
	}

	/*for (auto& bodyA : m_Bodies)
	{
		if (bodyA.GetType() == p2BodyType::NONE) continue;

		const auto retrieve = m_QuadTree->Retrieve(&bodyA);
		for (auto& bodyB : retrieve)
		{
			if (&bodyA == bodyB) continue;
			m_ContactManager.TestContacts(bodyA, *bodyB);
		}
	}*/

	m_ContactManager.TestContacts(m_Bodies[0], m_Bodies[1]);
	//m_Bodies[0].ApplyForceToCenter(m_Gravity * dt);

	for (auto& contact : m_ContactManager.contacts)
	{
		contact->isAlreadyUpdated = false;
	}
}

p2Body* p2World::CreateBody(p2BodyDef* bodyDef)
{
	if (m_BodyInitIndex + 1 > m_Bodies.size())
	{
		m_Bodies.resize(m_Bodies.size() + MAX_BODY_LEN);
	}

	auto& body = m_Bodies[m_BodyInitIndex];
	body.Init(bodyDef);
	m_BodyInitIndex++;

	return &body;
}

void p2World::SetContactListener(p2ContactListener* contactLis)
{
	m_ContactListener = contactLis;
	m_ContactManager.contactListener = contactLis;
}

p2ContactListener* p2World::GetContactListener() const
{
	return m_ContactListener;
}

p2ContactManager p2World::GetContactManager() const
{
	return m_ContactManager;
}

p2QuadTree* p2World::GetQuadTree() const
{
	return m_QuadTree;
}
