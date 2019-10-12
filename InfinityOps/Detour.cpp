#include "main.h"

unsigned char Detour::HookSection[ 0x10000 ] EXECUTABLE_SECTION;
unsigned int  Detour::HookCount = 0;

unsigned int Detour::AllocStub( ) {
	unsigned int* dwStartStub = (unsigned int*)( &this->HookSection[ this->HookCount * 0x80 ] );
	this->HookCount++;
	return (unsigned int)dwStartStub;
}

unsigned int Detour::ResolveBranch( unsigned int dwInstruction, unsigned int dwBranchAddress ) {
	unsigned int dwCurrentOffset = dwInstruction & 0x3FFFFFC;
	if ( dwCurrentOffset & ( 1 << 25 ) ) dwCurrentOffset |= 0xFC000000;
	return dwBranchAddress + dwCurrentOffset;
}

void Detour::PatchInJump( unsigned int dwAddress, unsigned int dwDestination, bool Linked ) {
	unsigned int dwInstruction[ 4 ] = { 0 };
	dwInstruction[ 0 ] = 0x3C000000 + ( ( dwDestination >> 16 ) & 0xFFFF );
	dwInstruction[ 1 ] = 0x60000000 + ( dwDestination & 0xFFFF );
	dwInstruction[ 2 ] = 0x7C0903A6;
	dwInstruction[ 3 ] = 0x4E800420 + ( Linked ? 1 : 0 );

	sys_dbg_write_process_memory( dwAddress, dwInstruction, sizeof( unsigned int ) * 4 );
}

Detour::Detour( unsigned int dwAddress, unsigned int dwDestination ) {
	if ( dwAddress == NULL ) return;
	this->dwAddress = dwAddress;
	this->pAddress = (unsigned int*)dwAddress;
	this->dwDestination = *(int*)dwDestination;
	this->dwStubAddress = (unsigned int*)this->AllocStub( );
	memcpy( this->dwRestoreInstructions, (void*)( this->dwAddress ), 0x10 );

	unsigned int InstructionWrittenCount = 0;
	for ( int i = 0; i < 4; i++ ) {
		if ( ( pAddress[ i ] & 0xF8000000 ) == 0x48000000 ) {
			PatchInJump( (unsigned int)&this->dwStubAddress[ InstructionWrittenCount ], (unsigned int)this->ResolveBranch( this->pAddress[ i ], (int)&this->pAddress[ i ] ), true );
			InstructionWrittenCount += 4;
		}
		else {

			sys_dbg_write_process_memory( ( std::uint64_t )&this->dwStubAddress[ InstructionWrittenCount ], &this->pAddress[ i ], sizeof( unsigned int ) * 4 );
			InstructionWrittenCount++;
		}
	}

	PatchInJump( (unsigned int)&this->dwStubAddress[ InstructionWrittenCount ], this->dwAddress + 0x10, false );
	PatchInJump( this->dwAddress, this->dwDestination, false );

	memcpy( this->dwCurrentInstructions, (void*)( this->dwAddress ), 0x10 );

	this->dwStubToc[ 0 ] = (int)this->dwStubAddress;
	this->dwStubToc[ 1 ] = *(int*)( ( (int)dwDestination ) + 4 );

	this->CallOriginal = ( void*( *)( ... ) )this->dwStubToc;

}

Detour::~Detour( ) {
	sys_dbg_write_process_memory( (uint64_t)this->dwAddress, this->dwRestoreInstructions, 0x10 );
}
