#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class BaseGameObject;

enum Event : __int16
{
    kEvent_0 = 0,
    kEvent_1 = 1,

    kEventDeathReset_4 = 4,
    kEvent_5 = 5,
    kEventResetting_6 = 6,

    kEvent_7 = 7,

    kEvent_9 = 9,
    kEvent_10 = 10,

    kEvent_14 = 14,
    kEvent_15 = 15,
    kEvent_16 = 16,

    kEventScreenShake_19 = 19,
};

EXPORT BaseGameObject* CC Event_Get_417250(__int16 eventType);

EXPORT void CC Event_Broadcast_417220(int eventType, BaseGameObject* pObject);


END_NS_AO