#pragma once

#include <type_traits>

enum EForceInit
{
  ForceInit,
  ForceInitToZero
};

enum EForceStart { ForceStart };

enum ENoInit { NoInit };

#define NON_COPYABLE_CLASS(name)   \
        name(const name&) = delete;\
        name& operator=(const name&) = delete;

#define NON_MOVABLE_CLASS(name) \
        name(name&&) = delete;  \
        name& operator=(name&&) = delete;

#define INTERFACE_BODY(name)       \
        public:                    \
        name() = default;          \
        virtual ~name() = default;
