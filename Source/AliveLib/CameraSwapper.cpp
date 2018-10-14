#include "stdafx.h"
#include "CameraSwapper.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "Sound.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"

class ScreenClipper : public BaseGameObject
{
public:
    EXPORT ScreenClipper* ctor_416D60(PSX_Point xy, PSX_Point wh, __int16 layer)
    {
        BaseGameObject_ctor_4DBFA0(1, 0);

        SetVTable(this, 0x5445C4);
        field_4_typeId = Types::eScreenClipper;

        field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap);
        field_6_flags.Set(BaseGameObject::eDrawable);

        field_40_rect.x = xy.field_0_x;
        field_40_rect.y = xy.field_2_y;
        field_40_rect.w = wh.field_0_x;
        field_40_rect.h = wh.field_2_y;

        field_48_ot_layer = layer;

        gObjList_drawables_5C1124->Push_Back(this);

        // This actually matters here due how its called
        return this;
    }

    virtual void VDestructor(signed int flags)
    {
        vDtor_416E00(flags);
    }

    virtual void VRender(int** ot) override
    {
        vRender_416F70(ot);
    }

    virtual void VScreenChanged() {}
    virtual void VUpdate() { }

    void vRender_416F70(int** ot)
    {
        PSX_RECT clipRect = {};

        clipRect.x = field_40_rect.x;
        clipRect.y = field_40_rect.y;
        clipRect.w = field_40_rect.w - field_40_rect.x;
        clipRect.h = field_40_rect.h - field_40_rect.y;

        if (gPsxDisplay_5C1130.field_C_buffer_index)
        {
            // Move to the lower buffer if not using the top buffer
            clipRect.y += 256;
        }

        Prim_PrimClipper* pClipper = &field_20_clippers[gPsxDisplay_5C1130.field_C_buffer_index];
        Init_PrimClipper_4F5B80(pClipper, &clipRect);
        OrderingTable_Add_4F8AA0(&ot[field_48_ot_layer], &pClipper->mBase);
    }

    EXPORT void dtor_416E30()
    {
        SetVTable(this, 0x5445C4);
        gObjList_drawables_5C1124->Remove_Item(this);
        BaseGameObject_dtor_4DBEC0();
    }

    EXPORT void vDtor_416E00(signed int flags)
    {
        dtor_416E30();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
    }

    EXPORT void Update_Clip_Rect_416EB0(PSX_Point xy, PSX_Point wh)
    {
        field_40_rect.x = min(xy.field_0_x, field_40_rect.x);
        field_40_rect.y = min(xy.field_2_y, field_40_rect.y);

        field_40_rect.w = max(wh.field_0_x, field_40_rect.w);
        field_40_rect.h = max(wh.field_2_y, field_40_rect.h);
    }

private:
    Prim_PrimClipper field_20_clippers[2];
    PSX_RECT field_40_rect;
    __int16 field_48_ot_layer;
};
ALIVE_ASSERT_SIZEOF(ScreenClipper, 0x4C);

void CameraSwapper::ctor_4E5000(BYTE** ppCamRes, CameraSwapEffects changeEffect, __int16 xpos, __int16 ypos)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4
    field_4E_xpos_converted = (40 * xpos) / 23;
    field_50_ypos_converted = ypos;
    Init_4E50C0(ppCamRes, changeEffect);
}

void CameraSwapper::dtor_4E5790()
{
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4

    sNum_CamSwappers_5C1B66--;

    if (field_34_pSubObject)
    {
        field_34_pSubObject->field_6_flags.Set(BaseGameObject::eDead);
    }

    if (sMap_word_5C311C)
    {
        gMap_5C3030.sub_480740(0);
        sMap_word_5C311C = 0;
    }

    BackgroundMusic::Play_4CB030();
    MusicController::EnableMusic_47FE10(1);
    Start_Sounds_For_Objects_In_Near_Cameras_4CBB60();
    BaseGameObject_dtor_4DBEC0();
}

void CameraSwapper::vdtor_4E4D90(signed int flags)
{
    dtor_4E5790();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void CameraSwapper::Init_4E50C0(BYTE** ppCamRes, CameraSwapEffects changeEffect)
{
    NOT_IMPLEMENTED();

    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap);

    field_4_typeId = Types::eCameraSwapper;

    field_34_pSubObject = nullptr;

    if (changeEffect == CameraSwapEffects::eEffect5_1_FMV || changeEffect == CameraSwapEffects::eEffect9_2_FMV || changeEffect == CameraSwapEffects::eEffect10_3_FMV)
    {
        field_30_ppCamRes = ppCamRes;
    }
    else
    {
        pScreenManager_5BB5F4->DecompressToVRam_40EF60(reinterpret_cast<WORD**>(ppCamRes));
    }

    sNum_CamSwappers_5C1B66++;

    if (sNum_CamSwappers_5C1B66 != 1)
    {
        // TODO: Missing flag changes

        // There can only be 1 active at a time
        return;
    }
    field_38_changeEffect = changeEffect;
    int v7 = 0;
    switch (changeEffect)
    {
    case CameraSwapEffects::eEffect0_InstantChange:
        pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(0, 0, 640, 240);
        field_6_flags.Set(BaseGameObject::eDead);
        field_34_pSubObject = nullptr;
        return;

    case CameraSwapEffects::eEffect1_LeftToRight:
        // TODO
        field_56 = 80;

        field_34_pSubObject = alive_new<ScreenClipper>();

       
        break;

        // TODO: Missing cases for other effects
    }

}

void CameraSwapper::VDestructor(signed int flags)
{
    vdtor_4E4D90(flags);
}
