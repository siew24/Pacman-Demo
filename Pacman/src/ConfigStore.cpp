#include "../inc/ConfigStore.h"

double ConfigStore::ghostVolume = 60.0;
double ConfigStore::pacmanVolume = 60.0;
double ConfigStore::musicVolume = 60.0;
int ConfigStore::debug = 0;
bool ConfigStore::instaQuit = false;

std::string SessionStore::currentName = "";

// Default scores
std::vector<std::pair<std::string, int>> LeaderboardsStore::leaderboards;