#pragma once
#include <vector>
#include <tuple>
#include <algorithm>

struct ConfigStore {
	static double volume;
	static int debug;
};
struct LeaderboardsStore {
	static void addEntry(std::string name, int score) {
		leaderboards.push_back(std::make_pair(name, score));
		std::sort(leaderboards.begin(), leaderboards.end(),
			[](auto& lhs, auto& rhs) {
				return lhs.second > rhs.second;
			}
		);
		leaderboards.pop_back();
	}

	static std::vector<std::pair<std::string, int>> leaderboards;
};