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
#include <engine/scene.h>
#include <gtest/gtest.h>
#include "graphics/shape2d.h"
#include "physics/collider2d.h"

TEST(Physics, TestBallFallingToGround)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->devMode = false;
	config->gravity = p2Vec2(0, 0);
	config->maxFramerate = 0;
	engine.Init(std::move(config));

	auto* sceneManager = engine.GetSceneManager();

	json sceneJson;
	sceneJson["name"] = "Ball Falling To Ground";

	json entityBody1;
	entityBody1["name"] = "Body1";

	json transformJson1 = {
		{"type", sfge::ComponentType::TRANSFORM2D},
		{"position", {700, 300}},
		{"scale", {1.0, 1.0}}
	};
	json circleShapeJson = {
		{"name", "Circle Shape Component"},
		{"type", sfge::ComponentType::SHAPE2D},
		{"shape_type", sfge::ShapeType::CIRCLE},
		{"radius", 50}
	};
	json rigidBodyJson1 = {
		{"name", "Rigidbody"},
		{"type", sfge::ComponentType::BODY2D},
		{"body_type", p2BodyType::DYNAMIC},
	};
	json circleColliderJson = {
		{"name", "Circle Collider"},
		{"type", sfge::ComponentType::COLLIDER2D},
		{"collider_type", sfge::ColliderType::CIRCLE},
		{"radius", 50},
		{"restitution", 1},
		{"bounce", 1},
		{"friction", 1},
		{"sensor", false}
	};

	json entityBody2;
	entityBody2["name"] = "Ground";

	json transformJson2 = {
		{"type", sfge::ComponentType::TRANSFORM2D},
		{"position", {700, 500}},
		{"angle", 0},
		{"scale", {1.0, 1.0}}
	};
	json rectShapeJson = {
		{"name", "Rect Shape Component"},
		{"type", sfge::ComponentType::SHAPE2D},
		{"shape_type", sfge::ShapeType::RECTANGLE},
		{"size", {100, 100}}
	};
	json rigidBodyJson2 = {
		{"name", "Rigidbody"},
		{"type", sfge::ComponentType::BODY2D},
		{"body_type", p2BodyType::STATIC},
	};
	json rectColliderJson = {
		{"name", "Rect Collider"},
		{"type", sfge::ComponentType::COLLIDER2D},
		{"collider_type", sfge::ColliderType::BOX},
		{"size", {100, 100}},
		{"restitution", 1},
		{"bounce", 1},
		{"friction", 1}
	};

	entityBody1["components"] = {transformJson1, rectShapeJson, rigidBodyJson1, rectColliderJson };
	entityBody2["components"] = {transformJson2, rectShapeJson, rigidBodyJson1, rectColliderJson};

	sceneJson["entities"] = {entityBody1, entityBody2};
	sceneJson["systems"] = json::array({
		{{"script_path", "scripts/contact_debug_system.py"}},
		{{"systemClassName", "Debug"}}
	});

	sceneManager->LoadSceneFromJson(sceneJson);
	engine.Start();
}

TEST(Physics, TestShapeContact)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->gravity = p2Vec2(0.0f, 0.0f);
	engine.Init(std::move(config));

	auto* sceneManager = engine.GetSceneManager();

	json sceneJson;
	sceneJson["name"] = "Contacts";

	const int entitiesNmb = 10;
	json entities[entitiesNmb];

	json shapes[] =
	{
		{
			{"name","Rect Shape Component"},
			{"type",sfge::ComponentType::SHAPE2D},
			{"shape_type", sfge::ShapeType::RECTANGLE},
			{"size",{100,100}}
		},
		{
			{"name","Rect Shape Component"},
			{"type",sfge::ComponentType::SHAPE2D},
			{"shape_type", sfge::ShapeType::CIRCLE},
			{"radius",100}
		}
	};
	json colliders[] =
	{
		{
			{"name","Rect Collider"},
			{"type", sfge::ComponentType::COLLIDER2D},
			{"collider_type",sfge::ColliderType::BOX},
			{"size",{100,100}},
			{"sensor",true}
		},
		{
			{"name","Circle Collider"},
			{"type", sfge::ComponentType::COLLIDER2D},
			{"collider_type",sfge::ColliderType::CIRCLE},
			{"radius",100},
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
			{"velocity", {rand() % 400, rand() % 400}}
		};

		const int randShapeIndex = rand() % 2;
		entityJson["components"] = { transformJson, shapes[randShapeIndex], rigidbody, colliders[randShapeIndex] };

	}

	sceneJson["entities"] = entities;
	sceneJson["systems"] = json::array({
		{
			{ "script_path", "scripts/contact_debug_system.py" }
		},
		{
			{ "script_path", "scripts/stay_onscreen_system.py" }
		}
		/*{
			{ "script_path", "scripts/mouse_raycast_system.py" }
		}*/
	}
	);

	sceneManager->LoadSceneFromJson(sceneJson);
	engine.Start();
}

TEST(Physics, TestShapeContactCpp)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->gravity = p2Vec2(0.0f, 0.0f);
	engine.Init(std::move(config));

	auto* sceneManager = engine.GetSceneManager();

	json sceneJson;
	sceneJson["name"] = "Contacts";

	const auto entitiesNmb = 8;
	json entities[entitiesNmb];

	json shapes[] = {
		{
			{"name","Rect Shape Component"},
			{"type",sfge::ComponentType::SHAPE2D},
			{"shape_type", sfge::ShapeType::RECTANGLE},
			{"size",{50,50}}
		},
		{
			{"name","Circle Shape Component"},
			{"type",sfge::ComponentType::SHAPE2D},
			{"shape_type", sfge::ShapeType::CIRCLE},
			{"radius",25}
		}
	};
	json colliderShapes[] = {
		{
			{"name","Rect Collider"},
			{"type", sfge::ComponentType::COLLIDER2D},
			{"collider_type",sfge::ColliderType::BOX},
			{"size",{50,50}},
			{"sensor",true}
		},
		{
			{"name","Circle Collider"},
			{"type", sfge::ComponentType::COLLIDER2D},
			{"collider_type",sfge::ColliderType::CIRCLE},
			{"radius",25},
			{"sensor",true}
		}
	};

	for (auto& entityJson : entities) {
		json transformJson = {
			{"position",{ rand() % 800,rand() % 600 }},
			{"type", sfge::ComponentType::TRANSFORM2D}
		};
		json rigidbody = {
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
	sceneJson["systems"] = json::array({ {{"systemClassName", "Debug"}} });

	sceneManager->LoadSceneFromJson(sceneJson);
	engine.Start();
}