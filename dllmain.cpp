
#include "stdafx.h"
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>
#include "mem.h"
#include "proc.h"
#include "offsets.h"
#include <math.h>
#include "Aimbot.h"
#define M_PI 3.14159265358979323846

float euclidean_distance(float x, float y) {
	return sqrtf((x * x) + (y * y));
}

bool bHealth = false, bAmmo = false, bRecoil = false, bArmor = false, bTriggerbot = false, bFly = false, bSpeed = false, bReload = false, bTurn = false, bAimbot = false;
uintptr_t modBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

ent* localPlayer = *(ent**)(modBase + 0x10F4F4);



DWORD WINAPI HackThread(HMODULE hModule)
{

	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "----------------------------------------------------" << std::endl;
	std::cout << "             Assault Cube Internal" << std::endl;
	std::cout << "----------------------------------------------------" << std::endl;
	std::cout << "" << std::endl;


	std::cout << "      [NUMPAD1] Godmode\n";

	std::cout << "" << std::endl;

	std::cout << "      [NUMPAD2] Infinite Ammo\n";

	std::cout << "" << std::endl;

	std::cout << "      [NUMPAD3] No Recoil\n";

	std::cout << "" << std::endl;

	std::cout << "      [NUMPAD4] Infinite Armor\n";

	std::cout << "" << std::endl;

	std::cout << "      [Mouse3] Triggerbot\n";

	std::cout << "" << std::endl;

	std::cout << "      [NUMPAD5] FlyHack\n";

	std::cout << "" << std::endl;

	std::cout << "      [NUMPAD6] SpeedHack\n";

	std::cout << "" << std::endl;

	std::cout << "      [NUMPAD7] Rate Of Fire\n";

	std::cout << "" << std::endl;
	std::cout << "      [DELETE] EXIT\n";



	GetEntCrosshair = (tGetEntCrosshair)(modBase + 0x607c0);

	
	float closest_player = -1.0f;
	float closest_yaw = 0.0f;
	float closest_pitch = 0.0f;


	// Theres multiple ways to make a triggerbot, this game has a attack bool, so we can manipulate that, but in case this game didn't, this is how we'd do it:
	//  INPUT input = { 0 }; SendInput takes an array of input events, which allows you to send multiple events
	//  
	//	input.type = INPUT_MOUSE;
	//	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;     <----- we can use this send a mouse1 input, it would just bAttack
	//	SendInput(1, &input, sizeof(INPUT)); 
	//   





	// Dont be retarded like I was for the first 2 hours, dont forget about numlock
	while (true)
	{
		if (GetAsyncKeyState(VK_DELETE) & 1)
		{
			break;
		}

		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
			bHealth = !bHealth;

		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bAmmo = !bAmmo;
		}

		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bRecoil = !bRecoil;

			if (bRecoil) //replace with code that does nothing
			{
				mem::Nop((BYTE*)(modBase + 0x63786), 10);
			}

			else  //rewrite the bytes
			{

				mem::Patch((BYTE*)(modBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD4) & 1)
		{
			bArmor = !bArmor;
		}

		if (GetAsyncKeyState(VK_MBUTTON) & 1)
		{
			bTriggerbot = !bTriggerbot;
		}

		if (GetAsyncKeyState(VK_NUMPAD5) & 1)
		{
			bFly = !bFly;
		}

		if (GetAsyncKeyState(VK_NUMPAD6) & 1)
		{
			bSpeed = !bSpeed;
		}

		if (GetAsyncKeyState(VK_NUMPAD7) & 1)
		{
			bReload = !bReload;
		}

		if (GetAsyncKeyState(VK_NUMPAD8) & 1)
		{
			bAimbot = !bAimbot;
		}


		if (localPlayer)
		{
			if (bHealth)
			{
				localPlayer->health = 6000;
			}

			if (bAmmo)
			{
				//This follows the path we took in reclass, we named our localPlayer, then we went to offset 374
				//to find the current weapon, then we went to offset 14 to find the ammoptr, then we went to the ammo
				// to find the amount of ammo in our weapon. In C++ we do the same thing, which looks like:
				localPlayer->currentweapon->ammoptr->ammo = 6000;

			}

			if (bArmor)
			{
				//explained above
				localPlayer->armor = 6000;
			}

			if (bTriggerbot)
			{
				ent* crossHairEnt = GetEntCrosshair();

				if (crossHairEnt)
				{
					if (localPlayer->team != crossHairEnt->team)  // If they aren't on our team
					{
						localPlayer->bAttack = 1;						// learned that theres other ways to do this for games that dont have an attack bool, shown above
					}

				}

				else
				{
					localPlayer->bAttack = 0;
				}

			}

			if (bFly)
			{
				localPlayer->bFly = 5;
			}

			else
			{
				localPlayer->bFly = 0;
			}

			if (bSpeed) // incredibly laggy and get some weird visual nonsense that Im too noob to understand how to fix, thanks
						// almost positive theres a better way to do this, but This used things I've already learned, how to reverse a function and how to call it
			{
				funcSpeed = (tfuncSpeed)(modBase + 0x25770);
				funcSpeed();
			}

			if (bReload)  // I'm not sure if theres 1 value for this, will check later, because Im pretty sure I can do all of this with just 1
			{			  // command, something like localPlayer->bReload = 0, instead of doing it individually for all weapons
				localPlayer->bReloadAssault = 0;
				localPlayer->bReloadDual = 0;
			}


			if (bAimbot)
			{
				for (int i = 0; i < *numOfPlayers; i++)
				{
					DWORD* enemy_list = (DWORD*)(0x50F4F8);
					DWORD* enemy_offset = (DWORD*)(*enemy_list + (i * 4));
					ent* enemy = (ent*)(*enemy_offset);

					float abspos_x = enemy->bodypos.x - localPlayer->bodypos.x;
					float abspos_y = enemy->bodypos.y - localPlayer->bodypos.y;
					float abspos_z = enemy->bodypos.z - localPlayer->bodypos.z;

					
					float temp_distance = euclidean_distance(abspos_x, abspos_y);
					
					if (closest_player == -1.0f || temp_distance < closest_player)
					{
						closest_player = temp_distance;
					}

					float azimuth_xy = atan2f(abspos_y, abspos_x);
					float yaw = (float)(azimuth_xy * (180.0 / M_PI));

					closest_yaw = yaw + 90;

					if (abspos_y < 0) 
					{
						abspos_y *= -1;
					}

					if (abspos_y < 5)
					{
						if (abspos_x < 0) 
						{
							abspos_x *= -1;
						}
						abspos_y = abspos_x;

					}
					float azimuth_z = atan2f(abspos_z, abspos_y);
					closest_pitch = (float)(azimuth_z * (180.0 / M_PI));

					localPlayer->viewangle.x = closest_yaw;
					localPlayer->viewangle.y = closest_pitch;
				}
				
			}
			Sleep(5);
		}
	}
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

//why the fuck did the code break
