#include "main.h"

bool IsMultiplayer( )
{
	return ( *(int*)( 0x00595120 ) != 0xFB8100A0 );
}

Pointer* ui_mapname;
Pointer* g_gametype;

int DB_FindXAssetHeader_s[ 2 ] = { 0x2601A0, 0xB576E8 };
int Com_GetExtensionSubString_s[ 2 ] = { 0x4DA2A8, 0xB576E8 };
int Hunk_AllocateTempMemoryHigh_s[ 2 ] = { 0x4BD4E0, 0xB576E8 };

int SP_DB_FindXAssetHeader_s[ 2 ] = { 0x1B1E20, 0xA544B0 };
int SP_Com_GetExtensionSubString_s[ 2 ] = { 0x467268, 0xA544B0 };
int SP_Hunk_AllocateTempMemoryHigh_s[ 2 ] = { 0x00449E98, 0xA544B0 };

const char* Com_GetExtensionSubString( const char* fileName )
{
	const char* ( *Com_GetExtensionSubString_t )( const char* fileName ) = ( const char*( *)( const char* ) )( IsMultiplayer( ) ? Com_GetExtensionSubString_s : SP_Com_GetExtensionSubString_s );

	return Com_GetExtensionSubString_t( fileName );
}

void* Hunk_AllocateTempMemoryHigh( unsigned int length )
{
	void*( *Hunk_AllocateTempMemoryHigh_t )( unsigned int length ) = ( void*( *)( unsigned int ) )( IsMultiplayer( ) ? Hunk_AllocateTempMemoryHigh_s : SP_Hunk_AllocateTempMemoryHigh_s );

	return Hunk_AllocateTempMemoryHigh_t( length );
}

void DB_FindXAssetHeader( RawFile** File, int assetType, const char* assetName, bool errorIfMissing, int waitTime )
{
	void( *DB_FindXAssetHeader_t )( RawFile** File, int assetType, const char* assetName, bool errorIfMissing, int waitTime ) = ( void( *)( RawFile** File, int, const char*, bool, int ) )( IsMultiplayer( ) ? DB_FindXAssetHeader_s : SP_DB_FindXAssetHeader_s );

	DB_FindXAssetHeader_t( File, assetType, assetName, errorIfMissing, waitTime );
}

void ApplyPatches( )
{
	ui_mapname = (Pointer*)( IsMultiplayer( ) ? 0x170EC70 : 0x138E31C );
	g_gametype = (Pointer*)( IsMultiplayer( ) ? 0x170EC84 : 0x1844C08 );

	int Nop = 0x60000000, branch = 0x4BFFFE70;
	sys_dbg_write_process_memory( ( std::uint64_t )( IsMultiplayer( ) ? 0x004BD584 : 0x00449F3C ), &Nop, 4 );
	sys_dbg_write_process_memory( ( std::uint64_t )( IsMultiplayer( ) ? 0x004C8EC4 : 0x00449F3C ), &Nop, 4 );
	sys_dbg_write_process_memory( ( std::uint64_t )( IsMultiplayer( ) ? 0x004C8ED0 : 0x00449F3C ), &Nop, 4 );
	sys_dbg_write_process_memory( ( std::uint64_t )( IsMultiplayer( ) ? 0x004C8EDC : 0x00449F3C ), &branch, 4 );

}