#pragma once

#define EXECUTABLE_SECTION __attribute__((section(".text"))); // Cant Execute Data Segment

class Detour {
private:
	static unsigned char HookSection[ 0x10000 ] EXECUTABLE_SECTION;
	static unsigned int HookCount;

	unsigned int dwAddress,
		*pAddress,
		*dwStubAddress,
		dwDestination,
		dwRestoreInstructions[ 4 ],
		dwCurrentInstructions[ 4 ],
		dwStubToc[2];

	unsigned int AllocStub( );
	unsigned int ResolveBranch( unsigned int dwInstruction, unsigned int dwBranchAddress );
	void PatchInJump( unsigned int dwAddress, unsigned int dwDestination, bool Linked );

public:
	Detour( unsigned int dwAddress, unsigned int dwDestination );
	~Detour( );

	void* ( *CallOriginal )( ... );
};
