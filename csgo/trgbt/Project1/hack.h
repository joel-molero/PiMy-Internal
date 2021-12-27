#pragma once

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

struct Vec2 {
	float x, y;
};
struct Vec3 {
	float x, y, z;
};
struct Vec4 {
	float x, y, z, w;
};

class Ent {
public:
	union {
		DEFINE_MEMBER_N(bool, isDormant, 0xED);

		DEFINE_MEMBER_N(int, iHealth, 0x100);

		DEFINE_MEMBER_N(Vec3, vecOrigin, 0x138);

		DEFINE_MEMBER_N(int, iTeamNum, 0xF4);
	};
};

class EntListObj {
public:
	struct Ent* ent;
	char padding[12];
};

class EntList {
public:
	EntListObj ents[32];
};

class Hack {
public:
	uintptr_t dwEntityList = 0x4DCFA94;
	uintptr_t dwViewMatrix = 0x4DC1394;
	uintptr_t dwLocalPlayer = 0xDB458C;
	uintptr_t m_fFlags = 0x104;

	uintptr_t engine;
	uintptr_t client;
	uintptr_t getLocalPlayer;
	//uintptr_t* player;
	Ent* localEnt;
	EntList* entList;

	float viewMatrix[16];

	void Bunny();
	void Init();
	void Update();
	bool CheckValidEnt(Ent* ent);
	bool WorldToScreen(Vec3 pos, Vec2& screen);

};