#ifndef HIGHSCORE_H
#define HIGHSCORE_H
#include <vector>
#include <utility>

class HighScore
{
public:
	static HighScore& getInstance()
	{
		static HighScore instance;
		return instance;
	}
	void ReadScore();
	void WriteScore();
	bool addScore(int score, std::vector<std::pair<std::string, int>>& scores);
	void addInitials(std::string inputText);
	bool checkAndAddNewScore(int newScore);
	std::vector<std::pair<std::string, int>> getScores();
private:
	HighScore() {};
	HighScore(HighScore const&);
	void operator=(HighScore const&);
	std::vector<std::pair<std::string, int>> scores;
};

#endif 