#include "includes.h"

void Hack::Init() {
	client = (uintptr_t)GetModuleHandle("client.dll");
	engine = (uintptr_t)GetModuleHandle("engine.dll");
	entList = (EntList*)(client + dwEntityList);
	localEnt = entList->ents[0].ent; //(*entList).ents[0]
	getLocalPlayer = (uintptr_t*)(client + dwLocalPlayer);

	Bones_Orders[0].Cabeza = 8;
	Bones_Orders[0].Cuello = 7;
	Bones_Orders[0].Pelvis = 0;
	Bones_Orders[0].HombroD = 38;
	Bones_Orders[0].CodoD = 39;
	Bones_Orders[0].ManoD = 40;
	Bones_Orders[0].HombroI = 10;
	Bones_Orders[0].CodoI = 11;
	Bones_Orders[0].ManoI = 12;
	Bones_Orders[0].RodillaD = 73;
	Bones_Orders[0].PieD = 74;
	Bones_Orders[0].RodillaI = 66;
	Bones_Orders[0].PieI = 67;

	Bones_Orders[1].Cabeza = 8;
	Bones_Orders[1].Cuello = 7;
	Bones_Orders[1].Pelvis = 0;
	Bones_Orders[1].HombroD = 39;
	Bones_Orders[1].CodoD = 40;
	Bones_Orders[1].ManoD = 41;
	Bones_Orders[1].HombroI = 11;
	Bones_Orders[1].CodoI = 12;
	Bones_Orders[1].ManoI = 13;
	Bones_Orders[1].RodillaD = 80;
	Bones_Orders[1].PieD = 81;
	Bones_Orders[1].RodillaI = 71;
	Bones_Orders[1].PieI = 72;

	Bones_Orders[2].Cabeza = 8;
	Bones_Orders[2].Cuello = 7;
	Bones_Orders[2].Pelvis = 0;
	Bones_Orders[2].HombroD = 39;
	Bones_Orders[2].CodoD = 40;
	Bones_Orders[2].ManoD = 41;
	Bones_Orders[2].HombroI = 11;
	Bones_Orders[2].CodoI = 12;
	Bones_Orders[2].ManoI = 13;
	Bones_Orders[2].RodillaD = 80;
	Bones_Orders[2].PieD = 81;
	Bones_Orders[2].RodillaI = 72;
	Bones_Orders[2].PieI = 73;

	Bones_Orders[3].Cabeza = 8;
	Bones_Orders[3].Cuello = 7;
	Bones_Orders[3].Pelvis = 0;
	Bones_Orders[3].HombroD = 39;
	Bones_Orders[3].CodoD = 40;
	Bones_Orders[3].ManoD = 41;
	Bones_Orders[3].HombroI = 11;
	Bones_Orders[3].CodoI = 12;
	Bones_Orders[3].ManoI = 13;
	Bones_Orders[3].RodillaD = 81;
	Bones_Orders[3].PieD = 82;
	Bones_Orders[3].RodillaI = 72;
	Bones_Orders[3].PieI = 73;

	Bones_Orders[4].Cabeza = 8;
	Bones_Orders[4].Cuello = 7;
	Bones_Orders[4].Pelvis = 0;
	Bones_Orders[4].HombroD = 39;
	Bones_Orders[4].CodoD = 40;
	Bones_Orders[4].ManoD = 41;
	Bones_Orders[4].HombroI = 11;
	Bones_Orders[4].CodoI = 12;
	Bones_Orders[4].ManoI = 13;
	Bones_Orders[4].RodillaD = 82;
	Bones_Orders[4].PieD = 83;
	Bones_Orders[4].RodillaI = 73;
	Bones_Orders[4].PieI = 74;

	Bones_Orders[5].Cabeza = 8;
	Bones_Orders[5].Cuello = 7;
	Bones_Orders[5].Pelvis = 0;
	Bones_Orders[5].HombroD = 38;
	Bones_Orders[5].CodoD = 39;
	Bones_Orders[5].ManoD = 40;
	Bones_Orders[5].HombroI = 10;
	Bones_Orders[5].CodoI = 11;
	Bones_Orders[5].ManoI = 12;
	Bones_Orders[5].RodillaD = 76;
	Bones_Orders[5].PieD = 77;
	Bones_Orders[5].RodillaI = 69;
	Bones_Orders[5].PieI = 70;

	Bones_Orders[6].Cabeza = 8;
	Bones_Orders[6].Cuello = 7;
	Bones_Orders[6].Pelvis = 0;
	Bones_Orders[6].HombroD = 38;
	Bones_Orders[6].CodoD = 39;
	Bones_Orders[6].ManoD = 40;
	Bones_Orders[6].HombroI = 10;
	Bones_Orders[6].CodoI = 11;
	Bones_Orders[6].ManoI = 12;
	Bones_Orders[6].RodillaD = 74;
	Bones_Orders[6].PieD = 75;
	Bones_Orders[6].RodillaI = 67;
	Bones_Orders[6].PieI = 68;

	Bones_Orders[7].Cabeza = 8;
	Bones_Orders[7].Cuello = 7;
	Bones_Orders[7].Pelvis = 0;
	Bones_Orders[7].HombroD = 39;
	Bones_Orders[7].CodoD = 40;
	Bones_Orders[7].ManoD = 41;
	Bones_Orders[7].HombroI = 11;
	Bones_Orders[7].CodoI = 12;
	Bones_Orders[7].ManoI = 13;
	Bones_Orders[7].RodillaD = 75;
	Bones_Orders[7].PieD = 76;
	Bones_Orders[7].RodillaI = 68;
	Bones_Orders[7].PieI = 69;

	Bones_Orders[8].Cabeza = 8;
	Bones_Orders[8].Cuello = 7;
	Bones_Orders[8].Pelvis = 0;
	Bones_Orders[8].HombroD = 39;
	Bones_Orders[8].CodoD = 40;
	Bones_Orders[8].ManoD = 41;
	Bones_Orders[8].HombroI = 11;
	Bones_Orders[8].CodoI = 12;
	Bones_Orders[8].ManoI = 13;
	Bones_Orders[8].RodillaD = 74;
	Bones_Orders[8].PieD = 75;
	Bones_Orders[8].RodillaI = 67;
	Bones_Orders[8].PieI = 68;

	Model_names[0].Name = "ctm_fbi";
	Model_names[0].Bone_Distribution = &Bones_Orders[1];

	Model_names[1].Name = "ctm_gig";
	Model_names[1].Bone_Distribution = &Bones_Orders[5];

	Model_names[1].Name = "ctm_gsg";
	Model_names[1].Bone_Distribution = &Bones_Orders[5];

	Model_names[1].Name = "ctm_idf";
	Model_names[1].Bone_Distribution = &Bones_Orders[5];

	Model_names[1].Name = "ctm_sas";
	Model_names[1].Bone_Distribution = &Bones_Orders[4];

	Model_names[1].Name = "ctm_st6";
	Model_names[1].Bone_Distribution = &Bones_Orders[5];

	Model_names[1].Name = "ctm_swa";
	Model_names[1].Bone_Distribution = &Bones_Orders[2];

	Model_names[1].Name = "tm_anar";
	Model_names[1].Bone_Distribution = &Bones_Orders[0];

	Model_names[1].Name = "tm_balk";
	Model_names[1].Bone_Distribution = &Bones_Orders[0];

	Model_names[1].Name = "tm_leet";
	Model_names[1].Bone_Distribution = &Bones_Orders[8];

	Model_names[1].Name = "tm_phoe";
	Model_names[1].Bone_Distribution = &Bones_Orders[0];

	Model_names[1].Name = "tm_pira";
	Model_names[1].Bone_Distribution = &Bones_Orders[0];

	Model_names[1].Name = "tm_prof";
	Model_names[1].Bone_Distribution = &Bones_Orders[0];

	Model_names[1].Name = "tm_sepa";
	Model_names[1].Bone_Distribution = &Bones_Orders[0];

	/*int aux [13]  = { Cabeza, Cuello, Pelvis, HombroD, CodoD, ManoD, HombroI, CodoI, ManoI, RodillaD, PieD, RodillaI, PieI };

	for (int i = 0; i < 13; i++)
	{
		Bones[i] = aux[i];
	}*/
}

void Hack::Update() {
	memcpy(&viewMatrix, (PBYTE*)(client + dwViewMatrix), sizeof(viewMatrix));
}

void Hack::Bunny() {
	int *flag = (int*)(*getLocalPlayer + m_fFlags);
	if (*flag == 257)
	{
		INPUT inputs[2] = {};
		inputs[0].type = INPUT_KEYBOARD;
		inputs[0].ki.time = 0;
		inputs[0].ki.wVk = 0;
		inputs[0].ki.dwExtraInfo = 0;
		inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;
		inputs[0].ki.wScan = 0x1B;

		inputs[1].type = INPUT_KEYBOARD;
		inputs[1].ki.time = 0;
		inputs[1].ki.wVk = 0;
		inputs[1].ki.dwExtraInfo = 0;
		inputs[1].ki.wScan = 0x1B;
		inputs[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;



		UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		Sleep(50);
	}



}

bool Hack::CheckValidEnt(Ent* ent) {
	if (ent == nullptr)
		return false;
	if (ent == localEnt)
		return false;
	if (ent->iHealth <= 0)
		return false;
	if (ent->isDormant)
		return false;
	return true;
}

bool Hack::WorldToScreen(Vec3 pos, Vec2& screen) {
	Vec4 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

Vec3 Hack::GetBonePos(Ent* ent, int bone) {
	uintptr_t bonePtr = ent->boneMatrix;
	Vec3 bonePos;
	bonePos.x = *(float*)(bonePtr + 0x30 * bone + 0x0C);
	bonePos.y = *(float*)(bonePtr + 0x30 * bone + 0x1C);
	bonePos.z = *(float*)(bonePtr + 0x30 * bone + 0x2C);
	return bonePos;
}