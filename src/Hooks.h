#pragma once

// Includes
#include "stdafx.h"

// Hook Stubs
static DWORD __declspec(naked) AddonCheck_Stub(DWORD dwAddonHash)
{
	__asm
	{
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}
static QWORD __declspec(naked) ParseScript_Stub(DWORD dwStatics, DWORD dwGlobalVars, DWORD dwHeader, DWORD unk2, QWORD unk3, QWORD unk4)
{
	__asm
	{
		li r3, 1
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}
static void __declspec(naked) PushNative_Stub(DWORD dwNativePoolPointer, DWORD dwNativeHash, DWORD dwNativeAddress)
{
	__asm
	{
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}

// Methods
DWORD NetworkPlayerIdToInt();
QWORD ParseScript(DWORD dwStatics, DWORD dwGlobalVars, DWORD dwHeader, DWORD unk2, QWORD unk3, QWORD unk4);
void PushNative(DWORD dwNativeTable, DWORD dwNativeHash, DWORD dwNativeAddress);
void Timer();
DWORD XamUserGetNameHook(DWORD dwUserIndex, LPSTR szUserName, DWORD cchUserName);
DWORD XamUserGetXUIDHook(DWORD dwUserIndex, DWORD unk, PXUID pxuid);
DWORD XamUserGetSigninInfoHook(DWORD dwUserIndex, DWORD dwFlags, PXUSER_SIGNIN_INFO pSigninInfo);