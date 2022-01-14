#pragma once

#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <assert.h>
#include <cmath>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>

#include<string>
#include<string.h>

#pragma comment(lib, "d3d9.lib");

#pragma comment(lib, "d3dx9.lib");

#define SEGMENTS 7
#define LAYERS 4

#include "dx.h"
#include "hook.h"
#include "drawing.h"
#include "hack.h"
