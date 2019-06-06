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

#ifndef SFGE_P2CONTACT_H
#define SFGE_P2CONTACT_H

#include <p2body.h>

/**
* \brief Representation of a contact given as argument in a p2ContactListener
*/
class p2Contact
{
public:
	p2Contact(p2Collider* colliderA, p2Collider* colliderB);

	p2Collider* GetColliderA() const;
	p2Collider* GetColliderB() const;

	p2Vec2 contactPoint;
	p2Vec2 mtv;
	p2Vec2 normal;
private:
	p2Collider* m_ColliderA = nullptr;
	p2Collider* m_ColliderB = nullptr;
};

/**
* \brief Listener of contacts happening in an attached p2World
*/
class p2ContactListener
{
public:
	virtual void AddContact(p2Contact* contact) = 0;
	virtual void DeleteContact(p2Contact* contact) = 0;
};

/**
* \brief Managing the creation and destruction of contact between colliders
*/
class p2ContactManager
{
public:
	void TestContacts(p2Body& bodyA, p2Body& bodyB);
	int CheckContact(p2Contact contact);

	static bool CheckSat(p2Contact* contact);
	static bool CheckBoxSat(p2Contact* contact);
	static bool CheckCircleSat(p2Contact* contact);
	static bool CheckCircleBoxSat(p2Contact* contact);

	std::vector<p2Contact*> possibleContacts;

	p2ContactListener* contactListener;
};
#endif