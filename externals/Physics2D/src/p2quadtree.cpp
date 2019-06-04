#include <utility>
#include <p2quadtree.h>
#include <iostream>

p2QuadTree::p2QuadTree(const int nodeLevel, p2Aabb bounds)
{
	// Set base values
	m_NodeLevel = nodeLevel;
	m_Bounds = std::move(bounds);
}

void p2QuadTree::Clear()
{
	m_Objects.clear();
	m_Children.clear();
	m_Children.reserve(MAX_CHILD_TREE_NMB);
}

void p2QuadTree::Split()
{
	if (m_NodeLevel > MAX_LEVELS) return;

	// Set the current position
	auto currentPosition = m_Bounds.bottomLeft;

	// Define the size of the child sides depending on the amount of child tree number
	const auto childSize = (m_Bounds.topRight - currentPosition) / sqrt(MAX_CHILD_TREE_NMB);

	for (auto x = 0; x < sqrt(MAX_CHILD_TREE_NMB); ++x)
	{
		for (auto y = 0; y < sqrt(MAX_CHILD_TREE_NMB); ++y)
		{
			p2Aabb childAabb;

			currentPosition = m_Bounds.bottomLeft + p2Vec2(childSize.x * x, childSize.y * y);

			childAabb.bottomLeft = currentPosition;
			childAabb.topRight = currentPosition + childSize;

			// Add the node to the child array
			m_Children.push_back(new p2QuadTree(m_NodeLevel + 1, childAabb));
		}
	}

	for (auto& obj : m_Objects)
	{
		for (auto* child : m_Children)
		{
			if (p2Aabb::DoOverlapWith(child->m_Bounds, obj->GetAabb())) child->Insert(obj);
		}
	}

	m_Objects.clear();
}

int p2QuadTree::GetIndex(p2Body * rect)
{
	for (auto& body : m_Objects)
	{
		if (body == rect) return m_NodeLevel;
	}

	for (auto& child : m_Children)
	{
		child->GetIndex(rect);
	}

	return 0;
}

void p2QuadTree::Insert(p2Body* obj)
{
	if (!m_Children.empty())
	{
		for (auto* child : m_Children)
		{
			if (p2Aabb::DoOverlapWith(child->m_Bounds, obj->GetAabb())) child->Insert(obj);
		}
	}
	else
	{
		m_Objects.push_back(obj);
	}

	if (m_Objects.size() > MAX_OBJECTS) {
		Split();
	}
}

std::vector<p2Body*> p2QuadTree::Retrieve(const p2Body* rect)
{
	std::vector<p2Body*> returnValue;
	for (auto& body : m_Objects)
	{
		if (body == rect) returnValue = { std::begin(m_Objects), std::end(m_Objects) };
	}

	for (auto& child : m_Children)
	{
		const auto retrieve = child->Retrieve(rect);
		if (!retrieve.empty())
		{
			returnValue = retrieve;
			break;
		}
	}

	return returnValue;
}

void p2QuadTree::SetBounds(p2Aabb bounds)
{
	m_Bounds = std::move(bounds);
}

p2Aabb p2QuadTree::GetBounds() const
{
	return m_Bounds;
}

std::vector<p2QuadTree*> p2QuadTree::GetChildren() const
{
	return m_Children;
}

std::list<p2Body*> p2QuadTree::GetObjects() const
{
	return m_Objects;
}