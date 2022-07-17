#pragma once

#include "stdlib.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"
#include "Path.hpp"
#include "Slig.hpp"
#include "Slog.hpp"
#include "UXB.hpp"


namespace ChaosModHelpers {
void KillSpawnedAliveObjects();

AETypes GetActiveGiveThrowableEffect();

void LoadResourceFromAnotherLvl(ResourceManager::ResourceType type, AEResourceID resourceId, LevelIds levelId, const char_type* pFileName);

void LoadThrowableResources(AETypes grenadeType);

u16 AddNewTLV(Path_TLV* newTlv);


void LoadSlogResources();

Slog* SpawnSlog(s16 xpos, s16 ypos, FP scale, XDirection_short startDirection, Choice_short asleep);

Slog* SpawnSlog(FP xpos, FP ypos, FP scale, XDirection_short startDirection, Choice_short asleep);

void SpawnSlogs(FP xpos, FP ypos, FP scale, Choice_short asleep, u8 count);


void LoadSligResources(s16 disabledResources);

Slig* SpawnSlig(s16 xpos, s16 ypos, FP scale, Path_Slig::StartState startState, s16 shootOnSightDelay, XDirection_short startDirection);

Slig* SpawnSlig(FP xpos, FP ypos, FP scale, Path_Slig::StartState startState, s16 shootOnSightDelay, XDirection_short startDirection);

void SpawnSligs(FP xpos, FP ypos, FP scale, Path_Slig::StartState startState, s16 shootOnSightDelay, u8 count);


void LoadUXBResources();

UXB* SpawnUXB(s16 xpos, s16 ypos, FP scale, Path_UXB::StartState startState);

UXB* SpawnUXB(FP xpos, FP ypos, FP scale, Path_UXB::StartState startState);

void SpawnUXBs(FP xpos, FP ypos, FP scale, Path_UXB::StartState startState, u8 count);
} // namespace ChaosModHelpers