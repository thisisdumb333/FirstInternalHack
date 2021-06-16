#include "stdafx.h"
#include "mem.h"

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
}

void mem::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);

	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;
}

uintptr_t mem::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}

//detour32 (where you want to write your jump, what you want to jump to, and the length of your jump)																		
bool mem::Detour32(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < 5) return false;

	DWORD curProtection;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);  //pages of mememory we're overwriting dont have write protection due to security, so we give it here

	uintptr_t relativeAddress = dst - src - 5; 

	*src = 0xE9;  //jump instruction

	*(uintptr_t*)(src + 1) = relativeAddress;  //address you jump to

	VirtualProtect(src, len, curProtection, &curProtection);    //Used to reset the protection since we overwrote in line 57
	return true;

}

BYTE* mem::TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < 5) return 0;

	//create gateway
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE); 

	//write the stolen bytes to the gateway
	memcpy_s(gateway, len, src, len);         //copy stolen bytes into the gateway, or else you will corrupt the stack

	//get the gateway to destination address
	uintptr_t gatewayRelativeAddr = src - gateway - 5;

	//add jump opcode to end of the gateway
	*(gateway + len) = 0xE9;

	//write the address of the gateway to the jump
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelativeAddr;

	//preform the detour
	Detour32(src, dst, len);

	return gateway;
}

//Hooking currently gives me an assault cube fatal error, win32 exception: 0x0000005 [0xeaf61180]
//gotta luck more into it.