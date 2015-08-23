#ifndef PARSER_H
#define PARSER_H
#include <SDL.h>
#include "powerup.h"
#include "enemy.h"
#include "asteroid.h"
#include "gate.h"
#include "boss.h"

class Parser
{
public:
	static Parser& getInstance()
	{
		static Parser instance;
		return instance;
	}
	void Parse(std::string levelName, std::vector<PowerUp>& powerups, std::vector<Enemy>& enemies, std::vector<Asteroid>& asteroids, std::vector<Gate>& gates, Boss& boss);
private:
	const int PARSING_UNIT = 16;
	Parser() {};
	Parser(Parser const&);
	void operator=(Parser const&);
};


#endif