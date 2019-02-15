#pragma once

#include <mysql++11.h>

#include "IDatabase.h"

#pragma comment(lib, "mysqlclient.lib")

class MySql : public IDatabase
{
public:
    explicit MySql(std::string server, std::string username, std::string password, std::string db_name)
    {
        try
        {
            daotk::mysql::connect_options options;
            options.server = move(server);
            options.username = move(username);
            options.password = move(password);
            options.dbname = move(db_name);
            options.autoreconnect = true;
            options.timeout = 30;

            bool result = db_.open(options);
            if (!result)
            {
                Log::GetLog()->critical("Failed to open connection!");
                return;
            }

            result = db_.query(fmt::format("CREATE TABLE IF NOT EXISTS Killer ("
                "`id` INT NOT NULL AUTO_INCREMENT,"
                "`mapname` VARCHAR(30) NULL,"
                "`killerSteamId` BIGINT(11) UNSIGNED NULL,"
                "`killerPlayerName` VARCHAR(100) NULL,"
                "`killedPlayer` VARCHAR(200) NULL,"
                "`datetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                "PRIMARY KEY(id),"
                "INDEX ix_killerSteamId (killerSteamId ASC),"
                "INDEX ix_mapname (mapname ASC));"));

            if (!result)
            {
                Log::GetLog()->critical("({} {}) Failed to create table!", __FILE__, __FUNCTION__);
            }
        }
        catch (const std::exception& exception)
        {
            Log::GetLog()->critical("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
        }
    }

    bool Killed(uint64 killerSteamId, std::string killerPlayerName, std::string killedPlayer, std::string mapname)
    {
        bool result = db_.query(fmt::format("INSERT INTO Killer (killerSteamId, killerPlayerName, killedPlayer, mapname) VALUES ({}, '{}', '{}', '{}');", killerSteamId, killerPlayerName, killedPlayer, mapname));
        if (!result)
        {
            Log::GetLog()->critical("({} {}) Failed to insert killer table! {}", __FILE__, __FUNCTION__, db_.error_message());
        }

        return result;
    }

private:
    daotk::mysql::connection db_;
};
