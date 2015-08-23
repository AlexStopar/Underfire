#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "highscore.h"
#include "common.h"

void HighScore::ReadScore()
{
	std::string line;
	std::ifstream highScoreFile("highscores.txt");
	for (int i = 0; i < 11; i++)
	{
		std::getline(highScoreFile, line);
		scores.push_back(std::make_pair(line.substr(0, line.find(",")),
				atoi(line.substr(line.find(",") + 1, line.size() - 1).c_str())));
	}
	highScoreFile.close();
}

void HighScore::WriteScore()
{
	std::ofstream highScoreFile("highscores.txt");
	for (int i = 0; i < 11; i++)
	{
		highScoreFile << scores[i].first << "," << scores[i].second;
		highScoreFile << "\n";
	}
	highScoreFile.close();
}

bool HighScore::checkAndAddNewScore(int newScore)
{
	bool isNewScore = addScore(newScore, scores);	
	return isNewScore;
}


bool HighScore::addScore(int score, std::vector<std::pair<std::string, int>>& scores)
{
	bool isNewScore = false;
	for (std::vector<std::pair<std::string, int>>::iterator it = scores.begin(); it != scores.end(); ++it) {
		if (score == it->second) return false;
	}
	scores.push_back(std::make_pair("AAA", score));
	std::sort(scores.begin(), scores.end(), [](const std::pair<std::string, int> &p1, const std::pair<std::string, int> &p2)
	{
		return p1.second > p2.second;
	});
	if (scores[11].second != score) isNewScore = true;
	scores.pop_back();
	return isNewScore;
}

void HighScore::addInitials(std::string inputText)
{
	for (std::vector<std::pair<std::string, int>>::iterator it = scores.begin(); it != scores.end(); ++it) {
		if (it->second == score) it->first = inputText;
	}
}

std::vector<std::pair<std::string, int>> HighScore::getScores()
{
	return scores;
}

