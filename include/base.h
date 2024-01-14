#pragma once

#include "configuration.h"
#include "platform.h"
#include "types.h"

#include "Common/Misc/CoreMiscDefines.h"

#define C_PTR_CAST(type, var) ((type*)var)

#define CAST_PTR(type, var) static_cast<type*>(var)

#define TRY_CAST_PTR(type, var) dynamic_cast<type*>(var)


#define _SF(A)   A,A+1,A+2,A+2,A+3,A+0
#define _CF(A)   A,A+2,A+1,A+2,A+0,A+3   // Ad-hoc clockwise order change

#define dPI 3.14159265359

#define NODISCARD [[nodiscard]]
#define LIKELY [[likely]]
#define UNLIKELY [[unlikely]]
#define FORCEINLINE __forceinline	

