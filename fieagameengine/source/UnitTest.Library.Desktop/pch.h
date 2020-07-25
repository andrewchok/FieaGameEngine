#pragma once

// Standard
#include <cstdint>
#include <crtdbg.h>
#include <string>

// Microsoft Native Unit Testing Framework
#include "CppUnitTest.h"

inline std::size_t operator "" _z(unsigned long long int x)
{
    return static_cast<size_t>(x);
}

