#include "..\include\p2collider.h"

p2Collider::p2Collider()
{
	
}

p2Collider::p2Collider(p2ColliderDef colDef)
{
	colliderDefinition = colDef;
}

bool p2Collider::IsSensor() const
{
	return colliderDefinition.isSensor;
}

void * p2Collider::GetUserData() const
{
	return userData;
}

p2Shape* p2Collider::GetShape() const
{
	return colliderDefinition.shape;
}

void p2Collider::SetUserData(void* colliderData)
{
	userData = colliderData;
}
