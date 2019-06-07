#include "../inc/ConfigStore.h"

double ConfigStore::volume = 100;
int ConfigStore::debug = 0;

// Default scores
std::vector<std::pair<std::string, int>> LeaderboardsStore::leaderboards =
{
	std::make_pair("Derrick", 10000),
	std::make_pair("Pacman", 9000),
	std::make_pair("Siew24", 7000),
	std::make_pair("TheLegend27", 6500),
	std::make_pair("Inky", 5000),
	std::make_pair("Pinky", 3700),
	std::make_pair("Hawk", 3500),
	std::make_pair("Clyde", 3000),
	std::make_pair("Blinky", 3000),
	std::make_pair("Steven", 2000)
};