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

#include <p2shape.h>

p2CircleShape::p2CircleShape(float radius) : p2Shape()
{
	type = CIRCLE;
	m_Radius = radius;
}

void p2CircleShape::SetRadius(float radius)
{
	m_Radius = radius;
}

float p2CircleShape::GetRadius() const
{
	return m_Radius;
}

p2RectShape::p2RectShape() : p2Shape()
{
}

p2RectShape::p2RectShape(p2Vec2 size) : p2Shape()
{
	type = RECT;
	m_Size = size;
}

void p2RectShape::SetSize(p2Vec2 size)
{
	m_Size = size;
}

p2Vec2 p2RectShape::GetSize() const
{
	return m_Size;
}

p2PolygonShape::p2PolygonShape() : p2Shape()
{
}

p2PolygonShape::p2PolygonShape(std::vector<p2Vec2> points) : p2Shape()
{
	type = POLY;
	m_Points = points;
}

void p2PolygonShape::SetPoints(std::vector<p2Vec2> points)
{
	m_Points = points;
}

std::vector<p2Vec2> p2PolygonShape::GetPoints() const
{
	return m_Points;
}
