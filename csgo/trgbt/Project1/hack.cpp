#include "includes.h"


void Hack::EntityListReload() {
	getLocalPlayer = (uintptr_t*)(client + dwLocalPlayer);
	localEnt = (Ent*)*getLocalPlayer;

	int count = 0;
	//enemies_list->enemieentity = {};
	for (int i = 0; i < 32; i++) {
		Ent* curEnt = entList->ents[i].ent;
		if (!CheckValidEntStart(curEnt))
			continue;
		else
		{
			count++;
		}
	}
	enemies_list.enemieentity = new Ent * [count];
	enemies_list.pos = new int[count];
	enemies_list.rage = new bool[count];
	enemies_list.colors = new int[count];
	enemies_list.countPlayers = count;

	count = 0;
	for (int i = 0; i < 32; i++) {
		Ent* curEnt = entList->ents[i].ent;
		if (!CheckValidEntStart(curEnt))
			continue;
		else
		{
			enemies_list.enemieentity[count] = curEnt;
			enemies_list.pos[count] = i;
			enemies_list.rage[count] = false;
			enemies_list.colors[count] = 0;
			count++;
		}
	}

	std::vector<Button*> botons1;
	int separation = 10 + 3 * SizeColorButton + 30;

	for (int i = 0; i < enemies_list.countPlayers; i++)
	{
		botons1.emplace_back(new HumanButton((windowWidth / 2) + 10 +separation*i, YSizePressButton + 10, &enemies_list.rage[i], &enemies_list.colors[i]));
	}

	buttons[1].buttons = botons1;

	buttons[1].numbuttons = enemies_list.countPlayers;
}

void Hack::UpdateMenu()
{
	window = GetForegroundWindow();
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(window, &p);

	//si el click se ha pulsado miramos antes de nada si ha pulsado alguna pestaña para ponerla como pressed y poder drawear sus botones.
	if (GetAsyncKeyState(VK_LBUTTON) & 1)
	{

		bool pesta = false;
		for (int i = 0; i < numButtons; i++)
		{
			if (buttons[i].isMouseOn(p))
			{
				buttons[buttonpressed].pressed = false;
				buttons[i].pressed = true;
				buttonpressed = i;
				pesta = true;
				break;
			}
		}

		if (!pesta)
		{
			for (int i = 0; i < buttons[buttonpressed].numbuttons; i++)
			{
				if(buttons[buttonpressed].buttons[i]->Click(p))
				{
					break;
				}
			}

		}
	}

	//Cuadrado Principal.
	DrawFilledRect(windowWidth / 2, 0, windowWidth / 2, windowHeight, D3DCOLOR_ARGB(255, 192, 192, 192));

	for (int i = 0; i < numButtons; i++)
	{
		buttons[i].Draw(p);
	}

	buttons[buttonpressed].DrawButtons(p);


	//una vez seteado el botón que está pulsado en este momento drawearemos sus botones.

}
void Hack::InitializeMenu()
{
	//USING UNIQUE PTR HE LEIO POR INTERNE QUE MEHO PERO NIDEA DE COMO USARLLO)
	//std::vector<std::unique_ptr<Button>> botons;
	std::vector<Button*> botons;
	botons.emplace_back(new BoolButton((windowWidth / 2) + 10, YSizePressButton + 10, &trigger));
	botons.emplace_back(new BoolButton((windowWidth / 2) + 10, YSizePressButton + 20 + YSizeButton, &aim));
	botons.emplace_back(new BoolButton((windowWidth / 2) + 10, YSizePressButton + 30 + 2 * YSizeButton, &bunny));
	botons.emplace_back(new BoolButton((windowWidth / 2) + 10, YSizePressButton + 60 + 3 * YSizeButton, &lines_behind));
	botons.emplace_back(new BoolButton((windowWidth / 2) + 10, YSizePressButton + 70 + 4 * YSizeButton, &wallhack));


	std::vector<Button*> botons1;

	//botons1.emplace_back(new HumanButton((windowWidth / 2) + 10, YSizePressButton + 10, &enemies_list.rage[0], &enemies_list.colors[0]));
	/*Button* botons[5]; //= { new BoolButton() };

	botons[0] = &(new BoolButton());

	botons = new BoolButton[5];

	botons[0].set((windowWidth / 2) + 10, YSizePressButton + 10, &trigger);
	botons[1].set((windowWidth / 2) + 10, YSizePressButton + 20 + YSizeButton, &aim);
	botons[2].set((windowWidth / 2) + 10, YSizePressButton + 30 + 2*YSizeButton, &bunny);
	botons[3].set((windowWidth / 2) + 10, YSizePressButton + 60 + 3 * YSizeButton, &lines_behind);
	botons[4].set((windowWidth / 2) + 10, YSizePressButton + 70 + 4 * YSizeButton, &wallhack);

	Button* botons_tr = botons;*/
	//Button* botons_tr = new Button[5];

	//botons_tr[0] = botons[0];

	PressButton* Pest = new PressButton[3];

	std::vector<Button*> botons2;



	Pest[0].set(true, (windowWidth / 2), 0, botons, 5);
	Pest[1].set(false, (windowWidth / 2) + XSizePressButton, 0, botons1, 0);
	Pest[2].set(false, (windowWidth / 2) + 2*XSizePressButton, 0, botons2, 0);

	numButtons = 3;
	buttonpressed = 0;

	buttons = Pest;
	

}
void Hack::Init() {
	client = (uintptr_t)GetModuleHandle("client.dll");
	engine = (uintptr_t)GetModuleHandle("engine.dll");
	entList = (EntList*)(client + dwEntityList);
	getLocalPlayer = (uintptr_t*)(client + dwLocalPlayer);
	localEnt = (Ent*)*getLocalPlayer;
	clientState = (uintptr_t*)(engine + dwClientState);
	time = -1;

	//set initial values for bools;
	trigger = false;
	aim = true;
	bunny = true;
	wallhack = true;
	lines_behind = true;

	InitializeMenu();

	EntityListReload();


	menu = true;




	

	halfSphere0 = new Vec3 *[LAYERS];
	halfSphere1 = new Vec3 * [LAYERS];
	for (int i = 0; i < LAYERS; i++)
	{
		halfSphere0[i] = new Vec3[SEGMENTS + 1];
		halfSphere1[i] = new Vec3[SEGMENTS + 1];
	}
	
}

void Hack::Update() {
	memcpy(&viewMatrix, (PBYTE)(client + dwViewMatrix), sizeof(viewMatrix));
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

bool Hack::WorldToScreen(Vec3& pos) {
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

	pos.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	pos.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
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
	return (Vec3*)(*clientState + dwClientState_ViewAngles);
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
		if (!CheckValidEnt(entList->ents[enemies_list.pos[i]].ent))
			continue;
		//EnemieBoneMatrix = (uintptr_t)(enemies_list.enemieentity[i] + m_dwBoneMatrix);
		//vel = GetEnemyVel(enemies_list.enemieentity[i]); ??? por que???
		WorldToScreen(GetBonePos(entList->ents[enemies_list.pos[i]].ent, 8), enemyPos);

		distance = (((windowWidth / 2) - enemyPos.x) * ((windowWidth / 2) - enemyPos.x)) + (((windowHeight / 2) - enemyPos.y) * ((windowHeight / 2) - enemyPos.y));

		if (distance < minDistance)
		{
			enemies_list.closestEntity = entList->ents[enemies_list.pos[i]].ent;
			minDistance = distance;
			position = enemies_list.pos[i];
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

/*
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
*/
void Hack::AimBot(int position) {


	Vec3 origin = *GetOrigin();
	Vec3 viewOffset = *GetViewOffset();
	Vec3* myPos = &(origin + viewOffset);
	Vec3* punch = GetPunchAngle();

	static Vec3* viewAngles = GetCurrentAngles();
	Vec3 enemyPos = GetBonePos(entList->ents[position].ent, 8);

	Vec3 deltaVec(enemyPos.x - myPos->x, enemyPos.y - myPos->y, enemyPos.z - myPos->z);

	float distance = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);

	float pitch = -asin(deltaVec.z / distance) * (180 / PI);
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);
	float z = 0;

	pitch -= punch->x * 2.0;
	yaw -= punch->y * 2.0;
	z -= punch->z * 2.0;


	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180)
	{
		viewAngles->x = pitch;
		viewAngles->y = yaw;
		viewAngles->z = z;
	}
}


void Hack::Trigger()
{
	int CrosshairID = *(int*)(*getLocalPlayer + m_iCrosshairId);
	int CrosshairTeam = entList->ents[CrosshairID - 1].ent->iTeamNum;
	int LocalTeam = localEnt->iTeamNum;
	
	if (CrosshairID > 0 && CrosshairID < 32 && LocalTeam != CrosshairTeam)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
		//std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

}

bool BoolButton::Click(POINT p)
{
	if (isMouseOn(p))
	{
		*associated = !*associated;
		return true;
	}

	return false;
}

void BoolButton::Draw(POINT p)
{
	//Si a lo que apunta es true
	if (*associated == true)
	{
		//Hacemos recuadrito un par de pixeles mas grande verde, para que tenga como un recuadro verde.
		DrawFilledRect(x - 2, y - 2, XSizeButton + 4, YSizeButton + 4, D3DCOLOR_ARGB(255, 0, 255, 0));

	}
	else
	{
		//Sino rojo.
		DrawFilledRect(x - 2, y - 2, XSizeButton + 4, YSizeButton + 4, D3DCOLOR_ARGB(255, 255, 0, 0));
	}

	if (isMouseOn(p))
	{
		//Si el raton esta encima gris más clarito 
		DrawFilledRect(x, y, XSizeButton, YSizeButton, D3DCOLOR_ARGB(255, 220, 220, 220));

	}
	else
	{
		//Sino gris menos claro sjasjsaj.
		DrawFilledRect(x, y, XSizeButton, YSizeButton, D3DCOLOR_ARGB(255, 211, 211, 211));
	}

	//DrawText(NULL, LPCSTR(text), y, D3DCOLOR_ARGB(255, 255, 255, 255));
}
void PressButton::Draw(POINT p)
{
	if (pressed)
	{
		//si está pulsado hacemos el gris oscuro.
		DrawFilledRect(x, y, XSizePressButton, YSizePressButton, D3DCOLOR_ARGB(255, 105, 105, 105));
	}
	else
	{
		if (isMouseOn(p))
		{
			//Si el raton está encima lo ponemos un poquito gris.
			DrawFilledRect(x, y, XSizePressButton, YSizePressButton, D3DCOLOR_ARGB(255, 128, 128, 128));
		}
		else
		{
			//Sino gris claro
			DrawFilledRect(x, y, XSizePressButton, YSizePressButton, D3DCOLOR_ARGB(255, 169, 169, 169));
		}
	}
}

void ColorButton::Draw(POINT p, int num_color, bool selected)
{
	if (selected)
	{
		DrawFilledRect(x - 2, y - 2, SizeColorButton + 4, SizeColorButton + 4, D3DCOLOR_ARGB(255, 255, 255, 0));
	}

	DrawFilledRect(x, y, SizeColorButton, SizeColorButton, global_colors[num_color]);

	if (isMouseOn(p))
	{
		DrawFilledRect(x, y, SizeColorButton, SizeColorButton, D3DCOLOR_ARGB(1, 255, 255, 255));
	}

	
}
