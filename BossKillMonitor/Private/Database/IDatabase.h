#pragma once

#ifdef BOSSKILLMONITOR_ARK
#include "../Public/ArkBossKillMonitor.h"
#else
#include "../Public/AtlasBossKillMonitor.h"
#endif

class IDatabase
{
public:
	virtual ~IDatabase() = default;

    virtual bool Killed(uint64 killerSteamId, std::string killerPlayerName, std::string killedPlayer, std::string mapname) = 0;
};
