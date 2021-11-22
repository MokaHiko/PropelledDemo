#include "Game.h"

#include <iostream>
#include <vector>

#include "ResourceManager.h"
#include "Sprite_Renderer.h"
#include "PhysicsEngine2D.h"
#include "GameObject.h"
#include "Enemy.h"
#include "ParticleGenerator.h"
#include "Pilot.h"
#include "Platform.h"
#include "Camera2D.h"

// camera
Camera2D* Camera;

// declare Physics Engine
PhysicsEngine2D* Physics; 
Sprite_Renderer* renderer;

// declare Player object
glm::vec2 HUMAN_SIZE = glm::vec2(50.0f, 50.0f) * 2.0f;
float     PLAYER_VELOCITY(500.0f);
float     PLAYER_JUMP_VELOCITY(100.0f);
Pilot*			Player;

//declare projectiles
ProjectileGenerator* PlayerProjectiles;

// declare particle system
ParticleGenerator* Particles;

// declare enemies & platforms
std::vector<Enemy> Enemies;
std::vector<Platform> Platforms;


Game::Game(unsigned int width, unsigned int height)
	:State(GAME_ACTIVE),Width(width), Height(height), Keys()
{
	HUMAN_SIZE = glm::vec2(Width / 12, Height/ 8);
	PLAYER_JUMP_VELOCITY = HUMAN_SIZE.y;
	PLAYER_VELOCITY = HUMAN_SIZE.x * 2;
}

Game::~Game()
{
	delete Particles;
	delete Physics;
	delete renderer;
	delete PlayerProjectiles;
	delete Player;
}

void Game::Init()
{
	Enemies.clear();
	Platforms.clear();

	// load physics engine
	Physics = new PhysicsEngine2D();

	// load shaders
	ResourceManager::LoadShader("src/shaders/spriteVS.shader", "src/shaders/spriteFS.shader", nullptr, "sprite");
	ResourceManager::LoadShader("src/shaders/particleVS.shader", "src/shaders/particleFS.shader", nullptr, "particle");

	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use();
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);

	// load textures
	ResourceManager::LoadTexture("src/textures/cave.png", true, "cave");
	ResourceManager::LoadTexture("src/spritesheets/enemies/redPriestIdle.png", true, "redPriestIdle");
	ResourceManager::LoadTexture("src/spritesheets/rogue/hero.png", true, "RogueSpriteSheet");
	ResourceManager::LoadTexture("src/spritesheets/UI/greenOutline.png", true, "greenOutline");
	ResourceManager::LoadTexture("src/spritesheets/particles/smokeParticle.png", true, "particle");
	ResourceManager::LoadTexture("src/spritesheets/particles/bullet.png", true, "bullet");
	ResourceManager::LoadTexture("src/spritesheets/particles/bulletDestroyedSpriteSheet.png", true, "bulletDestroyed");
	ResourceManager::LoadTexture("src/spritesheets/particles/blueSquare.png", true, "blueSquareParticle");
	ResourceManager::LoadTexture("src/spritesheets/UI/healthbar.png", true, "healthbar");
	ResourceManager::LoadTexture("src/spritesheets/tiles/darktiles.png", false , "darktiles");

	// load particles
	Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);

	// configure renderer
	renderer = new Sprite_Renderer(ResourceManager::GetShader("sprite"), Width, Height);

	// load enemies
	glm::vec2 enemyPos = glm::vec2(Width / 2.0f - HUMAN_SIZE.x + 80, Height - HUMAN_SIZE.y - 200);
	Animation redPriestIdle = ResourceManager::GetAnimationFromSpriteSheet("src/spritesheets/enemies/redPriestIdle.png", glm::vec2(288, 32), "redPriestIdle", glm::vec2(32, 32), glm::vec2(0, 8), 0);

	for (int i = 5; i < 13; i++)
	{
		glm::vec2 enemyPos = glm::vec2(HUMAN_SIZE.x * i, Height - 600);
		Enemy enemy(enemyPos, HUMAN_SIZE, ResourceManager::GetTexture("redPriestIdle"));
		enemy.AddAnimation("redPriestIdle", redPriestIdle);
		enemy.SetCurrentAnimation("redPriestIdle");
		enemy.SetColliderDimensions(glm::vec2(enemy.Size.x * 0.3, 0), glm::vec2(HUMAN_SIZE.x * 0.5, HUMAN_SIZE.y));
		Enemies.push_back(enemy);
	}

	// load platforms
	for (int level = 1; level < 10; level++)
	{
		for (int col= 0; col< 12; col++)
		{
			if (level % 3 == 0)
			{
				if (col % 3 == 0)
				{
					glm::vec2  darkTilePos = glm::vec2(HUMAN_SIZE.x * col, -HUMAN_SIZE.y * level);
					Platform darkTile(darkTilePos, glm::vec2(HUMAN_SIZE.x, HUMAN_SIZE.y / 4), ResourceManager::GetTexture("darktiles"));
					darkTile.Mass = 0;
					Platforms.push_back(darkTile);
				}
			}
		}
	}

	// player
	glm::vec2 playerPos = glm::vec2(Width / 2.0f - HUMAN_SIZE.x, Height - HUMAN_SIZE.y - 200);
	Player = new Pilot(playerPos, HUMAN_SIZE, ResourceManager::GetTexture("RogueSpriteSheet"), PLAYER_VELOCITY, PLAYER_JUMP_VELOCITY);

	// player collider
	Player->SetColliderDimensions(glm::vec2(Player->Size.x * 0.3, 0), glm::vec2(HUMAN_SIZE.x * 0.4, HUMAN_SIZE.y));
	Player->PlayerParticles = Particles;

	// player animations
	Animation walk = ResourceManager::GetAnimationFromSpriteSheet("src/spritesheets/rogue/hero.png", glm::vec2(500, 444), "walk", glm::vec2(48, 36), glm::vec2(0, 0), 0);
	Player->AddAnimation("idle", walk);
	Player->AddAnimation("walk", walk);
	Player->AddAnimation("jump", walk);
	Player->AddAnimation("propelled", walk);
	Player->SetCurrentAnimation("idle");

	Projectile projectile(glm::vec2(playerPos.x, playerPos.y + 20), HUMAN_SIZE * 0.25f, ResourceManager::GetTexture("bullet"));
	Animation bulletDestroyed = ResourceManager::GetAnimationFromSpriteSheet("src/spritesheets/bulletDestroyedSpriteSheet.png", glm::vec2(16, 80), "bulletDestroyed", glm::vec2(16, 16), glm::vec2(0, 4), 0);
	projectile.AddAnimation("bulletDestroyed", bulletDestroyed);
	PlayerProjectiles = new ProjectileGenerator(projectile);
	Player->PlayerProjectiles = PlayerProjectiles;

	//Load Camera
	Camera = new Camera2D(Width, Height, Player);
}

void Game::ProcessInput(float dt)
{
	if (State == GAME_ACTIVE)
	{
		Player->PlayerController(Keys, Width, Height, dt, MousePos, MouseButtons);
	}
}

void Game::DoCollisions()
{
	// enemy platform collision
	for (int i = 0; i < Enemies.size(); i++)
	{
		for (int j = 0; j < Platforms.size(); j++)
		{
			{
				Physics->HandleCollision(Enemies[i], Platforms[j]);
			}
		}
	}

	// projectile collisions
	for (int j = 0; j < PlayerProjectiles->Projectiles.size(); j++)
	{
		for (Enemy&  enemy: Enemies)
		{
			Physics->HandleCollision(PlayerProjectiles->Projectiles[j], enemy);
		}
	}

	// player collision
	for (int i = 0; i < Platforms.size(); i++)
	{
		Physics->HandleCollision(*Player, Platforms[i]);
	}

	for (Enemy& enemy : Enemies)
	{
		Physics->HandleCollision(*Player, enemy);
	}
}
	
void Game::Update(float dt)
{
	// player physics
	Player->ProcessRigidbody(Width,Height, dt);
	PlayerProjectiles->Update(Width, Height, dt);

	// enemy physics
	for (Enemy& enemy: Enemies)
		enemy.ProcessRigidbody(Width, Height, dt);

	// process particles
	Particles->Update(dt, *Player, 1, glm::vec2(40, 80), !Player->isGrounded);

	// object collisions
	DoCollisions();
}

void Game::Render(float dt)
{
	// draw background
	renderer->DrawSprite(ResourceManager::GetTexture("cave"), glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height), 0.0f);

	if (State == GAME_ACTIVE)
	{
		renderer->ViewMatrix = Camera->GetViewMatrix();
		// add lights
		std::vector<glm::vec2> newLightSources;
		for (GameObject& light : PlayerProjectiles->Projectiles)
		{
			newLightSources.push_back(light.Position);
		}
		renderer->SetLightPositions(newLightSources);

		// draw enemies
		for (Enemy& enemy : Enemies)
		{
			if (!enemy.IsDestroyed)
			{
				enemy.Animate(enemy.currentAnimation, *renderer, dt);
			}
			else
			{
				renderer->LightPositions.push_back(enemy.Position);
			}
		}

		// draw platforms
		for (Platform& platform: Platforms)
		{
			if (!platform.IsDestroyed)
			{
				platform.Draw(*renderer);
			}
		}
		
		//draw particles
		Particles->ViewMatrix = Camera->GetViewMatrix();
		Particles->Draw();


		//draw player
		Player->Animate(Player->currentAnimation, *renderer, dt);
		PlayerProjectiles->Draw(*renderer, dt);
	}
}

