// Includes
#include "stdafx.h"
#include "Tools.h"
#include <stdio.h>

// Methods
PCHAR va(PCHAR Text, ...)
{
	CHAR Buffer[2048];
	va_list VA_ARGS;
	va_start(VA_ARGS, Text);
	vsprintf(Buffer, Text, VA_ARGS);
	return Buffer;
}
PCHAR va(PCHAR Out, PCHAR Text, ...)
{
	va_list VA_ARGS;
	va_start(VA_ARGS, Text);
	vsprintf(Out, Text, VA_ARGS);
	return Out;
}
void DebugData(const PCHAR fmt, ...)
{
	char buffer[1000];
	va_list pArgList;
	va_start(pArgList, fmt);
	vsprintf_s(buffer, 1000, fmt, pArgList);
	va_end(pArgList);

	FILE * fp = fopen("game:\\The Saiyan Project.log", "a+");
	fprintf(fp, "%s", buffer);
	fclose(fp);

	DbgPrint(buffer);
}
char* XKeyboard(DWORD KeyboardFlag, LPCWSTR DefaultText, LPCWSTR Title, LPCWSTR Body)
{
	XOVERLAPPED Overlapped;
	WCHAR wszBuffer[512];
	char* szBuffer;
	XShowKeyboardUI(0, KeyboardFlag, DefaultText, Title, Body, wszBuffer, 512, &Overlapped);
	while (!XHasOverlappedIoCompleted(&Overlapped)) Sleep(100);
	wcstombs(szBuffer, wszBuffer, 512);
	return szBuffer;
}
void XMessageBox(LPCWSTR Title, LPCWSTR Body, DWORD Icon, bool Wait)
{
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED Overlapped;
	LPCWSTR ButtonText[] = { L"Continue" };
	XShowMessageBoxUI(0, Title, Body, 1, ButtonText, 0, Icon, &Result, &Overlapped);
	if (Wait) while (!XHasOverlappedIoCompleted(&Overlapped)) Sleep(100);
}
MESSAGEBOX_RESULT XMessageBox(LPCWSTR Title, LPCWSTR Body, DWORD ButtonCount, LPCWSTR* ButtonText, DWORD Icon)
{
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED Overlapped;
	XShowMessageBoxUI(0, Title, Body, ButtonCount, ButtonText, 0, Icon, &Result, &Overlapped);
	while (!XHasOverlappedIoCompleted(&Overlapped)) Sleep(100);
	return Result;
}
void XNotify(PWCHAR Text)
{
	XNotifyQueueUI(XNOTIFYQUEUEUI_TYPE::XNOTIFYUI_TYPE_PREFERRED_REVIEW, 0, XNOTIFYUI_PRIORITY::XNOTIFYUI_PRIORITY_HIGH, Text, 0);
}
HRESULT doMountPath(const PCHAR szDrive, const PCHAR szDevice, const PCHAR sysStr)
{
	CHAR szDestinationDrive[MAX_PATH];
	sprintf_s(szDestinationDrive, MAX_PATH, sysStr, szDrive);
	STRING DeviceName = MAKE_STRING(szDevice);
	STRING LinkName = MAKE_STRING(szDestinationDrive);
	ObDeleteSymbolicLink(&LinkName);
	return (HRESULT)ObCreateSymbolicLink(&LinkName, &DeviceName);
}
HRESULT MountPath(const PCHAR szDrive, const PCHAR szDevice, bool both)
{
	HRESULT res;
	if (both)
	{
		res = doMountPath(szDrive, szDevice, "\\System??\\%s");
		res = doMountPath(szDrive, szDevice, "\\??\\%s");
	}
	else
	{
		if (KeGetCurrentProcessType() == 2)
			res = doMountPath(szDrive, szDevice, "\\System??\\%s");
		else
			res = doMountPath(szDrive, szDevice, "\\??\\%s");
	}
	return res;
}
bool MountDrives()
{
	return MountPath("HDD:", "\\Device\\Harddisk0\\Partition1", FALSE) == S_OK;
}
bool GetModuleHash(PBYTE ModuleHash)
{
	DWORD ModuleSize;
	PBYTE Module = GetFile("game:\\TheSaiyanProject.xex", &ModuleSize);

	if (!Module || ModuleSize == 0)
		return false;

	BYTE buffer[0x14];

	XECRYPT_SHA_STATE sha;
	XeCryptShaInit(&sha);
	XeCryptShaUpdate(&sha, Module, ModuleSize);
	XeCryptShaFinal(&sha, buffer, 0x14);

	memcpy(ModuleHash, buffer, 0x14);

	return true;
}
bool GetCPUKey(PBYTE CPUKey)
{
	QWORD Fuses[2];
	Fuses[0] = HvPeekQWORD(0x8000020000020600);
	Fuses[1] = HvPeekQWORD(0x8000020000020A00);
	memcpy(CPUKey, Fuses, 0x10);

	return true;
}
PBYTE GetFile(PCHAR szPath, PDWORD pdwOutSize)
{
	FILE *f = fopen(szPath, "rb");

	if (!f)
	{
		*pdwOutSize = 0;
		return 0;
	}

	fseek(f, 0, SEEK_END);
	*pdwOutSize = ftell(f);
	rewind(f);

	BYTE *pbReturnData = (BYTE*)malloc(*pdwOutSize);

	fread(pbReturnData, 1, *pdwOutSize, f);

	fclose(f);

	return pbReturnData;
}
bool DumpFile(PCHAR szFileName, PBYTE pbData, DWORD dwSize)
{
	FILE *f = fopen(szFileName, "wb");

	if (f)
	{
		fwrite(pbData, 1, dwSize, f);
		fflush(f);
		fclose(f);
		return true;
	}

	return false;
}
DWORD ResolveFunction(PCHAR Module, DWORD Ordinal)
{
	DWORD Return = 0, Pointer = 0;
	HANDLE Handle;
	Return = XexGetModuleHandle(Module, &Handle);
	if (Return == 0)
	{
		Return = XexGetProcedureAddress(Handle, Ordinal, &Pointer);
		if (Pointer != 0)
			return Pointer;
	}
	return 0;
}
void PatchInJump(PDWORD addr, DWORD dest, bool linked)
{
	if (dest & 0x8000)
		addr[0] = 0x3D600000 + (((dest >> 16) & 0xFFFF) + 1);
	else
		addr[0] = 0x3D600000 + ((dest >> 16) & 0xFFFF);

	addr[1] = 0x396B0000 + (dest & 0xFFFF);
	addr[2] = 0x7D6903A6;

	addr[3] = 0x4E800420;
	if (linked)
		addr[3]++;

	__dcbst(0, addr);
	__sync();
}
void __declspec(naked) GLPR(void)
{
	__asm
	{
		std     r14, -0x98(sp)
			std     r15, -0x90(sp)
			std     r16, -0x88(sp)
			std     r17, -0x80(sp)
			std     r18, -0x78(sp)
			std     r19, -0x70(sp)
			std     r20, -0x68(sp)
			std     r21, -0x60(sp)
			std     r22, -0x58(sp)
			std     r23, -0x50(sp)
			std     r24, -0x48(sp)
			std     r25, -0x40(sp)
			std     r26, -0x38(sp)
			std     r27, -0x30(sp)
			std     r28, -0x28(sp)
			std     r29, -0x20(sp)
			std     r30, -0x18(sp)
			std     r31, -0x10(sp)
			stw     r12, -0x8(sp)
			blr
	}
}
DWORD RelinkGPLR(DWORD SFSOffset, PDWORD SaveStubAddress, PDWORD OriginalAddress)
{
	DWORD Instruction = 0, Replacing;
	PDWORD Saver = (PDWORD)GLPR;

	if (SFSOffset & 0x2000000)
		SFSOffset |= 0xFC000000;

	Replacing = OriginalAddress[SFSOffset / 4];
	for (int i = 0; i < 20; i++)
	{
		if (Replacing == Saver[i])
		{
			int NewOffset = (int)&Saver[i] - (int)SaveStubAddress;
			Instruction = 0x48000001 | (NewOffset & 0x3FFFFFC);
		}
	}
	return Instruction;
}
void HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination)
{
	if ((SaveStub != NULL) && (Address != NULL))
	{
		DWORD AddressRelocation = (DWORD)(&Address[4]);

		if (AddressRelocation & 0x8000)
		{
			SaveStub[0] = 0x3D600000 + (((AddressRelocation >> 16) & 0xFFFF) + 1);
		}
		else
		{
			SaveStub[0] = 0x3D600000 + ((AddressRelocation >> 16) & 0xFFFF);
		}

		SaveStub[1] = 0x396B0000 + (AddressRelocation & 0xFFFF);
		SaveStub[2] = 0x7D6903A6;

		for (int i = 0; i < 4; i++)
		{
			if ((Address[i] & 0x48000003) == 0x48000001)
			{
				SaveStub[i + 3] = RelinkGPLR((Address[i] & ~0x48000003), &SaveStub[i + 3], &Address[i]);
			}
			else
			{
				SaveStub[i + 3] = Address[i];
			}
		}

		SaveStub[7] = 0x4E800420;
		__dcbst(0, SaveStub);
		__emit(0x7C0004AC);
		__emit(0x4C00012C);

		PatchInJump(Address, Destination, FALSE);
	}
}
DWORD PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress)
{
	DWORD address = (DWORD)ResolveFunction(ImportedModuleName, Ordinal);
	if (address == NULL)
		return S_FALSE;

	void* headerBase = Module->XexHeaderBase;
	PXEX_IMPORT_DESCRIPTOR importDesc = (PXEX_IMPORT_DESCRIPTOR)RtlImageXexHeaderField(headerBase, 0x000103FF);
	if (importDesc == NULL)
		return S_FALSE;

	DWORD result = 2;
	char* stringTable = (char*)(importDesc + 1);
	XEX_IMPORT_TABLE_ORG* importTable = (XEX_IMPORT_TABLE_ORG*)(stringTable + importDesc->NameTableSize);

	for (DWORD x = 0; x < importDesc->ModuleCount; x++)
	{
		DWORD* importAdd = (DWORD*)(importTable + 1);
		for (DWORD y = 0; y < importTable->ImportTable.ImportCount; y++)
		{
			DWORD value = *((DWORD*)importAdd[y]);
			if (value == address)
			{
				memcpy((DWORD*)importAdd[y], &PatchAddress, 4);
				DWORD newCode[4];
				PatchInJump(newCode, PatchAddress, false);
				memcpy((DWORD*)importAdd[y + 1], newCode, 16);

				result = S_OK;
			}
		}

		importTable = (XEX_IMPORT_TABLE_ORG*)(((BYTE*)importTable) + importTable->TableSize);
	}

	return result;
}
DWORD PatchModuleImport(PCHAR Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress)
{
	LDR_DATA_TABLE_ENTRY* moduleHandle = (LDR_DATA_TABLE_ENTRY*)GetModuleHandle(Module);
	return (moduleHandle == NULL) ? S_FALSE : PatchModuleImport(moduleHandle, ImportedModuleName, Ordinal, PatchAddress);
}
static HvxCall HvxExpansionInstall(DWORD PhysicalAddress, DWORD CodeSize) {
	__asm {
		li			r0, 0x70
			sc
			blr
	}
}
static HvxCall HvxExpansionCall(DWORD ExpansionId, QWORD Param1 = 0, QWORD Param2 = 0, QWORD Param3 = 0, QWORD Param4 = 0) {
	__asm {
		li			r0, 0x71
			sc
			blr
	}
}
static HvxCall HvxBlowFuses()
{
	__asm
	{
		lis r3, 0x4856
		addi r3, r3, 0x5050
		li r0, 0x22
		sc
		blr
	}
}
HRESULT InitializeHvPeekPoke() {

	// Allocate physcial memory for this expansion
	VOID* pPhysExp = XPhysicalAlloc(0x1000, MAXULONG_PTR, 0, PAGE_READWRITE);
	DWORD physExpAdd = (DWORD)MmGetPhysicalAddress(pPhysExp);

	// Copy over our expansion data
	ZeroMemory(pPhysExp, 0x1000);
	memcpy(pPhysExp, HvPeekPokeExp, sizeof(HvPeekPokeExp));

	// Now we can install our expansion
	HRESULT result = (HRESULT)HvxExpansionInstall(physExpAdd, 0x1000);

	// Free our allocated data
	XPhysicalFree(pPhysExp);

	// Return our install result
	return result;
}
void HvBlowFuses()
{
	BYTE HvxBlowFusesData[0x8] = { 0x7D, 0x88, 0x02, 0xA6, 0x48, 0x00, 0x02, 0x99 };
	HvPokeBytes(0x800002000000A560, HvxBlowFusesData, 8);
}
BYTE HvPeekBYTE(QWORD Address) {
	return (BYTE)HvxExpansionCall(HvPeekPokeExpID, PEEK_BYTE, Address);
}
WORD HvPeekWORD(QWORD Address) {
	return (WORD)HvxExpansionCall(HvPeekPokeExpID, PEEK_WORD, Address);
}
DWORD HvPeekDWORD(QWORD Address) {
	return (DWORD)HvxExpansionCall(HvPeekPokeExpID, PEEK_DWORD, Address);
}
QWORD HvPeekQWORD(QWORD Address) {
	return HvxExpansionCall(HvPeekPokeExpID, PEEK_QWORD, Address);
}
HRESULT HvPeekBytes(QWORD Address, PVOID Buffer, DWORD Size) {

	// Create a physical buffer to peek into
	VOID* data = XPhysicalAlloc(Size, MAXULONG_PTR, 0, PAGE_READWRITE);
	ZeroMemory(data, Size);

	HRESULT result = (HRESULT)HvxExpansionCall(HvPeekPokeExpID,
		PEEK_BYTES, Address, (QWORD)MmGetPhysicalAddress(data), Size);

	// If its successful copy it back
	if (result == S_OK) memcpy(Buffer, data, Size);

	// Free our physical data and return our result
	XPhysicalFree(data);
	return result;
}
HRESULT HvPokeBYTE(QWORD Address, BYTE Value) {
	return (HRESULT)HvxExpansionCall(HvPeekPokeExpID, POKE_BYTE, Address, Value);
}
HRESULT HvPokeWORD(QWORD Address, WORD Value) {
	return (HRESULT)HvxExpansionCall(HvPeekPokeExpID, POKE_WORD, Address, Value);
}
HRESULT HvPokeDWORD(QWORD Address, DWORD Value) {
	return (HRESULT)HvxExpansionCall(HvPeekPokeExpID, POKE_DWORD, Address, Value);
}
HRESULT HvPokeQWORD(QWORD Address, QWORD Value) {
	return (HRESULT)HvxExpansionCall(HvPeekPokeExpID, POKE_QWORD, Address, Value);
}
HRESULT HvPokeBytes(QWORD Address, const PVOID Buffer, DWORD Size) {

	// Create a physical buffer to poke from
	VOID* data = XPhysicalAlloc(Size, MAXULONG_PTR, 0, PAGE_READWRITE);
	memcpy(data, Buffer, Size);

	HRESULT result = (HRESULT)HvxExpansionCall(HvPeekPokeExpID,
		POKE_BYTES, Address, (QWORD)MmGetPhysicalAddress(data), Size);

	// Free our physical data and return our result
	XPhysicalFree(data);
	return result;
}