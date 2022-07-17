#include "ChaosMod.hpp"
#include "DDCheat.hpp"


void ChaosMod::init()
{
    lib = LoadLibrary("AEChaosModManaged.dll");
    pInit = (__init) GetProcAddress(lib, "_init");
    pGetEffectDuration = (__getEffectDuration) GetProcAddress(lib, "_getEffectDuration");
    pGetFullEffectDuration = (__getFullEffectDuration) GetProcAddress(lib, "_getFullEffectDuration");
    pGetActiveEffect = (__getActiveEffect) GetProcAddress(lib, "_getActiveEffect");
    pGetSpeedModifier = (__getSpeedModifier) GetProcAddress(lib, "_getSpeedModifier");
    pGetInfo = (__getInfo) GetProcAddress(lib, "_getInfo");
    pSkipCurrentEffect = (__skipCurrentEffect) GetProcAddress(lib, "_skipCurrentEffect");
    pClose = (__close) GetProcAddress(lib, "_close");

    pInit();
}

void ChaosMod::update()
{
    auto effectCombined = pGetActiveEffect();
    auto effect = (ChaosEffect)(u16)effectCombined;

    activeEffect = effect;
    auto newEffectDuration = pGetEffectDuration();

    if (activeEffect != previousEffect || newEffectDuration > effectDuration)
    {
        previousEffect = effect;

        bool oneUse = effectCombined >> 24;
        effectOneTimeUse = oneUse;
        effectUsed = false;
        effectUsedDirty = false;
    }
    else if (effectUsedDirty)
    {
        effectUsed = true;
        effectUsedDirty = false;
    }

    effectDuration = newEffectDuration;
}

ChaosEffect ChaosMod::getActiveEffect()
{
    if (effectOneTimeUse)
        return effectUsed ? ChaosEffect::None : activeEffect;
    else
        return activeEffect;
}

void ChaosMod::markEffectAsUsed()
{
    if (effectOneTimeUse && !effectUsed)
    {
        effectUsedDirty = true;
    }
}

ChaosEffect ChaosMod::getRealActiveEffect()
{
    return activeEffect;
}

s32 ChaosMod::getEffectDuration()
{
    return effectDuration;
}

s32 ChaosMod::getFullEffectDuration()
{
    return pGetFullEffectDuration();
}

float ChaosMod::getSpeedModifier()
{
    return pGetSpeedModifier();
}

void ChaosMod::printInfo()
{
    char* str;
    str = new (char[2024]);
    
    pGetInfo(str, 2024);

    for (char_type* line = strtok(str, "\n\r"); line; line = strtok(0, "\n\r"))
        DDCheat::DebugStr_4F5560("%s\n", line);

    delete[] str;
}

void ChaosMod::skipCurrentEffect()
{
    pSkipCurrentEffect();
}

void ChaosMod::close()
{
    pClose();
    FreeLibrary(lib);
}

ChaosMod chaosMod;