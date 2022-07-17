#include "ChaosModHelpers.hpp"
#include "ChaosMod.hpp"
#include "LvlArchive.hpp"
#include "Math.hpp"
#include "Grid.hpp"

void ChaosModHelpers::KillSpawnedAliveObjects()
{
    for (s32 iterations = 0; iterations < 2; iterations++)
    {
        s16 idx = 0;

        while (idx < gBaseAliveGameObjects_5C1B7C->Size())
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(idx);
            idx++;

            if (!pObj)
            {
                break;
            }

            if (pObj->createdByChaosMod)
            {
                // DynamicArrayIter iter;
                // iter.field_0_pDynamicArray = gBaseGameObject_list_BB47C4;
                // iter.field_4_idx = idx;
                // iter.Remove_At_Iter_40CCA0();

                // pObj->VDestructor(1);

                //// Don't go forwards as we just removed an item otherwise we'd miss one
                // idx = iter.field_4_idx;
                pObj->field_10C_health = FP_FromInteger(0);
                pObj->field_6_flags.Set(BaseGameObject::Options::eDead_Bit3);
                pObj->field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            }
        }
    }
}

AETypes ChaosModHelpers::GetActiveGiveThrowableEffect()
{
    auto activeEffect = chaosMod.getRealActiveEffect();
    switch (activeEffect)
    {
        case ChaosEffect::ShrykullGrenades:
        case ChaosEffect::GiveGrenades:
            return AETypes::eGrenade_65;
        case ChaosEffect::GiveRocks:
            return AETypes::eRock_105;
        case ChaosEffect::GiveMeat:
            return AETypes::eMeat_84;
        case ChaosEffect::GiveBones:
            return AETypes::eBone_11;
        default:
            return AETypes::eNone_0;
    }
}

void ChaosModHelpers::LoadResourceFromAnotherLvl(ResourceManager::ResourceType type, AEResourceID resourceId, LevelIds levelId, const char_type* pFileName)
{
    if (!ResourceManager::GetLoadedResource_49C2A0(type, resourceId, 0, 0))
    {
        LevelIds previousLvl = LevelIds::eNone;
        if (gMap_5C3030.field_A_level != levelId)
        {
            previousLvl = gMap_5C3030.field_A_level;

            sLvlArchive_5BC520.Free_433130();
            sLvlArchive_5BC520.Open_Archive_432E80(CdLvlName(levelId));
        }

        ResourceManager::LoadResourceFile_49C170(pFileName, nullptr);

        if (previousLvl != LevelIds::eNone)
        {
            sLvlArchive_5BC520.Free_433130();
            sLvlArchive_5BC520.Open_Archive_432E80(CdLvlName(previousLvl));
        }
    }
}

void ChaosModHelpers::LoadThrowableResources(AETypes grenadeType)
{
    LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kAbepickResID, LevelIds::eMines_1, "ABEPICK.BAN");
    LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID, LevelIds::eMines_1, "ABETHROW.BAN");

    switch (grenadeType)
    {
        case AETypes::eBone_11:
            LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kBoneResID, LevelIds::eBonewerkz_8, "BONE.BAN");
            break;

        case AETypes::eMetal_24:
            LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, LevelIds::eBarracks_6, "METAL.BAN");
            LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, LevelIds::eBarracks_6, "GRENADE.BAN");
            break;

        case AETypes::eGrenade_65:
            LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, LevelIds::eBarracks_6, "EXPLO2.BAN");
            LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, LevelIds::eBarracks_6, "ABEBLOW.BAN");
            LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, LevelIds::eBarracks_6, "DOGBLOW.BAN");
            LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, LevelIds::eBarracks_6, "METAL.BAN");
            LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, LevelIds::eBarracks_6, "GRENADE.BAN");
            LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kSplineResID, LevelIds::eBarracks_6, "SPLINE.BAN");
            break;

        case AETypes::eMeat_84:
            LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kMeatResID, LevelIds::eMudomoVault_3, "MEAT.BAN");
            break;

        case AETypes::eRock_105:
            LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kAberockResID, LevelIds::eMines_1, "PUIROCK.BAN");
            break;

        default:
            break;
    }
}

u16 ChaosModHelpers::AddNewTLV(Path_TLV* newTlv)
{
    auto tlv = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera_4DB610(0, 0);
    while (tlv && !tlv->field_0_flags.Get(TLV_Flags::eBit3_End_TLV_List))
        tlv = sPath_dword_BB47C0->Next_TLV(tlv);

    if (tlv)
    {
        TlvItemInfoUnion data;
        data.all = sPath_dword_BB47C0->TLVInfo_From_TLVPtr_4DB7C0(tlv);

        u16 newOffset = data.parts.tlvOffset + tlv->field_2_length;
        tlv->field_0_flags.Clear(TLV_Flags::eBit3_End_TLV_List);

        newTlv->field_0_flags.Set(TLV_Flags::eBit3_End_TLV_List);
        (*sPath_dword_BB47C0->field_10_ppRes)[sPath_dword_BB47C0->field_C_pPathData->field_12_object_offset + newOffset] = static_cast<u8>(reinterpret_cast<u64>(newTlv));
        return newOffset;
    }

    return 0;
}



void ChaosModHelpers::LoadSlogResources()
{
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kDogbasicResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLOG.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kDogbasicResID, FALSE, FALSE))
    {
        LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kDogbasicResID, LevelIds::eBonewerkz_8, "SLOG.BND");
        LoadResourceFromAnotherLvl(ResourceManager::Resource_Animation, AEResourceID::kDogknfdResID, LevelIds::eBonewerkz_8, "DOGKNFD.BAN");
        return;
    }
    else if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kDogknfdResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("DOGKNFD.BAN", nullptr);
    }
}

Slog* ChaosModHelpers::SpawnSlog(s16 xpos, s16 ypos, FP scale, XDirection_short startDirection, Choice_short asleep)
{
    bool isHalfScale = scale == FP_FromDouble(0.5);
    s16 width = isHalfScale ? 13 : 25;

    Path_Slog path;
    path.field_1_tlv_state = 0;
    path.field_2_length = sizeof(Path_Slog);
    path.field_4_type = {TlvTypes::SligPersist_46, 0};
    path.field_8_top_left.field_0_x = xpos;
    path.field_8_top_left.field_2_y = ypos;
    path.field_C_bottom_right.field_0_x = xpos + width;
    path.field_C_bottom_right.field_2_y = ypos + width;
    path.field_10_scale = isHalfScale ? Scale_short::eHalf_1 : Scale_short::eFull_0;
    path.field_12_direction = startDirection;
    path.field_14_asleep = asleep;
    path.field_16_wake_up_anger = 10;
    path.field_18_bark_anger = 15;
    path.field_1A_chase_anger = 20;
    path.field_1C_chase_delay = 25;
    path.field_1E_disabled_resources = 0;
    path.field_20_anger_switch_id = 0;
    path.field_22_bone_eating_time = 60;

    LoadSlogResources();


    auto slog = ae_new<Slog>();
    slog->createdByChaosMod = true;
    slog->ctor_4C42E0(&path, 0xFFFF);
    slog->MapFollowMe_408D10(1);

    return slog;
}

Slog* ChaosModHelpers::SpawnSlog(FP xpos, FP ypos, FP scale, XDirection_short startDirection, Choice_short asleep)
{
    return SpawnSlog(FP_GetExponent(xpos), FP_GetExponent(ypos), scale, startDirection, asleep);
}

void ChaosModHelpers::SpawnSlogs(FP xpos, FP ypos, FP scale, Choice_short asleep, u8 count)
{
    s16 xpos16 = FP_GetExponent(xpos);
    s16 ypos16 = FP_GetExponent(ypos);

    s16 count2 = count / 2;
    s16 gridWidth = scale == FP_FromDouble(0.5) ? 13 : 25;

    for (u8 i = 0; i < count; ++i)
    {
        s16 xpos16_i = xpos16 - (count2 * gridWidth) + (i * gridWidth);
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(gMap_5C3030.field_0_current_level, gMap_5C3030.field_2_current_path, FP_FromInteger(xpos16_i), ypos, gridWidth))
        {
            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;
            if (sCollisions_DArray_5C1128->Raycast_417A60(
                    FP_FromInteger(xpos16_i), ypos - FP_FromInteger(40),
                    FP_FromInteger(xpos16_i), ypos + FP_FromInteger(240),
                    &pLine, &hitX, &hitY, scale == FP_FromInteger(1) ? 1 : 16))
            {
                ypos16 = FP_GetExponent(hitY);
            }
            SpawnSlog(xpos16_i, ypos16, scale, i <= count2 ? XDirection_short::eRight_1 : XDirection_short::eLeft_0, asleep);
        }
    }
}



void ChaosModHelpers::LoadSligResources(s16 disabledResources)
{
    if (!(disabledResources & 1) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgleverResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGLEVER.BAN", nullptr);
    }

    if (!(disabledResources & 2) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgliftResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGLIFT.BAN", nullptr);
    }

    if (!(disabledResources & 0x40) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgsleepResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGSLEEP.BAN", nullptr);
    }

    if ((disabledResources & 0x80u) == 0 && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgknfdResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGKNFD.BAN", nullptr);
    }

    if (!(disabledResources & 0x100) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgedgeResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGEDGE.BAN", nullptr);
    }

    if (!(disabledResources & 0x200) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgsmashResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGSMASH.BAN", nullptr);
    }

    if (!(disabledResources & 0x400) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgbeatResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGBEAT.BAN", nullptr);
    }

    if (!(disabledResources & 4) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgzshotResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLIGZ.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgbasicResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLIG.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SLGBLOW.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("SHADOW.BAN", nullptr);
    }
}

Slig* ChaosModHelpers::SpawnSlig(s16 xpos, s16 ypos, FP scale, Path_Slig::StartState startState, s16 shootOnSightDelay, XDirection_short startDirection)
{
    bool isHalfScale = scale == FP_FromDouble(0.5);
    s16 width = isHalfScale ? 13 : 25;

    Path_Slig path;
    // path.field_0_flags
    path.field_1_tlv_state = 0;
    path.field_2_length = sizeof(Path_Slig);
    path.field_4_type = {TlvTypes::SligPersist_46, 0};
    path.field_8_top_left.field_0_x = xpos;
    path.field_8_top_left.field_2_y = ypos;
    path.field_C_bottom_right.field_0_x = xpos + width;
    path.field_C_bottom_right.field_2_y = ypos + width;
    path.field_10_scale = isHalfScale ? Scale_short::eHalf_1 : Scale_short::eFull_0;
    path.field_12_start_state = startState;
    path.field_14_pause_time = 30;
    path.field_16_pause_left_min = 30;
    path.field_18_pause_left_max = 60;
    path.field_1A_pause_right_min = 30;
    path.field_1C_pause_right_max = 60;
    path.field_1E_shoot_possessed_sligs = Path_Slig::ShootPossessedSligs::eYes_0;
    path.field_20_shoot_on_sight_delay = shootOnSightDelay;
    path.field_22_num_times_to_shoot = 3;
    // path.field_24_padding
    // path.field_26_code1;
    // path.field_28_code2
    path.field_2A_chase_abe_when_spotted = Choice_short::eYes_1;
    path.field_2C_start_direction = startDirection;
    path.field_2E_panic_timeout = 300;
    path.field_30_num_panic_sounds = 3;
    path.field_32_panic_sound_timeout = 60;
    path.field_34_stop_chase_delay = 8;
    path.field_36_time_to_wait_before_chase = 30;
    // path.field_38_slig_bound_id
    path.field_3A_alerted_listen_time = 90;
    path.field_3C_percent_say_what = 100; // 0 = 0%, 100 = 100%
    path.field_3E_percent_beat_mud = 256; // 0 = 0%, 256 or higher = 100%
    // path.field_40_talk_to_abe
    path.field_42_dont_shoot = 0;
    path.field_44_Z_shoot_delay = 30;
    path.field_46_stay_awake = Choice_short::eYes_1;
    path.field_48_disable_resources = 0;
    path.field_4A_noise_wake_up_distance = 5;
    path.field_4C_slig_spawner_switch_id = 0;
    path.field_4E_unlimited_spawns = Choice_short::eNo_0;

    LoadSligResources(path.field_48_disable_resources);


    auto slig = ae_new<Slig>();
    slig->createdByChaosMod = true;
    slig->ctor_4B1370(&path, 0xFFFF);
    slig->MapFollowMe_408D10(1);

    return slig;
}

Slig* ChaosModHelpers::SpawnSlig(FP xpos, FP ypos, FP scale, Path_Slig::StartState startState, s16 shootOnSightDelay, XDirection_short startDirection)
{
    return SpawnSlig(FP_GetExponent(xpos), FP_GetExponent(ypos), scale, startState, shootOnSightDelay, startDirection);
}

void ChaosModHelpers::SpawnSligs(FP xpos, FP ypos, FP scale, Path_Slig::StartState startState, s16 shootOnSightDelay, u8 count)
{
    s16 xpos16 = FP_GetExponent(xpos);
    s16 ypos16 = FP_GetExponent(ypos);

    s16 count2 = count / 2;
    s16 gridWidth = scale == FP_FromDouble(0.5) ? 13 : 25;

    for (u8 i = 0; i < count; ++i)
    {
        s16 xpos16_i = xpos16 - (count2 * gridWidth) + (i * gridWidth);
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(gMap_5C3030.field_0_current_level, gMap_5C3030.field_2_current_path, FP_FromInteger(xpos16_i), ypos, gridWidth))
        {
            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;
            if (sCollisions_DArray_5C1128->Raycast_417A60(
                    FP_FromInteger(xpos16_i), ypos - FP_FromInteger(40),
                    FP_FromInteger(xpos16_i), ypos + FP_FromInteger(240),
                    &pLine, &hitX, &hitY, scale == FP_FromInteger(1) ? 1 : 16))
            {
                ypos16 = FP_GetExponent(hitY);
            }
            SpawnSlig(xpos16_i, ypos16, scale, startState, shootOnSightDelay, i <= count2 ? XDirection_short::eRight_1 : XDirection_short::eLeft_0);
        }
    }
}

void ChaosModHelpers::LoadUXBResources()
{
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("DOGBLOW.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kUXBResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("UXB.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("EXPLODE.BND", nullptr);
    }
}

UXB* ChaosModHelpers::SpawnUXB(s16 xpos, s16 ypos, FP scale, Path_UXB::StartState startState)
{
    bool isHalfScale = scale == FP_FromDouble(0.5);
    s16 width = isHalfScale ? 13 : 25;

    Path_UXB path;
    // path.field_0_flags
    path.field_1_tlv_state = 0;
    path.field_2_length = sizeof(Path_Slig);
    path.field_4_type = {TlvTypes::SligPersist_46, 0};
    path.field_8_top_left.field_0_x = xpos;
    path.field_8_top_left.field_2_y = ypos;
    path.field_C_bottom_right.field_0_x = xpos + width;
    path.field_C_bottom_right.field_2_y = ypos + width;
    path.field_10_pattern_length = 4;
    path.field_12_pattern = Math_RandomRange_496AB0(1, 9) * 1000 + Math_RandomRange_496AB0(1, 9) * 100 + Math_RandomRange_496AB0(1, 9) * 10 + Math_RandomRange_496AB0(1, 9);
    path.field_14_scale = isHalfScale ? Scale_short::eHalf_1 : Scale_short::eFull_0;
    path.field_16_start_state = startState;
    path.field_18_disabled_resources = 0;

    TlvItemInfoUnion tlv;
    tlv.all = 0xFFFFFFFF;
    
    LoadUXBResources();


    auto uxb = ae_new<UXB>();
    uxb->createdByChaosMod = true;
    uxb->ctor_4DE9A0(&path, tlv);
    uxb->MapFollowMe_408D10(1);

    return uxb;
}

UXB* ChaosModHelpers::SpawnUXB(FP xpos, FP ypos, FP scale, Path_UXB::StartState startState)
{
    return SpawnUXB(FP_GetExponent(xpos), FP_GetExponent(ypos), scale, startState);
}

void ChaosModHelpers::SpawnUXBs(FP xpos, FP ypos, FP scale, Path_UXB::StartState startState, u8 count)
{
    count += 1; // to avoid the middle (abe's pos)

    s16 xpos16 = FP_GetExponent(xpos);
    s16 ypos16 = FP_GetExponent(ypos);

    s16 count2 = count / 2;
    s16 gridWidth = scale == FP_FromDouble(0.5) ? 13 : 25;

    for (u8 i = 0; i < count; ++i)
    {
        if (i == count2)
            continue;

        s16 xpos16_i = xpos16 - (count2 * gridWidth) + (i * gridWidth);
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(gMap_5C3030.field_0_current_level, gMap_5C3030.field_2_current_path, FP_FromInteger(xpos16_i), ypos, gridWidth))
        {
            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;
            if (sCollisions_DArray_5C1128->Raycast_417A60(
                    FP_FromInteger(xpos16_i), ypos - FP_FromInteger(40),
                    FP_FromInteger(xpos16_i), ypos + FP_FromInteger(240),
                    &pLine, &hitX, &hitY, scale == FP_FromInteger(1) ? 1 : 16))
            {
                ypos16 = FP_GetExponent(hitY);
            }
            SpawnUXB(xpos16_i, ypos16, scale, startState);
        }
    }
}