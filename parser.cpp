#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "parser.h"
#include "level.h"
#include "common.h"
#include "gate.h"
#include "boss.h"

void Parser::Parse(std::string levelName, std::vector<PowerUp>& powerups, std::vector<Enemy>& enemies, std::vector<Asteroid>& asteroids, std::vector<Gate>& gates, Boss& boss)
{
	int xloc = SCREEN_WIDTH;
	int yloc = 0;
	powerups.clear();
	std::ifstream reader(levelName);
	std::string line;
	while (std::getline(reader, line))
	{
		std::istringstream ss(line);
		std::string token;
		while (std::getline(ss, token, ','))
		{
			if (token == "rap") powerups.push_back(PowerUp(xloc, yloc, POWERUP_WIDTH, POWERUP_HEIGHT, POWERUP_VEL, PowerUpType::RAPID));
			else if (token == "sco") powerups.push_back(PowerUp(xloc, yloc, POWERUP_WIDTH, POWERUP_HEIGHT, POWERUP_VEL, PowerUpType::SCORE));
			else if (token == "bul") powerups.push_back(PowerUp(xloc, yloc, POWERUP_WIDTH, POWERUP_HEIGHT, POWERUP_VEL, PowerUpType::BULLET));
			else if (token == "lif") powerups.push_back(PowerUp(xloc, yloc, POWERUP_WIDTH, POWERUP_HEIGHT, POWERUP_VEL, PowerUpType::LIFE));
			else if (token == "g") gates.push_back(Gate(xloc, yloc, GATE_WIDTH, GATE_HEIGHT, GATE_VEL));
			else if (token == "ees") enemies.push_back(Enemy(xloc, yloc, resolveEnemyEasyWidth(), ENEMY_EASY_HEIGHT, ENEMY_VEL, EnemyType::EASY, EnemyMoveType::STRAIGHT));
			else if (token == "eeu") enemies.push_back(Enemy(xloc, yloc, resolveEnemyEasyWidth(), ENEMY_EASY_HEIGHT, ENEMY_VEL, EnemyType::EASY, EnemyMoveType::CURVEUP));
			else if (token == "eed") enemies.push_back(Enemy(xloc, yloc, resolveEnemyEasyWidth(), ENEMY_EASY_HEIGHT, ENEMY_VEL, EnemyType::EASY, EnemyMoveType::CURVEDOWN));
			else if (token == "em") enemies.push_back(Enemy(xloc, yloc, resolveEnemyMediumWidth(), ENEMY_MEDIUM_HEIGHT, resolveMediumVel(), EnemyType::MEDIUM, EnemyMoveType::STRAIGHT));
			else if (token == "a") asteroids.push_back(Asteroid(xloc, yloc, ASTEROID_WIDTH, ASTEROID_HEIGHT, ASTEROID_VEL, Size::LARGE, Direction::W));
			else if (token == "b")
			{
				boss.setX(xloc);
				boss.setY(yloc - resolveBossOffset(true));
			}
			xloc += PARSING_UNIT;
		}
		xloc = SCREEN_WIDTH;
		yloc += PARSING_UNIT;
	}
}
