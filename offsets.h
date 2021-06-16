#pragma once
#include "stdafx.h"
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>
#include "mem.h"
#include "proc.h"

struct Vector3 { float x, y, z; };

// Created with ReClass.NET 1.2 by KN4CK3R

class ent
{
public:
	char pad_0000[4]; //0x0000
	Vector3 headpos; //0x0004
	char pad_0010[36]; //0x0010
	Vector3 bodypos; //0x0034
	Vector3 viewangle; //0x0040
	char pad_004C[54]; //0x004C
	uint8_t bNoclip; //0x0082
	uint8_t bInvisible; //0x0083
	char pad_0084[116]; //0x0084
	int32_t health; //0x00F8
	int32_t armor; //0x00FC
	char pad_0100[96]; //0x0100
	int32_t N00000331; //0x0160
	int32_t bReloadPistol; //0x0164
	char pad_0168[16]; //0x0168
	int32_t bReloadAssault; //0x0178
	char pad_017C[8]; //0x017C
	int32_t bReloadDual; //0x0184
	char pad_0188[156]; //0x0188
	int8_t bAttack; //0x0224
	char name[16]; //0x0225
	char pad_0235[247]; //0x0235
	int32_t team; //0x032C
	char pad_0330[8]; //0x0330
	int32_t bFly; //0x0338
	char pad_033C[56]; //0x033C
	class weapon* currentweapon; //0x0374
}; //Size: 0x0378
static_assert(sizeof(ent) == 0x378);

class weapon
{
public:
	char pad_0000[20]; //0x0000
	class ammoinclip* ammoptr; //0x0014
}; //Size: 0x0018
static_assert(sizeof(weapon) == 0x18);

class ammoinclip
{
public:
	uint32_t ammo; //0x0000
}; //Size: 0x0004
static_assert(sizeof(ammoinclip) == 0x4);

// for triggerbot
typedef ent* (_cdecl* tGetEntCrosshair)();
tGetEntCrosshair GetEntCrosshair{ nullptr };

// used for speedhack
typedef void(__cdecl* tfuncSpeed) ();
tfuncSpeed funcSpeed;



int* numOfPlayers = (int*)(0x50f500);


