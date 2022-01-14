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
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;
//std::atomic<bool> huha = false;

void Trigger()
{
    int CrosshairID = *(int*)(*hack->getLocalPlayer + m_iCrosshairId);
    int CrosshairTeam = hack->entList->ents[CrosshairID - 1].ent->iTeamNum;
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
    if (hack->menu)
    {
        hack->UpdateMenu();
    }
    //DrawFilledRect(10, 10, XSizePressButton, YSizePressButton, D3DCOLOR_ARGB(255, 105, 105, 105));

    for (int i = 0; i < hack->enemies_list.countPlayers; i++) {
        Vec2 Coords_bones[13];
        Ent* curEnt = hack->entList->ents[hack->enemies_list.pos[i]].ent;
        if (!hack->CheckValidEnt(curEnt))
            continue;


        uintptr_t* aux = (uintptr_t*)((uintptr_t)curEnt + m_pStudioHdr);
        uintptr_t* aux2 = (uintptr_t*)(*aux);
        studiohdr_t* studio_hdr = (studiohdr_t*)(*aux2);
        
        
        if (!(curEnt->iTeamNum == hack->localEnt->iTeamNum))
        {
            /*
            for (int i = 0; i < studio_hdr->hitbox_count; i++)
            {
                mstudiohitboxset_t* mstudiohitbox_set = studio_hdr->pHitboxSet(i);

                for (int j = 0; j < mstudiohitbox_set->numhitboxes; j++)
                {
                    mstudiobbox_t* mstudio_bbox = mstudiohitbox_set->pHitbox(j);

                    uintptr_t* aux = (uintptr_t*)(((uintptr_t)curEnt->boneMatrix) + 0x30 * ((uintptr_t)mstudio_bbox->bone));
                    matrix3x4_t* bonematrix1 = (matrix3x4_t*)(aux);
                    Vec3 vmin, vmax;
                    VectorTransform_sdk(mstudio_bbox->bbmin, *bonematrix1, vmin);
                    VectorTransform_sdk(mstudio_bbox->bbmax, *bonematrix1, vmax);

                    if (mstudio_bbox->radius == -1)
                    {
                        DrawCube(vmax, vmin, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
                    }
                    else
                    {
                        Vec3 vmin_3(vmin);
                        Vec3 vmax_3(vmax);
                        DrawCapsule(&vmin_3, &vmax_3, mstudio_bbox->radius, 2, D3DCOLOR_ARGB(255, 255, 0, 0), hack->halfSphere0, hack->halfSphere1);
                    }
                }
            }
            */

            if (hack->wallhack)
            {

                for (int j = 0; j < studio_hdr->bone_count; j++)
                {
                    mstudiobone_t* papyrusBone = studio_hdr->pBone(j);

                    if ((papyrusBone->flags & BONE_USED_BY_HITBOX) && (papyrusBone->parent != -1))
                    {
                        Vec3 childSansBone3D = hack->GetBonePos(curEnt, j);
                        //Vec3 childSansBone3D(papyrusBone->poseToBone[0][3], papyrusBone->poseToBone[1][3], papyrusBone->poseToBone[2][3]);
                        //Vec3 childSansBone3D(papyrusBone->pos.x, papyrusBone->pos.y, papyrusBone->pos.z);
                        //mstudiobone_t* fatherAlphysBone_t = studio_hdr->pBone(papyrusBone->parent);

                        //Vec3 fatherAlphysBone3D(fatherAlphysBone_t->poseToBone[0][3], fatherAlphysBone_t->poseToBone[1][3], fatherAlphysBone_t->poseToBone[2][3]);
                        //Vec3 fatherAlphysBone3D(fatherAlphysBone_t->pos.x, fatherAlphysBone_t->pos.y, fatherAlphysBone_t->pos.z);
                        Vec3 fatherAlphysBone3D = hack->GetBonePos(curEnt, papyrusBone->parent);

                        Vec2 fatherAlphysBone2D, childSansBone2D;
                        if (!(hack->WorldToScreen(fatherAlphysBone3D, fatherAlphysBone2D)) || !(hack->WorldToScreen(childSansBone3D, childSansBone2D)))
                        {
                            continue;
                        }
                        DrawLine(fatherAlphysBone2D, childSansBone2D, 2, global_colors[hack->enemies_list.colors[i]]);

                    }
                }
            }
        }
        
        if (hack->lines_behind)
        {
            D3DCOLOR color;

            if (curEnt->iTeamNum == hack->localEnt->iTeamNum)
                color = D3DCOLOR_ARGB(255, 0, 255, 0);
            else
                color = D3DCOLOR_ARGB(255, 255, 0, 0);

            Vec2 entPos2D;
            /*
            bool lodibuja = true;
            for (int i = 0; i < 13; i++)
            {
                if (!(hack->WorldToScreen(hack->GetBonePos(curEnt, hack->Bones[i]), Coords_bones[i])))
                {
                    lodibuja = false;
                    break;
                }
            }
            if (lodibuja)
            {
                DrawLine(Coords_bones[Cabeza_i], Coords_bones[Cuello_i], 10, color);
                DrawLine(Coords_bones[Pelvis_i], Coords_bones[Cuello_i], 10, color);
                DrawLine(Coords_bones[HombroD_i], Coords_bones[Cuello_i], 10, color);
                DrawLine(Coords_bones[HombroI_i], Coords_bones[Cuello_i], 10, color);
                DrawLine(Coords_bones[HombroD_i], Coords_bones[CodoD_i], 10, color);
                DrawLine(Coords_bones[HombroI_i], Coords_bones[CodoI_i], 10, color);
                DrawLine(Coords_bones[ManoD_i], Coords_bones[CodoD_i], 10, color);
                DrawLine(Coords_bones[ManoI_i], Coords_bones[CodoI_i], 10, color);
                DrawLine(Coords_bones[Pelvis_i], Coords_bones[RodillaI_i], 10, color);
                DrawLine(Coords_bones[Pelvis_i], Coords_bones[RodillaD_i], 10, color);
                DrawLine(Coords_bones[PieI_i], Coords_bones[RodillaI_i], 10, color);
                DrawLine(Coords_bones[PieD_i], Coords_bones[RodillaD_i], 10, color);
            }
            */

            if (hack->WorldToScreen(curEnt->vecOrigin, entPos2D))
                DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, color);
        }
    }
    
    //aqui se dibuja
    //DrawFilledRect(25, 25, 100, 100, D3DCOLOR_ARGB(255, 255, 255, 255));
    //DrawFilledRect(windowWidth / 2 - 2, windowHeight / 2 - 2, 4, 4, D3DCOLOR_ARGB(255, 255, 255, 255));
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

DWORD WINAPI MainThread(HMODULE hModule)
{

    if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
        memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);
    
        oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
    }

    hack = new Hack();
    hack->Init();

    /*
    //Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Welcome\n";

    moduleBase = (DWORD)GetModuleHandle("client.dll");
    */

    
    while (true)
    {
        hack->Update();
        int closestEnemy = hack->FindClosestEnemyToCrosshair();
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        if (hack->aim)
        {
            if (GetAsyncKeyState(VK_MENU /*alt key*/))
            {
                int position = hack->FindClosestEnemyToCrosshair();
                if (position != -1)
                    hack->AimBot(position);
            }
        }

        if (GetAsyncKeyState(VK_F2 /*alt key*/) & 1)
        {
            hack->trigger = !hack->trigger;
        }
 
        if (hack->trigger) {
            
            std::thread t1(Trigger);
            //hack->Trigger();
            
            t1.join();
        }
        
        if (hack->bunny)
        {
            if (GetAsyncKeyState(VK_SPACE))
            {
                hack->Bunny();
            }
        }

        if (GetAsyncKeyState(VK_ESCAPE) & 1)
        {
            hack->menu = !hack->menu;
        }

        if (GetAsyncKeyState(VK_F1))
        {
            hack->EntityListReload();
        }
    }
    Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

    //fclose(f);
    //FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    Sleep(100);
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
        break;
    }
    return TRUE;
}