#include "..\include\p2quadtree.h"

p2QuadTree::p2QuadTree(int nodeLevel, p2AABB bounds)
{
	// Set base values
	m_NodeLevel = nodeLevel;
	m_Bounds = bounds;
}

p2QuadTree::~p2QuadTree()
{
}

void p2QuadTree::Clear()
{
	for (p2QuadTree* quad : nodes)
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
	if (m_NodeLevel > MAX_LEVELS)
		return;

	// Define the corners of the current node
	const p2Vec2 extends = m_Bounds.GetExtends();

	// Set the current position
	p2Vec2 currentPosition = p2Vec2(m_Bounds.left.x, m_Bounds.bottom.y);

	// Define the size of the child sides depending on the amount of child tree number
	const float childSideSize = (m_Bounds.top.y - currentPosition.y) / sqrt(CHILD_TREE_NMB);

	for (int i = 0; i < CHILD_TREE_NMB; i++)
	{
		p2AABB childAABB;

		childAABB.bottom = p2Vec2(0, currentPosition.y);
		childAABB.left = p2Vec2(currentPosition.x, 0);

		childAABB.top = p2Vec2(0, currentPosition.y + childSideSize);
		childAABB.right = p2Vec2(currentPosition.x + childSideSize, 0);

		// Check if it needs to jump on the y axis
		if (currentPosition.x + childSideSize >= extends.x)
			currentPosition = { m_Bounds.bottom.x, currentPosition.y + childSideSize };
		else
			currentPosition.x = currentPosition.x + childSideSize;

		// Add the node to the child array
		nodes[i] = new p2QuadTree(m_NodeLevel + 1, childAABB);
	}
}

int p2QuadTree::GetIndex(p2Body * rect)
{
	for (int i = 0; i < CHILD_TREE_NMB; i++)
	{
		for(p2Body* body : nodes[i]->m_Objects)
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
