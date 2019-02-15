#pragma once

namespace BossKillMonitor
{
    inline std::string GetDbPath()
    {
#ifdef BOSSKILLMONITOR_ARK
        return ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/BossKillMonitor/ArkDB.db";
#else
        return ArkApi::Tools::GetCurrentDir() + "/AtlasApi/Plugins/BossKillMonitor/AtlasDB.db";
#endif
    }

    inline std::string GetConfigPath()
    {
#ifdef BOSSKILLMONITOR_ARK
        return ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/BossKillMonitor/config.json";
#else
        return ArkApi::Tools::GetCurrentDir() + "/AtlasApi/Plugins/BossKillMonitor/config.json";
#endif
    }
}
