#pragma once

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}
/*
#define Cabeza 8
#define Cuello 7
#define Pelvis 0
#define HombroD 38
#define CodoD 39
#define ManoD 40
#define HombroI 10
#define CodoI 11
#define ManoI 12
#define RodillaD 73
#define PieD 74
#define RodillaI 66
#define PieI 67
*/

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


struct Model_Name {
	const char* Name;
	Bone_Order* Bone_Distribution;
};
struct Bone_Order{
	char Cabeza;
	char Cuello;
	char Pelvis;
	char HombroD;
	char CodoD;
	char ManoD;
	char HombroI;
	char CodoI;
	char ManoI;
	char RodillaD;
	char PieD;
	char RodillaI;
	char PieI;

};

struct Vec2 {
	float x, y;
};
struct Vec3 {
	float x, y, z;
};
struct Vec4 {
	float x, y, z, w;
};

struct model_t {
	void* fnHandle;
	char szName[260];
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

	Bone_Order Bones_Orders[9];
	Model_Name Model_names[14];

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