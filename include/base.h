#pragma once

#include "configuration.h"
#include "platform.h"
#include "types.h"

#define NON_COPYABLE_CLASS(name)								\
				name(const name&) = delete;							\
				name& operator =(const name&) = delete;	\

#define NON_MOVABLE_CLASS(name)							\
				name(name&&) = delete;							\
				name& operator =(name&&) = delete;	\

#define C_PTR_CAST(type, var) ((type*)var)

#define CAST_PTR(type, var) static_cast<type*>(var)

#define TRY_CAST_PTR(type, var) dynamic_cast<type*>(var)