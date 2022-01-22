#include "includes.h"
#include <iostream>
#include <Windows.h>
#include <WinUser.h>

#define m_iTeamNum 0xF4
#define dwLocalPlayer 0xDB558C
#define dwEntityList 0x4DD0AB4
#define m_iCrosshairId 0x11838
#define m_fFlags 0x104
#define dwForceJump 0x527998C
#define m_pStudioHdr 0x2950
#define m_dwBoneMatrix 0x26A8

uintptr_t moduleBase;
Vec3 test;



template<typename T> T RPM(uintptr_t address) {
    try { return *(T*)address; }
    catch (...) { return T(); }
}
/*
uintptr_t getLocalPlayer() { //This will get the address to localplayer. 
    return RPM< uintptr_t>(moduleBase + dwLocalPlayer);
}

uintptr_t getPlayer(int index) {  //Each player in the game has an index.
    return RPM< uintptr_t>(moduleBase + dwEntityList + index * 0x10); //We use index times 0x10 because the distance between each player 0x10.
}

int flag(uintptr_t player) {
    return RPM<int>(player + m_fFlags);
}

int getTeam(uintptr_t player) {
    return RPM<int>(player + m_iTeamNum);
}


int getCrosshairID(uintptr_t player) {
    return RPM<int>(player + m_iCrosshairId);
}
*/
typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);
void* GetInterface(const char* dllname, const char* interfacename)
{
    tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllname), "CreateInterface");

    int returnCode = 0;
    void* interfaceWek = CreateInterface(interfacename, &returnCode);

    return interfaceWek;
}

typedef void* (__fastcall* _DrawModelExecute)(void* _this, void* edx, void* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, void* pCustomBoneToWorld);
void __fastcall hkDrawModelExecute(void* _this, void* edx, void* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, void* pCustomBoneToWorld);

void* ModelRenderVMT[25];
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
_DrawModelExecute oDrawModelExecute = nullptr;

extern LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;

IVModelInfoClient* ModelInfoClient = (IVModelInfoClient*)GetInterface("engine.dll", "VModelInfoClient004");
IVModelRender* ModelRender = (IVModelRender*)GetInterface("engine.dll", "VEngineModel016");
IMaterialSystem* MaterialSystem = (IMaterialSystem*)GetInterface("materialsystem.dll", "VMaterialSystem080");



IClientEntityList* ClientEntityList = (IClientEntityList*)GetInterface("client.dll", "VClientEntityList003");
//std::atomic<bool> huha = false;


void Trigger()
{
    int CrosshairID = *(int*)(*hack->getLocalPlayer + m_iCrosshairId);
    int CrosshairTeam = hack->entitiesE->enemyEnts[CrosshairID - 1]->iTeamNum;
    int LocalTeam = hack->localEnt->iTeamNum;

    if (CrosshairID > 0 && CrosshairID < 32 && LocalTeam != CrosshairTeam)
    {

        mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
        //std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Optional
    }

    /*
    int CrosshairID = getCrosshairID(getLocalPlayer());
    int CrosshairTeam = getTeam(getPlayer(CrosshairID - 1));
    int LocalTeam = getTeam(getLocalPlayer());

    if (CrosshairID > 0 && CrosshairID < 32 && LocalTeam != CrosshairTeam)
    {
            mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
            Sleep(100); //Optional
    }
    */
}



void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {

    if (!pDevice)
        pDevice = o_pDevice;

    //for (int i = 0; i < 5; i++)
    for (int i = 0; i < hack->enemies_list.countPlayers; i++)
    {
        Vec2 Coords_bones[13];
        Ent* curEnt = hack->enemies_list.enemieentity[i];
        if (!hack->CheckValidEnt(curEnt))
            continue;


        uintptr_t* aux = (uintptr_t*)((uintptr_t)curEnt + m_pStudioHdr);
        uintptr_t* aux2 = (uintptr_t*)(*aux);
        studiohdr_t* studio_hdr = (studiohdr_t*)(*aux2);
        
        if (!(curEnt->iTeamNum == hack->localEnt->iTeamNum))
        {
         
            for (int i = 0; i < studio_hdr->bone_count; i++)
            {
                mstudiobone_t* papyrusBone = studio_hdr->pBone(i);

                if ((papyrusBone->flags & BONE_USED_BY_HITBOX) && (papyrusBone->parent != -1))
                {
                    Vec3 childSansBone3D = hack->GetBonePos(curEnt, i);
                  
                    Vec3 fatherAlphysBone3D = hack->GetBonePos(curEnt, papyrusBone->parent);

                    Vec2 fatherAlphysBone2D, childSansBone2D;
                    if (!(hack->WorldToScreen(fatherAlphysBone3D, fatherAlphysBone2D)) || !(hack->WorldToScreen(childSansBone3D, childSansBone2D)))
                    {
                        continue;
                    }
                    DrawLine(fatherAlphysBone2D, childSansBone2D, 2, D3DCOLOR_ARGB(255, 0, 255, 0));

                }
            }
        }
        

        D3DCOLOR color;

        if (curEnt->iTeamNum == hack->localEnt->iTeamNum)
            color = D3DCOLOR_ARGB(255, 0, 255, 0);
        else
            color = D3DCOLOR_ARGB(255, 255, 0, 0);

        Vec2 entPos2D;
       
        if (hack->WorldToScreen(curEnt->vecOrigin, entPos2D))
            DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, color);
    }
    
    //aqui se dibuja
    hack->crosshair2D.x = windowWidth / 2;
    hack->crosshair2D.y = windowHeight / 2;

    Vec2 l, r, t, b;
    l = r = t = b = hack->crosshair2D;
    l.x -= hack->crosshairSizeL;
    r.x += hack->crosshairSizeR;
    b.y += hack->crosshairSizeT;
    t.y -= hack->crosshairSizeB;

    DrawLine(r, t, 2, D3DCOLOR_ARGB(115, 132, 13, 255));
    DrawLine(l, t, 2, D3DCOLOR_ARGB(115, 255, 0, 0));
    DrawLine(r, b, 2, D3DCOLOR_ARGB(115, 0, 255, 0));
    DrawLine(l, b, 2, D3DCOLOR_ARGB(115, 0, 0, 255));
    DrawLine(t, b, 2, D3DCOLOR_ARGB(165, 255, 255, 255));
    DrawLine(l, r, 2, D3DCOLOR_ARGB(165, 255, 255, 255));
    //DrawLine(t, l, 2, D3DCOLOR_ARGB(135, 32, 200, 255));
    //DrawLine(t, b, 66, D3DCOLOR_ARGB(0, 32, 20, 255));
    
    oEndScene(pDevice);
}



void __fastcall hkDrawModelExecute(void* _this, void* edx, void* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, void* pCustomBoneToWorld)
{
    if (pInfo.pModel)
    {
        // The name of the model that is currently being rendered
        //const char* modelName = ModelInfoClient->GetModelName(pInfo.pModel);
        bool isEnemy = false;
        for (int i = 0; i < hack->enemies_list.countPlayers; i++)
        {
            // Check if a player model is being rendered
            if ((Ent*)ClientEntityList->GetClientEntity(pInfo.entity_index) == hack->enemies_list.enemieentity[i]) //aqui se pueden comparar indices de pInfo.entity_index con indices de entitiesE.ents[i]
            {
                // Get the Entity pointer of the player that is being rendered
                // Forgot to mention you need ClientEntityList interface. If you don't have it you're a bad person since you didn't do the CreateInterface tutorial
                Ent* modelEnt = (Ent*)ClientEntityList->GetClientEntity(pInfo.entity_index);

                // Validity check (not dormant, alive, etc...)
                if (!hack->CheckValidEnt(modelEnt))
                {
                    oDrawModelExecute(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
                    return;
                }

                // Get the materials that we want to override this model with
                IMaterial* matInvis = MaterialSystem->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail");
                IMaterial* matVis = MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear");

                // Draw material that will be visible through walls
                if (matInvis)
                {
                    // This flag tells the game to draw the model even if it's behind a wall
                    matInvis->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

                    // Change the color of our Material (doesn't necessarily work on all materials)
                    float colorInvis[3] = { 0.74f, 0.205f, 0.205f };
                    matInvis->ColorModulate(colorInvis);
                    // Set alpha value
                    matInvis->AlphaModulate(1.f);

                    // Override the material with ours
                    ModelRender->ForcedMaterialOverride(matInvis);
                    // By calling the original function here, we can have different materials for the non-visible and visible part of the model
                    oDrawModelExecute(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
                }

                // Draw material for the part of the player model that is visible
                if (matVis)
                {
                    // Don't let the material display through walls
                    matVis->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
                    float colVis[3] = { 0.205f, 0.74f, 0.205f };
                    matVis->ColorModulate(colVis);

                    matVis->AlphaModulate(1.f);

                    ModelRender->ForcedMaterialOverride(matVis);
                    // Draw our visible material "on top" of our previous material
                    oDrawModelExecute(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
                }
                isEnemy = true;
                // Reset state
                ModelRender->ForcedMaterialOverride(nullptr);
            }
        }
        if (!isEnemy)
        {
            oDrawModelExecute(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
            return;
        }
    }
    
}

DWORD WINAPI MainThread(HMODULE hModule)
{
    
    if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
        memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);
    
        oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
    }
    /*
    if (GetModelRender(ModelRenderVMT, sizeof(ModelRenderVMT))) {
        
        
    }
    */
    hack = new Hack();
    hack->Init();
    memcpy(ModelRenderVMT, *(void***)ModelRender, sizeof(ModelRenderVMT));
    oDrawModelExecute = (_DrawModelExecute)TrampHook((char*)ModelRenderVMT[21], (char*)hkDrawModelExecute, 6);

    
   
    
    

    /*
    //Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Welcome\n";

    moduleBase = (DWORD)GetModuleHandle("client.dll");
    */

    bool trig = false;
    
    while (true)
    {
        hack->Update(ModelRenderVMT,ModelRender);
        int closestEnemy = hack->FindClosestEnemyToCrosshair();
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        if (GetAsyncKeyState(VK_MENU /*alt key*/))
        {
            int position = hack->FindClosestEnemyToCrosshair();
            if (position != -1)
                hack->AimBot(position);
        }

        if (GetAsyncKeyState(VK_F2 /*alt key*/) & 1)
        {
            trig = !trig;
        }
 
        if (trig) {
            
            std::thread t1(Trigger);
            //hack->Trigger();
            
            t1.join();
        }
        
        if (GetAsyncKeyState(VK_SPACE))
        {
            hack->Bunny();
        }


        if (GetAsyncKeyState(VK_F1))
        {
            hack->EntityListReload();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

    //fclose(f);
    //FreeConsole();
    
    FreeLibraryAndExitThread(hModule, 0);
    Sleep(4000);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
       // return TRUE;
        break;
    }
    return TRUE;
}