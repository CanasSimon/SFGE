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

#include <p2matrix.h>

p2Mat22::p2Mat22() = default;

p2Mat22::p2Mat22(p2Vec2 r1, p2Vec2 r2)
{
	rows[0] = r1;
	rows[1] = r2;
}

p2Mat22 p2Mat22::operator+(p2Mat22 m1) const
{
	const p2Vec2 r1 = p2Vec2(rows[0].x + m1.rows[0].x, rows[0].y + m1.rows[0].y);
	const p2Vec2 r2 = p2Vec2(rows[1].x + m1.rows[1].x, rows[1].y + m1.rows[1].y);
	return {r1, r2};
}

p2Mat22 p2Mat22::operator-(p2Mat22 m1) const
{
	const p2Vec2 r1 = p2Vec2(rows[0].x - m1.rows[0].x, rows[0].y - m1.rows[0].y);
	const p2Vec2 r2 = p2Vec2(rows[1].x - m1.rows[1].x, rows[1].y - m1.rows[1].y);
	return {r1, r2};
}

p2Mat22 p2Mat22::operator*(p2Mat22 m1) const
{
	const p2Vec2 r1 = p2Vec2(rows[0].x, rows[0].y);
	const p2Vec2 r2 = p2Vec2(rows[1].x, rows[1].y);

	const p2Vec2 c1 = p2Vec2(m1.rows[0].x, m1.rows[0].y);
	const p2Vec2 c2 = p2Vec2(m1.rows[1].x, m1.rows[1].y);
	
	const p2Vec2 result1 = p2Vec2(p2Vec2::Dot(r1, c1), p2Vec2::Dot(r1, c2));
	const p2Vec2 result2 = p2Vec2(p2Vec2::Dot(r2, c1), p2Vec2::Dot(r2, c2));

	return {result1, result2};
}

p2Vec2 p2Mat22::operator*(p2Vec2 v) const
{
	const float x = (rows[0].x * v.x) + (rows[0].y * v.y);
	const float y = (rows[1].x * v.x) + (rows[1].y * v.y);
	return {x, y};
}

p2Mat22 p2Mat22::operator*(float f) const
{
	const p2Vec2 r1 = p2Vec2(rows[0].x * f, rows[0].y * f);
	const p2Vec2 r2 = p2Vec2(rows[1].x * f, rows[1].y * f);
	return {r1, r2};
}

p2Mat22 p2Mat22::operator/(float f) const
{
	const p2Vec2 r1 = p2Vec2(rows[0].x / f, rows[0].y / f);
	const p2Vec2 r2 = p2Vec2(rows[1].x / f, rows[1].y / f);
	return {r1, r2};
}

p2Mat22 p2Mat22::Invert() const
{
	const p2Vec2 r1 = p2Vec2(rows[1].y, -rows[0].y);
	const p2Vec2 r2 = p2Vec2(-rows[1].x, rows[0].x);

	return p2Mat22(r1, r2) * (1 / GetDeterminant());
}

float p2Mat22::GetDeterminant() const
{
	return (rows[0].x * rows[1].y) - (rows[0].y * rows[1].x);
}

p2Mat33::p2Mat33() = default;

p2Mat33::p2Mat33(p2Vec3 r1, p2Vec3 r2, p2Vec3 r3)
{
	rows[0] = r1;
	rows[1] = r2;
	rows[2] = r3;
}

p2Mat33 p2Mat33::operator+(p2Mat33 m1) const
{
	const p2Vec3 r1 = p2Vec3(rows[0].x + m1.rows[0].x, rows[0].y + m1.rows[0].y, rows[0].z + m1.rows[0].z);
	const p2Vec3 r2 = p2Vec3(rows[1].x + m1.rows[1].x, rows[1].y + m1.rows[1].y, rows[1].z + m1.rows[1].z);
	const p2Vec3 r3 = p2Vec3(rows[2].x + m1.rows[2].x, rows[2].y + m1.rows[2].y, rows[2].z + m1.rows[2].z);
	return {r1, r2, r3};
}

p2Mat33 p2Mat33::operator-(p2Mat33 m1) const
{
	const p2Vec3 r1 = p2Vec3(rows[0].x - m1.rows[0].x, rows[0].y - m1.rows[0].y, rows[0].z - m1.rows[0].z);
	const p2Vec3 r2 = p2Vec3(rows[1].x - m1.rows[1].x, rows[1].y - m1.rows[1].y, rows[1].z - m1.rows[1].z);
	const p2Vec3 r3 = p2Vec3(rows[2].x - m1.rows[2].x, rows[2].y - m1.rows[2].y, rows[2].z - m1.rows[2].z);
	return {r1, r2, r3};
}

p2Mat33 p2Mat33::operator*(p2Mat33 m1) const
{
	const p2Vec3 r1 = p2Vec3(rows[0].x, rows[0].y, rows[0].z);
	const p2Vec3 r2 = p2Vec3(rows[1].x, rows[1].y, rows[1].z);
	const p2Vec3 r3 = p2Vec3(rows[2].x, rows[2].y, rows[2].z);
	
	const p2Vec3 c1 = p2Vec3(m1.rows[0].x, m1.rows[0].y, m1.rows[0].z);
	const p2Vec3 c2 = p2Vec3(m1.rows[1].x, m1.rows[1].y, m1.rows[1].z);
	const p2Vec3 c3 = p2Vec3(m1.rows[2].x, m1.rows[2].y, m1.rows[2].z);
	
	const p2Vec3 result1 = p2Vec3(p2Vec3::Dot(r1, c1), p2Vec3::Dot(r1, c2), p2Vec3::Dot(r1, c3));
	const p2Vec3 result2 = p2Vec3(p2Vec3::Dot(r2, c1), p2Vec3::Dot(r2, c2), p2Vec3::Dot(r2, c3));
	const p2Vec3 result3 = p2Vec3(p2Vec3::Dot(r3, c1), p2Vec3::Dot(r3, c2), p2Vec3::Dot(r3, c3));

	return {result1, result2, result3};
}

p2Vec3 p2Mat33::operator*(p2Vec3 v) const
{
	const float x = (rows[0].x * v.x) + (rows[0].y * v.y) + (rows[0].z * v.z);
	const float y = (rows[1].x * v.x) + (rows[1].y * v.y) + (rows[1].z * v.z);
	const float z = (rows[2].x * v.x) + (rows[2].y * v.y) + (rows[2].z * v.z);
	return {x, y, z};
}

p2Mat33 p2Mat33::operator*(float f) const
{
	const p2Vec3 r1 = p2Vec3(rows[0].x * f, rows[0].y * f, rows[0].z * f);
	const p2Vec3 r2 = p2Vec3(rows[1].x * f, rows[1].y * f, rows[1].z * f);
	const p2Vec3 r3 = p2Vec3(rows[2].x * f, rows[2].y * f, rows[2].z * f);
	return {r1, r2, r3};
}

p2Mat33 p2Mat33::operator/(float f) const
{
	const p2Vec3 r1 = p2Vec3(rows[0].x / f, rows[0].y / f, rows[0].z / f);
	const p2Vec3 r2 = p2Vec3(rows[1].x / f, rows[1].y / f, rows[1].z / f);
	const p2Vec3 r3 = p2Vec3(rows[2].x / f, rows[2].y / f, rows[2].z / f);
	return {r1, r2, r3};
}

p2Mat33 p2Mat33::Invert() const
{
	// Calculate the rows of the minors matrix
	p2Vec3 minorR1 = p2Vec3((rows[1].y * rows[2].z) - (rows[1].z * rows[2].y), (rows[1].x * rows[2].z) - (rows[1].z * rows[2].x), (rows[1].x * rows[2].y) - (rows[1].y * rows[2].x));
	p2Vec3 minorR2 = p2Vec3((rows[0].y * rows[2].z) - (rows[0].z * rows[2].y), (rows[0].x * rows[2].z) - (rows[0].z * rows[2].x), (rows[0].x * rows[2].y) - (rows[0].y * rows[2].x));
	p2Vec3 minorR3 = p2Vec3((rows[0].y * rows[1].z) - (rows[0].z * rows[1].y), (rows[0].x * rows[1].z) - (rows[0].z * rows[1].x), (rows[0].x * rows[1].y) - (rows[0].y * rows[1].x));

	// Modify rows of the minors matrix to translate it to the cofactors matrix
	minorR1.y = -minorR1.y;
	minorR2.x = -minorR2.x;
	minorR2.z = -minorR2.z;
	minorR3.y = -minorR3.y;

	// Create the 3x3 matrix
	auto result = p2Mat33(minorR1, minorR2, minorR3);

	// Transpose the matrix
	result.rows[0].y = minorR2.x;
	result.rows[0].z = minorR3.x;
	result.rows[1].x = minorR1.y;
	result.rows[1].z = minorR3.y;
	result.rows[2].x = minorR1.z;
	result.rows[2].y = minorR2.z;

	return result * (1 / GetDeterminant());
}

float p2Mat33::GetDeterminant() const
{
	const float result1 = rows[0].x * ((rows[1].y * rows[2].z) - (rows[1].z * rows[2].y));
	const float result2 = rows[0].y * ((rows[1].x * rows[2].z) - (rows[1].z * rows[2].x));
	const float result3 = rows[0].z * ((rows[1].x * rows[2].y) - (rows[1].y * rows[2].x));
	return result1 - result2 + result3;
}
