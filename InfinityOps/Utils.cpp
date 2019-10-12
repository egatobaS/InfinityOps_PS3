#include "main.h"

int sys_dbg_read_process_memory(uint64_t address, void* data, size_t size) {
	system_call_4(904, (uint64_t)sys_process_getpid(), address, size, (uint64_t)data);
	return_to_user_prog(int);
}

int sys_dbg_write_process_memory(uint64_t address, void* data, size_t size) {
	system_call_4(905, (uint64_t)sys_process_getpid(), address, size, (uint64_t)data);
	__dcbst((void*)address);
	__sync();
	__isync();
	return_to_user_prog(int32_t);
}

bool ReadFile( const char *File, char *Buffer, size_t Size )
{
	int fd = 0;
	uint64_t pos = 0;
	uint64_t nread = 0;
	if ( cellFsOpen( File, 0, &fd, NULL, 0 ) == CELL_FS_SUCCEEDED )
	{
		cellFsLseek( fd, 0, CELL_FS_SEEK_SET, &pos );
		cellFsRead( fd, Buffer, Size, &nread );
		cellFsClose( fd );

		return true;
	}
	return false;
}

bool WriteFile( const char *File, char *Buffer, size_t Size )
{
	int fd = 0;
	uint64_t pos = 0, nwrite = 0;

	if ( cellFsOpen( File, CELL_FS_O_WRONLY | CELL_FS_O_CREAT | CELL_FS_O_TRUNC, &fd, NULL, 0 ) == CELL_FS_SUCCEEDED )
	{
		cellFsLseek( fd, 0, CELL_FS_SEEK_SET, &pos );
		cellFsWrite( fd, Buffer, Size, &nwrite );
		cellFsClose( fd );

		return true;
	}

	return false;
}

bool DirectoryExist( const char* Directory )
{
	struct CellFsStat s;
	if ( cellFsStat( Directory, &s ) == CELL_FS_SUCCEEDED )
		return ( ( s.st_mode & CELL_FS_S_IFDIR ) != 0 );

	return false;
}

bool FileExist( const char* File )
{
	struct CellFsStat s;
	if ( cellFsStat( File, &s ) == CELL_FS_SUCCEEDED )
		return ( ( s.st_mode & CELL_FS_S_IFREG ) != 0 );

	return false;
}

void CreateDirectory( const char* Directory )
{
	if ( !DirectoryExist( Directory ) )
		cellFsMkdir( Directory, 0777 );
}


int GetFileSize( const char* File )
{
	struct CellFsStat s;
	if ( cellFsStat( File, &s ) == CELL_FS_SUCCEEDED )
		return s.st_size;

	return -1;
}

void CreateFilename( char* Output, const char* filename ) 
{

	sprintf( Output, "/dev_hdd0/tmp/raw/%s", filename );
}


