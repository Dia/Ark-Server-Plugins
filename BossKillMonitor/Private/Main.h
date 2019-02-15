#pragma once

#include "Database/IDatabase.h"

namespace BossKillMonitor
{
    inline std::unique_ptr<IDatabase> database;

    std::string GetDbPath();

    enum Action {
        Add = 0x0,
        Drop = 0x1,
        Craft = 0x2,
        Upload = 0x3,
        Download = 0x4,
        Consume = 0x5,
    };
}
