#include "stdafx_ao.h"
#include "Function.hpp"
#include "Particle.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"

START_NS_AO

Particle* CC New_DestroyOrCreateObject_Particle_419D00(FP xpos, FP ypos, FP scale)
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kDeathFlareResID_349, 1, 0);

    auto pParticle = ao_new<Particle>();
    if (!pParticle)
    {
        return nullptr;
    }

    pParticle->ctor_478880(xpos, ypos, 6284, 70, 43, ppRes);
    pParticle->field_10_anim.field_B_render_mode = 1;
    pParticle->field_BC_sprite_scale = FP_FromInteger(2) * scale;

    if (scale == FP_FromInteger(1))
    {
        pParticle->field_10_anim.field_C_layer = 39;
    }
    else
    {
        pParticle->field_10_anim.field_C_layer = 17;
    }

    pParticle->field_CC_bApplyShadows &= ~1u;

    return pParticle;
}

void CC New_Particles_419A80(FP /*xpos*/, FP /*ypos*/, FP /*scale*/, __int16 /*count*/, __int16 /*type*/)
{
    // looks like New_Smoke_Particles
    NOT_IMPLEMENTED();
}

void CC New_Chant_Particle_4198E0(FP xpos, FP ypos, FP scale, __int16 layer)
{
    auto pParticle = ao_new<Particle>();
    if (pParticle)
    {
        pParticle->ctor_478880(xpos, ypos, 1492, 38, 21, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kOmmflareResID_312, 1, 0));
        pParticle->field_CC_bApplyShadows &= ~1u;
        pParticle->field_10_anim.field_B_render_mode = 1;

        if (layer)
        {
            pParticle->field_10_anim.field_C_layer = layer;
        }
        else if (scale == FP_FromInteger(1))
        {
            pParticle->field_10_anim.field_C_layer = 36;
        }
        else
        {
            pParticle->field_10_anim.field_C_layer = 17;
        }

        pParticle->field_BC_sprite_scale = scale;
    }
}

void CC New_Shiny_Particle_4199A0(FP xpos, FP ypos, FP scale, __int16 layer)
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kOmmflareResID_312, 1, 0);
    auto pParticle = ao_new<Particle>();
    if (pParticle)
    {
        pParticle->ctor_478880(xpos, ypos, 1492, 38, 21, ppRes);

        pParticle->field_CC_bApplyShadows &= ~1u;
        pParticle->field_10_anim.field_B_render_mode = 1;
        pParticle->field_C0_r = 100;
        pParticle->field_C2_g = 100;
        pParticle->field_C4_b = 100;

        if (layer)
        {
            pParticle->field_10_anim.field_C_layer = layer;
        }
        else if (scale == FP_FromInteger(1))
        {
            pParticle->field_10_anim.field_C_layer = 36;
        }
        else
        {
            pParticle->field_10_anim.field_C_layer = 17;
        }

        pParticle->field_BC_sprite_scale = scale;
    }
}

void CC New_ShootingZFire_Particle_419810(FP xpos, FP ypos, FP scale)
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kZflashResID_344, 1, 0);
    auto pParticle = ao_new<Particle>();
    if (pParticle)
    {
        pParticle->ctor_478880(xpos, ypos, 2348, 126, 44, ppRes);
        pParticle->field_CC_bApplyShadows &= ~1u;

        pParticle->field_C4_b = 55;
        pParticle->field_C2_g = 55;
        pParticle->field_C0_r = 55;

        pParticle->field_10_anim.field_B_render_mode = 1;

        if (scale == FP_FromInteger(1))
        {
            pParticle->field_10_anim.field_C_layer = 36;
        }
        else
        {
            pParticle->field_10_anim.field_C_layer = 17;
        }

        pParticle->field_BC_sprite_scale = scale;
    }
}

Particle* Particle::ctor_478880(FP xpos, FP ypos, int animFrameTableOffset, int maxW, int maxH, BYTE** ppAnimData)
{
    ctor_417C10();
    SetVTable(this, 0x4BCDB0);

    field_C4_b = 128;
    field_C2_g = 128;
    field_C0_r = 128;

    field_4_typeId = Types::eParticle_93;

    Animation_Init_417FD0(
        animFrameTableOffset,
        maxW,
        maxH,
        ppAnimData,
        1);

    if (field_6_flags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;
    field_E4_scale_amount = FP_FromInteger(0);

    return this;
}

BaseGameObject* Particle::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Particle::VUpdate()
{
    VUpdate_478930();
}

void Particle::VUpdate_478930()
{
    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;

    field_BC_sprite_scale += field_E4_scale_amount;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

END_NS_AO
