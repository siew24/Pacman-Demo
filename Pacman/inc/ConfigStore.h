#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

struct ConfigStore {
	static double ghostVolume;
	static double pacmanVolume;
	static double musicVolume;
	static int debug;
};
struct LeaderboardsStore {
	static int addEntry(std::string name, int score) {
		for (int i = 0; i < leaderboards.size(); ++i) {
			if (leaderboards[i].second < score) {
				leaderboards.insert(leaderboards.begin() + i, std::make_pair(name, score));
				return i;
			}
		}
		leaderboards.emplace_back(std::make_pair(name, score));
		return leaderboards.size() - 1;
	}

	static std::vector<std::pair<std::string, int>> leaderboards;
};