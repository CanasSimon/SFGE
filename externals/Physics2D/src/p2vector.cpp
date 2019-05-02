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

#include <p2vector.h>
#include <cmath>
#include "engine/vector.h"

/*********************
 * p2Vec2 operations *
 *********************/

p2Vec2::p2Vec2()
{
	x = 1.0f;
	y = 1.0f;
}

p2Vec2::p2Vec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

bool p2Vec2::operator==(const p2Vec2& rhs) const
{
	return x == rhs.x && y == rhs.y;
}

bool p2Vec2::operator!=(const p2Vec2& rhs) const
{
	return x != rhs.x && y != rhs.y;
}

p2Vec2 p2Vec2::operator+(const p2Vec2& v) const
{
	return {x + v.x, y + v.y};
}

p2Vec2& p2Vec2::operator+=(const p2Vec2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

p2Vec2 p2Vec2::operator-(const p2Vec2& v) const
{
	return {x-v.x, y-v.y};
}

p2Vec2& p2Vec2::operator-=(const p2Vec2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

p2Vec2& p2Vec2::operator*=(float f)
{
	x *= f;
	y *= f;
	return (*this);
}

p2Vec2 p2Vec2::operator/(float f) const
{
	return {x/f, y/f};
}

p2Vec2 p2Vec2::operator*(float f) const
{
	return {x*f, y*f};
}

p2Vec2 p2Vec2::operator*(const p2Vec2& v) const
{
	return { x * v.x, y * v.y };
}

float p2Vec2::Dot(p2Vec2 v1, p2Vec2 v2)
{
	//TODO
	return (v1.x * v2.x) + (v1.y * v2.y);
}
float p2Vec2::Cross(p2Vec2 v1, p2Vec2 v2)
{
	return (v1.x * v2.y) - (v2.x * v2.y);
}
float p2Vec2::GetMagnitude() const
{
	//TODO
	return sqrt(pow(x, 2) + pow(y, 2));
}

p2Vec2 p2Vec2::Normalized() const
{
	const float magnitude = GetMagnitude();
	return {x / magnitude, y / magnitude};
}

void p2Vec2::NormalizeSelf()
{
	const float magnitude = GetMagnitude();

	x = x / magnitude;
	y = y / magnitude;
}

p2Vec2 p2Vec2::Rotate(float angle) const
{
	return {x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle)};
}

p2Vec2 p2Vec2::Lerp(const p2Vec2& v1, const p2Vec2& v2, float t)
{
	return v1 + v2 * t;
}

float p2Vec2::AngleBetween(const p2Vec2& v1, const p2Vec2& v2)
{
	return acos(Dot(v1, v2) / (v1.GetMagnitude() * v2.GetMagnitude()));
}

void p2Vec2::ProjectOn(const p2Vec2& v1)
{
	const float dot = Dot(*this, v1);
	x = dot / pow(v1.GetMagnitude(), 2) * v1.x;
	y = dot / pow(v1.GetMagnitude(), 2) * v1.y;
}

p2Vec2 p2Vec2::GetNormal() const
{
	return {y, -x};
}

p2Vec2 p2Vec2::GetReflection(const p2Vec2& n) const
{
	p2Vec2 nor = n.GetNormal().Normalized();
	const float dot = Dot(*this, nor);
	return *this - nor * 2 * Dot(*this, nor);
}

/*********************
 * p2Vec3 operations *
 *********************/

p2Vec3 p2Vec2::to3() const
{
	return {x, y, 0.0f};
}

p2Vec3::p2Vec3()
{
	x = 1.0f;
	y = 1.0f;
	z = 1.0f;
}

p2Vec3::p2Vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

p2Vec3 p2Vec3::operator+(const p2Vec3& v)const
{
	return {x + v.x, y + v.y, z + v.z};
}

p2Vec3& p2Vec3::operator+=(const p2Vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

p2Vec3 p2Vec3::operator-(const p2Vec3& v) const
{
	return {x - v.x, y - v.y, z- v.z};
}

p2Vec3& p2Vec3::operator-=(const p2Vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

p2Vec3& p2Vec3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= z;
	return (*this);
}

p2Vec3 p2Vec3::operator/(float f) const
{
	return {x / f, y / f, z/f};
}

p2Vec3 p2Vec3::operator*(float f) const
{
	return {x*f, y*f, z*f};
}

float p2Vec3::Dot(p2Vec3 v1, p2Vec3 v2)
{
	//TODO
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

p2Vec3 p2Vec3::Cross(p2Vec3 v1, p2Vec3 v2)
{
	//TODO
	return {v1.y * v2.z - v2.y * v1.z, v1.z * v2.x - v2.z * v1.x, v1.x * v2.y - v2.x * v1.y};
}

p2Vec3 p2Vec3::Rotate(float angle) const
{
	return p2Vec3();
}

p2Vec3 p2Vec3::Lerp(const p2Vec3& v1, const p2Vec3& v2, float t)
{
	const p2Vec3 origin = v1;
	const p2Vec3 target = v2;

	return origin + target * t;
}

float p2Vec3::AngleBetween(const p2Vec3& v1, const p2Vec3& v2)
{
	return acos(Dot(v1, v2) / (v1.GetMagnitude() * v2.GetMagnitude()));
}

float p2Vec3::GetMagnitude() const
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

p2Vec3 p2Vec3::Normalized() const
{
	const float magnitude = GetMagnitude();
	return {x / magnitude, y / magnitude, z / magnitude};
}

void p2Vec3::NormalizeSelf()
{
	const float magnitude = GetMagnitude();
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
}
