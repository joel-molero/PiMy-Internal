#include "includes.h"

void Hack::Init() {
	client = (uintptr_t)GetModuleHandle("client.dll");
	engine = (uintptr_t)GetModuleHandle("engine.dll");
	entList = (EntList*)(client + dwEntityList);
	localEnt = entList->ents[0].ent; //(*entList).ents[0]
	getLocalPlayer = (uintptr_t*)(client + dwLocalPlayer);
	clientState = (uintptr_t*)(engine + dwClientState);
	int count = 0;
	//enemies_list->enemieentity = {};
	for (int i = 1; i < 32; i++) {
		Ent* curEnt = entList->ents[i].ent;
		if (!CheckValidEntStart(curEnt))
			continue;
		else
		{
			count++;
		}
	}
	enemies_list.enemieentity = new Ent*[count];
	enemies_list.pos = new int[count];
	enemies_list.rage = new bool[count];
	enemies_list.countPlayers = count;

	count = 0;
	for (int i = 1; i < 32; i++) {
		Ent* curEnt = entList->ents[i].ent;
		if (!CheckValidEntStart(curEnt))
			continue;
		else
		{
			enemies_list.enemieentity[count] = curEnt;
			enemies_list.pos[count] = count;
			enemies_list.rage[count] = false;
			count++;
		}
	}
	//enemies_list.countPlayers = count;
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

bool Hack::CheckValidEntStart(Ent* ent) {
	if (ent == nullptr)
		return false;
	if (ent == localEnt)
		return false;
	if (ent->iHealth <= 0)
		return false;
	if (ent->iTeamNum == localEnt->iTeamNum)
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

Vec3 Hack::GetMyPos() {
	uintptr_t MyBaseBoneMatrix = (uintptr_t)(*getLocalPlayer + m_dwBoneMatrix);
	Vec3 headPos;
	headPos.x = *(float*)(MyBaseBoneMatrix + 0x30 * 8 + 0x0C);
	headPos.y = *(float*)(MyBaseBoneMatrix + 0x30 * 8 + 0x1C);
	headPos.z = *(float*)(MyBaseBoneMatrix + 0x30 * 8 + 0x2C);

	return headPos;
}

Vec3 Hack::GetEnemyVel(Ent* ent) {
	Vec3 enemyVel = ent->vecVelocity;

	return enemyVel;
}

Vec3* Hack::GetCurrentAngles() {
	return (Vec3*)(*clientState+dwClientState_ViewAngles);
}

int Hack::FindClosestEnemyToCrosshair() {

	Vec2 enemyPos;
	Vec3 enemyHead;
	uintptr_t EnemieBoneMatrix;
	double minDistance = 999999;
	float distance;
	int position = -1;


	for (int i = 0; i < enemies_list.countPlayers; i++)
	{
		if (!CheckValidEnt(enemies_list.enemieentity[i]))
			continue;
		//EnemieBoneMatrix = (uintptr_t)(enemies_list.enemieentity[i] + m_dwBoneMatrix);
		//vel = GetEnemyVel(enemies_list.enemieentity[i]); ??? por que???
		WorldToScreen( GetBonePos(enemies_list.enemieentity[i], 8), enemyPos );

		distance = (((windowWidth / 2) - enemyPos.x) * ((windowWidth / 2) - enemyPos.x)) + (((windowHeight / 2) - enemyPos.y) * ((windowHeight / 2) - enemyPos.y));

		if (distance < minDistance)
		{
			enemies_list.closestEntity = enemies_list.enemieentity[i];
			minDistance = distance;
			position = i;
		}

	}
	return position;
	
}

Vec3* Hack::GetOrigin() {
	return (Vec3*)(*getLocalPlayer + m_vecOrigin);
}

Vec3* Hack::GetViewOffset() {
	return (Vec3*)(*getLocalPlayer + m_vecViewOffset);
}

Vec3* Hack::GetPunchAngle() {
	return (Vec3*)(*getLocalPlayer + m_aimPunchAngle);
}


int Hack::FindClosestEnemy(Vec3* final) {
	Vec3 enemiesHead;
	Vec3 myHead;
	Vec3 substract;
	Vec3 aids;
	double distance;
	double twodDistance;
	int position = -1;
	uintptr_t EnemieBoneMatrix;
	Vec3 vel;


	double minDistance = 999999;

	Vec3 headPos = GetMyPos();

	for (int i = 0; i < enemies_list.countPlayers; i++)
	{
		if (!CheckValidEnt(enemies_list.enemieentity[i]))
			continue;
		vel = GetEnemyVel(enemies_list.enemieentity[i]);
		
		//vel = (Vec3)(enemies_list.enemieentity[i] + m_vecVelocity);

		enemiesHead = GetBonePos(enemies_list.enemieentity[i], 8);

		substract.x = headPos.x - enemiesHead.x;
		substract.y = headPos.y - enemiesHead.y;
		substract.z = headPos.z - enemiesHead.z;

		distance = sqrt(pow(substract.x, 2) + pow(substract.y, 2) + pow(substract.z, 2));
		twodDistance = sqrt(pow(substract.x, 2) + pow(substract.y, 2));

		if (distance > 320)
		{
			enemiesHead.x += ((vel.x) / (distance / 10));
			enemiesHead.y += ((vel.y) / (distance / 10));
			enemiesHead.z += ((vel.z) / (distance / 10));
		}
		else
		{
			enemiesHead.x += ((vel.x) / distance);
			enemiesHead.y += ((vel.y) / distance);
			enemiesHead.z += ((vel.z) / distance);

		}
		substract.x = headPos.x - enemiesHead.x;
		substract.y = headPos.y - enemiesHead.y;
		substract.z = headPos.z - enemiesHead.z;
	
		aids.x = substract.x;
		aids.y = substract.y;
		aids.z = substract.z;
		 
		Vec3 angles;
		Vec3 punch;
		Vec3 currentAngles = { currentAngles.x = 0, currentAngles.y = 0, currentAngles.z = 0 };
		//Vec3 *currentAngles;

		float hypo = sqrt(pow(aids.x, 2) + pow(aids.y, 2));

		//currentAngles = GetCurrentAngles(clientState);
		//Vec3* currentAngles = (Vec3*)(clientState + dwClientState_ViewAngles);

		angles.x = asinf(aids.z / hypo) * (180.0f / PI);
		angles.y = atanf(aids.y / aids.x) * (180.0f / PI) + !((*(DWORD*)&aids.x) >> 31 & 1) * 180.0f;
		angles.z = 0.0f;

		if (angles.x == NAN || isnan(angles.x))
			return -1;

		if (angles.y >= 180 && angles.y < 270) {
			angles.y -= 360;
		}

		if (((currentAngles).y > 90) && (angles.y < -2))
		{
			Sleep(15);
			angles.y += 20;
		}

		if (((currentAngles).y < -90) && (angles.y > 2))
		{
			Sleep(15);
			angles.y -= 20;
		}

		float cdist = abs(angles.y - currentAngles.y);

		if (cdist < minDistance)
		{
			enemies_list.closestEntity = enemies_list.enemieentity[i];
			minDistance = cdist;
			position = i;
			final->x = substract.x;
			final->y = substract.y;
			final->z = substract.z;

		}

	}
	return position;
}

void Hack::AimBot(int position) {
	

	Vec3 origin = *GetOrigin();
	Vec3 viewOffset = *GetViewOffset();
	Vec3* myPos = &(origin + viewOffset);
	Vec3* punch = GetPunchAngle();

	static Vec3* viewAngles = GetCurrentAngles();
	Vec3 enemyPos = GetBonePos(enemies_list.enemieentity[position], 8);
	
	Vec3 deltaVec(enemyPos.x - myPos->x, enemyPos.y - myPos->y, enemyPos.z - myPos->z);

	float distance = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);
	
	float pitch = -asin(deltaVec.z / distance) * (180 / PI);
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);
	float z = 0;

	pitch -= punch->x*2.0;
	yaw -= punch->y*2.0;
	z -= punch->z*2.0;


	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180)
	{
		viewAngles->x = pitch;
		viewAngles->y = yaw;
		viewAngles->z = z;
	}
}