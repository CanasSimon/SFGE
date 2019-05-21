#include "..\include\p2quadtree.h"

p2QuadTree::p2QuadTree(int nodeLevel, p2AABB bounds)
{
	m_Children.resize(MAX_CHILD_TREE_NMB);
	// Set base values
	m_NodeLevel = nodeLevel;
	m_Bounds = bounds;
}

p2QuadTree::~p2QuadTree()
{
}

void p2QuadTree::Clear()
{
	for (p2QuadTree* quad : m_Children)
	{
		quad->Clear();

		for (p2Body* body : quad->m_Objects)
		{
			m_Objects.push_back(body);
		}

		delete(quad);
	}
}

void p2QuadTree::Split()
{
	if (m_NodeLevel > MAX_LEVELS) return;

	// Define the corners of the current node
	const p2Vec2 extends = m_Bounds.GetExtends();

	// Set the current position
	p2Vec2 currentPosition = m_Bounds.bottomLeft;

	// Define the size of the child sides depending on the amount of child tree number
	const p2Vec2 childSideSize = (m_Bounds.topRight - currentPosition) / sqrt(MAX_CHILD_TREE_NMB);

	for (int i = 0; i < MAX_CHILD_TREE_NMB; i++)
	{
		p2AABB childAABB;

		childAABB.bottomLeft = currentPosition;
		childAABB.topLeft = currentPosition + p2Vec2(0, childSideSize.y);
		childAABB.topRight = currentPosition + childSideSize;
		childAABB.bottomRight = currentPosition + p2Vec2(childSideSize.x, 0);

		// Check if it needs to jump on the y axis
		if (currentPosition.x + childSideSize.x >= extends.x)
			currentPosition = { m_Bounds.bottomLeft.x, currentPosition.y + childSideSize.y };
		else
			currentPosition.x = currentPosition.x + childSideSize.x;

		// Add the node to the child array
		m_Children[i] = new p2QuadTree(m_NodeLevel + 1, childAABB);
	}
}

int p2QuadTree::GetIndex(p2Body * rect)
{
	for (int i = 0; i < MAX_CHILD_TREE_NMB; i++)
	{
		for(p2Body* body : m_Children[i]->m_Objects)
		{
			if (body == rect)
				return i;
		}
	}

	return 0;
}

void p2QuadTree::Insert(p2Body * obj)
{
	m_Objects.push_back(obj);
}

void p2QuadTree::Retrieve()
{
}

void p2QuadTree::SetBounds(p2AABB bounds)
{
	m_Bounds = bounds;
}

p2AABB p2QuadTree::GetBounds() const
{
	return m_Bounds;
}

std::vector<p2QuadTree*> p2QuadTree::GetChildren() const
{
	return m_Children;
}
