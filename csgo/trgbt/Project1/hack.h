#pragma once

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#define Cabeza 8
#define Cuello 7
#define Pelvis 0
#define HombroD 39
#define CodoD 40
#define ManoD 41
#define HombroI 11
#define CodoI 12
#define ManoI 13
#define RodillaD 80
#define PieD 81
#define RodillaI 71
#define PieI 72


#define Cabeza_i 0
#define Cuello_i 1
#define Pelvis_i 2
#define HombroD_i 3
#define CodoD_i 4
#define ManoD_i 5
#define HombroI_i 6
#define CodoI_i 7
#define ManoI_i 8
#define RodillaD_i 9
#define PieD_i 10
#define RodillaI_i 11
#define PieI_i 12

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

		DEFINE_MEMBER_N(int, boneMatrix, 0x26A8);

		DEFINE_MEMBER_N(int, ArmorValue, 0x117CC);

		DEFINE_MEMBER_N(Vec3, aimPunchAngle, 0x303C);
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
	uintptr_t* getLocalPlayer;
	//uintptr_t* player;
	Ent* localEnt;
	EntList* entList;

	ID3DXLine* LineL;

	Vec2 crosshair2D;
	int crosshairSizeL = 44;
	int crosshairSizeR = 44;
	int crosshairSizeT = 44;
	int crosshairSizeB = 44;

	float viewMatrix[16];

	int Bones[13];

	void Bunny();
	void Init();
	void Update();
	bool CheckValidEnt(Ent* ent);
	bool WorldToScreen(Vec3 pos, Vec2& screen);
	Vec3 GetBonePos(Ent* ent, int bone);

};