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

#ifndef SFGE_P2_VECTOR_H
#define SFGE_P2_VECTOR_H

struct p2Vec3;

struct p2Vec2
{
	p2Vec2();

	p2Vec2(float x, float y);
	bool operator==(const p2Vec2 &rhs) const;
	bool operator!=(const p2Vec2 &rhs) const;
	bool operator>(const p2Vec2& rhs) const;
	bool operator>=(const p2Vec2& rhs) const;
	bool operator<(const p2Vec2& rhs) const;
	bool operator<=(const p2Vec2& rhs) const;

	p2Vec2 operator+(const p2Vec2& v)const;
	p2Vec2 operator+(const float& rhs) const;
	p2Vec2& operator+=(const p2Vec2& v);
	p2Vec2 operator-(const p2Vec2& v) const;
	p2Vec2 operator-() const;
	p2Vec2& operator-=(const p2Vec2& v);
	p2Vec2& operator*=(float f);
	p2Vec2 operator /(float f) const;
	p2Vec2 operator /(const p2Vec2& v) const;
	p2Vec2 operator*(float f) const;
	p2Vec2 operator *(const p2Vec2& v) const;

	static float Dot(p2Vec2 v1, p2Vec2 v2);
	static float Cross(p2Vec2 v1, p2Vec2 v2);
	float GetMagnitude() const;
	float GetDistance(const p2Vec2& v1) const;
	p2Vec2 Normalized() const;
	void NormalizeSelf();
	p2Vec2 Absolute() const;

	p2Vec2 Rotate(float angle) const;
	static p2Vec2 Lerp(const p2Vec2& v1, const p2Vec2& v2, float t);
	static float AngleBetween(const p2Vec2& v1, const p2Vec2& v2);
	static p2Vec2 GetVectorFrom(const p2Vec2& v1, const p2Vec2& v2);
	void ProjectOn(const p2Vec2 & v1);
	p2Vec2 GetProjectionOn(const p2Vec2& v1) const;
	p2Vec2 GetNormal() const;
	p2Vec2 GetReflection(const p2Vec2& n) const;

	bool OnSegment(const p2Vec2& v1, const p2Vec2& v2) const;
	int LineSide(const p2Vec2& v1, const p2Vec2& v2) const;
	static bool DoOverlap(const p2Vec2& v1, const p2Vec2& v2, const p2Vec2& w1, const p2Vec2& w2);

	p2Vec3 To3() const;

	float x = 0.0f;
	float y = 0.0f;

	static const p2Vec2 ZERO;
	static const p2Vec2 ONE;
	static const p2Vec2 UP;
	static const p2Vec2 DOWN;
	static const p2Vec2 RIGHT;
	static const p2Vec2 LEFT;
};

struct p2Vec3
{
	p2Vec3();
	p2Vec3(float x, float y, float z);


	p2Vec3 operator+(const p2Vec3& v) const;
	p2Vec3& operator+=(const p2Vec3 & v);
	p2Vec3 operator-(const p2Vec3& v) const;
	p2Vec3& operator-=(const p2Vec3& v);
	p2Vec3& operator*=(float f);
	p2Vec3 operator /(float f) const;
	p2Vec3 operator *(float f) const;
	/**
	* \brief Dot product of two vectors
	*/
	static float Dot(p2Vec3 v1, p2Vec3 v2);
	/**
	* \brief Cross product of two vectors
	*/
	static p2Vec3 Cross(p2Vec3 v1, p2Vec3 v2);
	static p2Vec3 Rotate(float angle);
	static p2Vec3 Lerp(const p2Vec3& v1, const p2Vec3& v2, float t);
	static float AngleBetween(const p2Vec3& v1, const p2Vec3& v2);
	/**
	* \brief Calculate the magnitude of the p2Vec2
	*/
	float GetMagnitude() const;
	/**
	* \brief Calculate a normalized version of the p2Vec2
	*/
	p2Vec3 Normalized() const;
	/**
	* \brief Normalize the p2Vec2
	*/
	void NormalizeSelf();
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

#endif