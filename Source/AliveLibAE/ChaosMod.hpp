#pragma once

typedef void(__cdecl* __init)();
typedef u32(__cdecl* __getActiveEffect)();
typedef s32(__cdecl* __getEffectDuration)();
typedef s32(__cdecl* __getFullEffectDuration)();
typedef float(__cdecl* __getSpeedModifier)();
typedef void(__cdecl* __getInfo)(char* str, int len);
typedef void(__cdecl* __skipCurrentEffect)();
typedef void(__cdecl* __close)();

enum ChaosEffect : u16
{
    None,
    Other,
    AbeSuicide,
    RestartPath,
    ThrowBackwards,
    Disco,
    AbeInvisible,
    AllMudsToAbe,
    GiveGrenades,
    SpawnSlig,
    SpawnSligs,
    GiveRocks,
    GiveMeat,
    GiveBones,
    ShrykullGrenades,
    SpawnSlog,
    SpawnSlogs,
    Dinnerbone,
    HorizontalDinnerbone,
    Superhot,
    InGameChat,
    FakeAbeSuicide,
    OnePunchAbe,
    IndestructibleAbe,
    FatAbe,
    BarrelRoll,
    FlipScreenX,
    FlipScreenY,
    BouncyThrowables,
    SpawnUXBs,
    Fade,
    GiveRedRing,
    GiveShrykullRing,
    GiveInvisibilityRing,
    GiveHealingRing,
    DoubleJump,
    FartCommunication,
};

class ChaosMod final
{
public:
    void init();
    void update();
    ChaosEffect getActiveEffect();
    void markEffectAsUsed();
    ChaosEffect getRealActiveEffect();
    int getEffectDuration();
    int getFullEffectDuration();
    float getSpeedModifier();
    void printInfo();
    void skipCurrentEffect();
    void close();

private:
    HMODULE lib;
    __init pInit;
    __getActiveEffect pGetActiveEffect;
    __getEffectDuration pGetEffectDuration;
    __getFullEffectDuration pGetFullEffectDuration;
    __getSpeedModifier pGetSpeedModifier;
    __getInfo pGetInfo;
    __skipCurrentEffect pSkipCurrentEffect;
    __close pClose;
    ChaosEffect previousEffect;
    ChaosEffect activeEffect;
    bool effectOneTimeUse;
    bool effectUsed;
    bool effectUsedDirty;
    s32 effectDuration;
};

extern ChaosMod chaosMod;
