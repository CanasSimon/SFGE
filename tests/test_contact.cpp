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

	const auto entitiesNmb = 2;
	json entities[entitiesNmb];

	json rectShape = {
		{"name", "Rect Shape Component"},
		{"type", sfge::ComponentType::SHAPE2D},
		{"shape_type", sfge::ShapeType::RECTANGLE},
		{"size", {200, 100}}
	};
	json circleShape = {
		{"name", "Rect Shape Component"},
		{"type", sfge::ComponentType::SHAPE2D},
		{"shape_type", sfge::ShapeType::CIRCLE},
		{"radius", 50}
	};
	json rectColliderShape = {
		{"name", "Rect Collider"},
		{"type", sfge::ComponentType::COLLIDER2D},
		{"collider_type", sfge::ColliderType::BOX},
		{"size", {200, 100}},
		{"sensor", true}
	};
	json circleColliderShape = {
		{"name", "Rect Collider"},
		{"type", sfge::ComponentType::COLLIDER2D},
		{"collider_type", sfge::ColliderType::CIRCLE},
		{"radius", 50},
		{"sensor", true}
	};

	json transformJson1 = {
		{"position", {400, 200}},
		{"type", sfge::ComponentType::TRANSFORM2D}
	};
	json transformJson2 = {
		{"position", {700, 200}},
		{"type", sfge::ComponentType::TRANSFORM2D}
	};
	json rigidbody = {
		{"name", "Rigidbody"},
		{"type", sfge::ComponentType::BODY2D},
		{"body_type", p2BodyType::DYNAMIC},
		{"max_colliders", 2}
	};
	entities[0]["components"] = { transformJson1, circleShape, rigidbody, circleColliderShape };
	entities[1]["components"] = { transformJson2, circleShape, rigidbody, circleColliderShape };

	sceneJson["entities"] = entities;
	sceneJson["systems"] = json::array({
			{
				{"systemClassName", "Debug"}
			},
			{
				{"systemClassName", "BodyTest"}
			}
		}
	);

	sceneManager->LoadSceneFromJson(sceneJson);
	engine.Start();
}
