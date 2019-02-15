#pragma once

#include <SQLiteCpp/Database.h>

#include "IDatabase.h"
#include "../Main.h"

class SqlLite : public IDatabase
{
public:
    explicit SqlLite(const std::string& path)
        : db_(path.empty()
            ? BossKillMonitor::GetDbPath()
            : path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
    {
        try
        {
            db_.exec("PRAGMA journal_mode=WAL;");
        }
        catch (const std::exception& exception)
        {
            Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
        }
    }

    bool Killed(uint64 killerSteamId, std::string killerPlayerName, std::string killedPlayer, std::string mapname)
    {
        return true;
    }

private:
    SQLite::Database db_;
};
