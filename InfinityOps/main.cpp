#include "main.h"

SYS_MODULE_INFO( InfinityOps, 0, 1, 1 );
SYS_MODULE_START( main_start );
SYS_MODULE_STOP( main_stop );

#pragma control %push notocrestore=2

Detour* RawFile_GetOpCodeChecksum;
Detour* Scr_LoadRawFile_FastFile;

char* Scr_LoadRawFile_FastFileHook(int inst, const char *filename, const char *extFilename, const char *codePos, bool archive) {
	char RawFileBuffer[255] = { 0 };
	CreateFilename(RawFileBuffer, extFilename);

	if (FileExist(RawFileBuffer)) {
		int FileLength = GetFileSize(RawFileBuffer);
		char* scriptBuffer = (char*)Hunk_AllocateTempMemoryHigh(FileLength + 1);
		memset(scriptBuffer, 0, FileLength + 1);

		if (scriptBuffer) {
			//printf("[Infinity Ops]: Loading %s\n", extFilename);

			ReadFile(RawFileBuffer, scriptBuffer, FileLength);
			return scriptBuffer;
		}
	}

	return (char*)Scr_LoadRawFile_FastFile->CallOriginal(inst, filename, extFilename, codePos, archive);
}

void RawFile_GetOpCodeChecksumHook( int* checksumValues, int scriptInstance_t ) {

	if ( !ui_mapname->DvarValue || !g_gametype->DvarValue )
		return;

	OpTable* Value = OP_GetValue( ui_mapname->DvarValue->current.string, g_gametype->DvarValue->current.string );

	if ( !Value ) {
		RawFile_GetOpCodeChecksum->CallOriginal( checksumValues, scriptInstance_t );
		//printf( "WARNING: The OpTable value for mapname \"%s\" on \"%s\" does not exist!\n\tUsers will not be able to join until the value has been accquired!\n\tThe values currently set are: 0x%X and 0x%X\n", ui_mapname->DvarValue->current.string, g_gametype->DvarValue->current.string, checksumValues[ 0 ], checksumValues[ 1 ] );
		return;
	}

	checksumValues[ 0 ] = Value->OpCount;
	checksumValues[ 1 ] = Value->FunctionCount;

	//printf( "[Infinity Ops]: OpTable value loaded!\n" );
}


#pragma control %pop notocrestore

extern "C" int main_start( void ) {


	printf( "[Infinity Ops]: Infinity Ops Loaded for %s\n", IsMultiplayer( ) ? "MP" : "ZM" );

	ApplyPatches( );

	Scr_LoadRawFile_FastFile = new Detour( IsMultiplayer( ) ? 0x5AB578 : 0x595110, (unsigned int)Scr_LoadRawFile_FastFileHook );
	RawFile_GetOpCodeChecksum = new Detour( IsMultiplayer( ) ? 0x5BA818 : 0x5A4248, (unsigned int)RawFile_GetOpCodeChecksumHook );

	return SYS_PRX_RESIDENT;
}

extern "C" int main_stop( void ) {

	printf( "[Infinity Ops]: Infinity Ops Stopped\n" );

	if ( Scr_LoadRawFile_FastFile ) delete Scr_LoadRawFile_FastFile;
	if ( RawFile_GetOpCodeChecksum ) delete RawFile_GetOpCodeChecksum;

	return SYS_PRX_RESIDENT;
}
