#include "Hooks.h"
#include "Main.h"

namespace BossKillMonitor::Hooks
{
    std::vector<std::string> split(const std::string &text, char sep) {
        std::vector<std::string> tokens;
        std::size_t start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos) {
            tokens.push_back(text.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(text.substr(start));
        return tokens;
    }

    DECLARE_HOOK(AShooterGameMode_Killed, void, AShooterGameMode*, AController *, AController *, APawn *, UDamageType *);

    void  Hook_AShooterGameMode_Killed(AShooterGameMode* _this, AController* Killer, AController* KilledPlayer, APawn* KilledPawn, UDamageType* DamageType)
    {
        if (!Killer)
        {
            AShooterGameMode_Killed_original(_this, Killer, KilledPlayer, KilledPawn, DamageType);
            return;
        }

        APlayerState* killerState = Killer->PlayerStateField();
        if (!killerState)
        {
            AShooterGameMode_Killed_original(_this, Killer, KilledPlayer, KilledPawn, DamageType);
            return;
        }

        FString killerPlayerName = killerState->PlayerNameField();
        int killerPlayerId = killerState->PlayerIdField();
        const uint64 killerSteamId = ArkApi::IApiUtils::GetSteamIdFromController(Killer);

        if (!KilledPlayer)
        {
            AShooterGameMode_Killed_original(_this, Killer, KilledPlayer, KilledPawn, DamageType);
            return;
        }

        FString killedPlayer;
        KilledPlayer->GetFullName(&killedPlayer, nullptr);

        ACharacter* character = KilledPlayer->CharacterField();
        std::string killedName;
        if (KilledPlayer->CharacterField() && character->IsA(APrimalDinoCharacter::GetPrivateStaticClass()))
        {
            FString fullname;
            character->GetFullName(&fullname, nullptr);

            std::vector<std::string> names = split(fullname.ToString(), ' ');
            killedName = names[0];

            /*Log::GetLog()->info("TYPEID {}", typeid(character).name());
            APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(character);
            if (!dino)
            {
                if (dino->bIsBossDino().Get())
                {
                }
                else
                {
                    Log::GetLog()->info("dino->bIsBossDino = false");
                }

                FString dino_name;
                dino->GetDinoDescriptiveName(&dino_name);
                Log::GetLog()->info("dino->GetDinoDescriptiveName = {}", dino_name.ToString());
            }
            else {
                Log::GetLog()->info("dino == null");
            }*/
        }

        UWorld* world = ArkApi::GetApiUtils().GetWorld();
        FString mapname;
        world->GetMapName(&mapname);
        const bool res = database->Killed(killerSteamId, killerPlayerName.ToString(), killedName, mapname.ToString());
        AShooterGameMode_Killed_original(_this, Killer, KilledPlayer, KilledPawn, DamageType);
    }

    void Init()
    {
        ArkApi::GetHooks().SetHook("AShooterGameMode.Killed",
            &Hook_AShooterGameMode_Killed,
            &AShooterGameMode_Killed_original);
    }

    void Unload()
    {
        ArkApi::GetHooks().DisableHook("AShooterGameMode.Killed", &Hook_AShooterGameMode_Killed);
    }
}