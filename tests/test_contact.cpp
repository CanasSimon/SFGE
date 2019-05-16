#include <engine/engine.h>
#include <engine/config.h>
#include <engine/scene.h>
#include <utility/json_utility.h>
#include <gtest/gtest.h>
#include "p2body.h"
#include "physics/collider2d.h"
#include "graphics/shape2d.h"

TEST(Physics, TestSAT)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->gravity = p2Vec2(0.0f, 0.0f);
	engine.Init(std::move(config));

	auto* sceneManager = engine.GetSceneManager();

	json sceneJson;
	sceneJson["name"] = "Contacts";

	const int entitiesNmb = 2;
	json entities[entitiesNmb];

	json shape =
	{
		{"name", "Rect Shape Component"},
		{"type", sfge::ComponentType::SHAPE2D},
		{"shape_type", sfge::ShapeType::RECTANGLE},
		{"size", {100, 100}}
	};

	json colliderShape1 =
	{
		{"name", "Rect Collider"},
		{"type", sfge::ComponentType::COLLIDER2D},
		{"collider_type", sfge::ColliderType::BOX},
		{"size", {100, 100}},
		{"sensor", true}
	};

	json colliderShape2 =
	{
		{"name", "Rect Collider"},
		{"type", sfge::ComponentType::COLLIDER2D},
		{"collider_type", sfge::ColliderType::BOX},
		{"size", {100, 100}},
		{"offset", {10, 10}},
		{"sensor", true}
	};

	for (int i = 0; i < entitiesNmb; i++)
	{
		json& entityJson = entities[i];

		json transformJson =
		{
			{"position", {400 + std::rand() % (800 - 400 + 1), 200 + std::rand() % (600 - 200 + 1)}},
			{"type", sfge::ComponentType::TRANSFORM2D}
		};

		json rigidbody =
		{
			{"name", "Rigidbody"},
			{"type", sfge::ComponentType::BODY2D},
			{"body_type",  p2BodyType::DYNAMIC},
			{"max_colliders",  1}
		};

		entityJson["components"] = { transformJson, shape, rigidbody, colliderShape1, colliderShape2 };
	}

	sceneJson["entities"] = entities;
	sceneJson["systems"] = json::array({
			{
				{"systemClassName", "DrawAABB"}
			},
			{
				{"systemClassName", "BodyTest"}
			},
			{
				{"systemClassName", "DrawSAT"}
			}
		}
	);

	sceneManager->LoadSceneFromJson(sceneJson);
	engine.Start();
}