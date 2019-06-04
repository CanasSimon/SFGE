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
#include <engine/engine.h>
#include <engine/config.h>
#include <engine/scene.h>
#include <gtest/gtest.h>
#include <audio/audio.h>
#include <utility/json_utility.h>
#include <SFML/Audio.hpp>
#include "graphics/shape2d.h"
#include "physics/collider2d.h"

TEST(Final, TestVector)
{
	sfge::Engine engine;
	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();
	engine.Init(std::move(initConfig));
	json sceneJson = {{"name", "Test Vector"}};
	json systemJson = {{"systemClassName", "VectorTest"}};
	sceneJson["systems"] = json::array({systemJson});
	auto* sceneManager = engine.GetSceneManager();
	sceneManager->LoadSceneFromJson(sceneJson);

	engine.Start();
}

TEST(Final, TestAabb)
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
	entities[1]["components"] = { transformJson2, rectShape, rigidbody, rectColliderShape };

	sceneJson["entities"] = entities;
	sceneJson["systems"] = json::array({
			{ {"systemClassName", "Debug"} },
			{ {"systemClassName", "BodyTest"} }
		});

	sceneManager->LoadSceneFromJson(sceneJson);
	engine.Start();
}

TEST(Final, TestShapeContactCpp)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->gravity = p2Vec2(0.0f, 0.0f);
	engine.Init(std::move(config));

	auto* sceneManager = engine.GetSceneManager();

	json sceneJson;
	sceneJson["name"] = "Contacts";

	const auto entitiesNmb = 256;
	json entities[entitiesNmb];

	json shapes[] =
	{
		{
			{"name","Rect Shape Component"},
			{"type",sfge::ComponentType::SHAPE2D},
			{"shape_type", sfge::ShapeType::RECTANGLE},
			{"size",{20,20}}
		},
		{
			{"name","Circle Shape Component"},
			{"type",sfge::ComponentType::SHAPE2D},
			{"shape_type", sfge::ShapeType::CIRCLE},
			{"radius",10}
		}
	};
	json colliderShapes[] =
	{
		{
			{"name","Rect Collider"},
			{"type", sfge::ComponentType::COLLIDER2D},
			{"collider_type",sfge::ColliderType::BOX},
			{"size",{20,20}},
			{"sensor",true}
		},
		{
			{"name","Circle Collider"},
			{"type", sfge::ComponentType::COLLIDER2D},
			{"collider_type",sfge::ColliderType::CIRCLE},
			{"radius",10},
			{"sensor",true}
		}
	};

	for (auto& entityJson : entities)
	{
		json transformJson =
		{
			{"position",{ rand() % 800,rand() % 600 }},
			{"type", sfge::ComponentType::TRANSFORM2D}
		};

		json rigidbody =
		{
			{"name", "Rigidbody"},
			{"type", sfge::ComponentType::BODY2D},
			{"body_type",  p2BodyType::DYNAMIC},
			{"max_colliders",  1},
			{"velocity", {rand() % 200, rand() % 200}}
		};

		const auto randShapeIndex = rand() % 2;
		entityJson["components"] = { transformJson, shapes[randShapeIndex], rigidbody, colliderShapes[randShapeIndex] };
	}

	sceneJson["entities"] = entities;
	sceneJson["systems"] = json::array({
			{
				{"systemClassName", "Debug"}
			},
			{
				{"script_path", "scripts/contact_debug_system.py"}
			}
		}
	);

	sceneManager->LoadSceneFromJson(sceneJson);
	engine.Start();
}

TEST(Final, TestPlanetSystemCpp)
{
	sfge::Engine engine;
	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();
	initConfig->gravity = p2Vec2();
	initConfig->devMode = false;
	initConfig->maxFramerate = 0;
	engine.Init(std::move(initConfig));
	json sceneJson = { { "name", "Test Planet Component" } };
	json systemJson = { { "systemClassName", "PlanetSystem" } };

	sceneJson["systems"] = json::array({ systemJson });
	auto* sceneManager = engine.GetSceneManager();
	sceneManager->LoadSceneFromJson(sceneJson);

	engine.Start();
}
