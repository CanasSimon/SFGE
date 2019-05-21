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
#include <imgui.h>
#include <physics/collider2d.h>
#include <engine/globals.h>
#include <engine/component.h>
#include <physics/physics2d.h>
#include <engine/engine.h>
namespace sfge
{
void editor::ColliderInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Collider");
	if (data != nullptr)
	{
		if (data->fixture != nullptr)
		{
			switch (data->fixture->GetType())
			{
			case p2ColliderType::CIRCLE:
				ImGui::LabelText("Type", "Circle");
				break;
			case p2ColliderType::RECT:
				ImGui::LabelText("Type", "Rectangle");
				break;
			case p2ColliderType::POLY:
				ImGui::LabelText("Type", "Polygon");
				break;
			default:
				ImGui::LabelText("Type", "None");
				break;
			}
		}
	}
}


void ColliderManager::OnEngineInit()
{
	MultipleComponentManager::OnEngineInit();
	m_BodyManager = m_Engine.GetPhysicsManager()->GetBodyManager();
}

void ColliderManager::CreateComponent(json& componentJson, Entity entity)
{
	Log::GetInstance()->Msg("Create component Collider");
	if (m_EntityManager->HasComponent(entity, ComponentType::BODY2D))
	{
		auto & body = m_BodyManager->GetComponentRef(entity);

		p2ColliderDef fixtureDef;

		if (CheckJsonExists(componentJson, "sensor"))
		{
			fixtureDef.isSensor = componentJson["sensor"];
		}

		std::unique_ptr<p2Shape> shape = nullptr;

		if (CheckJsonExists(componentJson, "collider_type"))
		{
			auto colliderType = static_cast<ColliderType>(componentJson["collider_type"]);
			switch (colliderType)
			{
			case ColliderType::CIRCLE:
				{
					auto circleShape = std::make_unique<p2CircleShape>();
					if (CheckJsonNumber(componentJson, "radius"))
					{
						const auto radius = pixel2meter(static_cast<float>(componentJson["radius"]));
						{
							std::ostringstream oss;
							oss << "Circle physics radius: " << radius;
							Log::GetInstance()->Msg(oss.str());
						}
						circleShape->SetRadius(radius);
					}
					shape = std::move(circleShape);
				}
				break;
			case ColliderType::BOX:
				{
					auto boxShape = std::make_unique<p2RectShape>();
					if (CheckJsonExists(componentJson, "size"))
					{
						const auto size = pixel2meter(GetVectorFromJson(componentJson, "size"));
						{
							std::ostringstream oss;
							oss << "Box physics size: " << size.x << ", " << size.y;
							Log::GetInstance()->Msg(oss.str());
						}
						boxShape->SetSize(size / 2.0f);
					}
					shape = std::move(boxShape);
				}
				break;
			/*case ColliderType::POLYGON:
				{
					auto polyShape = std::make_unique<p2PolygonShape>();
					if (CheckJsonExists(componentJson, "points"))
					{
						const auto points = componentJson["points"];
						{
							std::ostringstream oss;
							oss << "Polygon points count: " << points.size();
							Log::GetInstance()->Msg(oss.str());
						}
						polyShape->SetPoints(points);
					}
					shape = std::move(polyShape);
				}
				break;*/
			default:
				{
					system("pause");
					std::ostringstream oss;
					oss << "[Error] Collider of type: " << static_cast<int>(colliderType) <<
						" could not be loaded from json: " << componentJson;
					Log::GetInstance()->Error(oss.str());
				}
				break;
			}

			fixtureDef.colliderType = static_cast<p2ColliderType>(componentJson["collider_type"]);
		}
		if(CheckJsonNumber(componentJson, "bouncing"))
		{
			fixtureDef.restitution = componentJson["bouncing"];
		}
		if (CheckJsonExists(componentJson, "offset"))
		{
			fixtureDef.offset = pixel2meter(GetVectorFromJson(componentJson, "offset"));
		}
		if (shape != nullptr)
		{
			fixtureDef.shape = shape.get();

			auto index = GetFreeComponentIndex();
			if(index != -1)
			{
				fixtureDef.position = body.GetBody()->GetPosition();
				auto* fixture = body.GetBody()->CreateCollider(&fixtureDef);


				ColliderData& colliderData = m_Components[index];
				colliderData.entity = entity;
				colliderData.fixture = fixture;
				colliderData.body = body.GetBody();
				m_ComponentsInfo[index].data = &colliderData;
				m_ComponentsInfo[index].SetEntity(entity);
				fixture->SetUserData(&colliderData);
			}
		}
	}
}
int ColliderManager::GetFreeComponentIndex()
{
	for(auto i = 0u; i < m_Components.size();i++)
	{
		if(m_Components[i].entity == INVALID_ENTITY)
			return i;
	}
	return -1;
}
ColliderData *ColliderManager::AddComponent(Entity entity)
{
	(void) entity;
	return nullptr;
}
void ColliderManager::DestroyComponent(Entity entity)
{
	(void) entity;
}
ColliderData *ColliderManager::GetComponentPtr(Entity entity)
{
	(void)entity;
	return nullptr;
}
}
