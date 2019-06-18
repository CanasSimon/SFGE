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


#ifndef SFGE_P2_COLLIDER_H
#define SFGE_P2_COLLIDER_H

#include <p2shape.h>
#include <p2aabb.h>

enum class p2ColliderType
{
	NONE,
	CIRCLE,
	RECT,
	POLY
};

/**
* \brief Struct defining a p2Collider when creating one
*/
struct p2ColliderDef
{
	void* userData = nullptr;
	p2Shape* shape{};
	p2Vec2 position;
	p2Vec2 offset = { 0, 0 };
	bool isSensor = false;
	p2ColliderType colliderType = p2ColliderType::NONE;

	float restitution = 1;
	float bounce = 1;
	float friction = 1;
};

/**
* \brief Representation of a Collider attached to a p2Body
*/

class p2Collider
{
public:
	void Init(p2ColliderDef* colliderDef);
	void RebuildAabb(const p2Vec2& bodyPos, float bodyRot);
	p2Collider();
	void* GetUserData() const;
	p2Shape* GetShape() const;
	p2ColliderType GetType() const;
	void SetUserData(void* colliderData);
	p2Vec2 GetHalfExtend() const;
	p2Aabb GetAabb() const;

	p2Vec2 position;
	p2Vec2 offset = {0, 0};

	float restitution = 0;
	float bounce = 0;
	float friction = 0;

	bool isSensor;
private:
	p2Aabb m_Aabb;
	void* m_UserData = nullptr;
	p2Shape* m_Shape = nullptr;
	p2ColliderType m_ColliderType = p2ColliderType::NONE;
	p2ColliderDef m_ColliderDefinition;
	p2Vec2 m_HalfExtend = p2Vec2(0, 0);
};


#endif
