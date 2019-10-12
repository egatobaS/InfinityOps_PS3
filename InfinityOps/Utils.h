#pragma once

#include <sys/cdefs.h>
#include <exception>
#include <string>

#define Sleep(time) sys_timer_usleep(time * 1000)


extern "C" {
	int	_sys_printf(const char* format, ...);
	int	_sys_sprintf(char* s, const char* format, ...);
	int	_sys_vsprintf(char * s, const char * format, va_list arg);

	void *_sys_malloc(size_t n);
	void _sys_free(void* ptr);
	void *_sys_memchr(void * ptr, int value, size_t num);
	void *_sys_memcpy(void * destination, const void * source, size_t num);
	int _sys_memcmp(const void * ptr1, const void * ptr2, size_t num);
	void *_sys_memset(void * ptr, int value, size_t num);
	void *_sys_memmove(void * destination, const void * source, size_t num);

	char *_sys_strcat(char * destination, const char * source);
	char *_sys_strchr(char * str, int character);
	int _sys_strcmp(const char * str1, const char * str2);
	char *_sys_strcpy(char * destination, const char * source);
	size_t _sys_strlen(const char * str);
	char *_sys_strncat(char * destination, const char * source, size_t num);
	int _sys_strncmp(const char * str1, const char * str2, size_t num);
	char *_sys_strncpy(char * destination, const char * source, size_t num);
	char *_sys_strrchr(char * str, int character);
	int _sys_tolower(int c);
	int _sys_toupper(int c);
}

#define printf			_sys_printf
#define sprintf			_sys_sprintf
#define vsprintf		_sys_vsprintf

#define malloc			_sys_malloc
#define free			_sys_free
#define memchr			_sys_memchr
#define memcpy			_sys_memcpy
#define memcmp			_sys_memcmp
#define memset			_sys_memset
#define memmove			_sys_memmove

#define strcat			_sys_strcat
#define strchr			_sys_strchr
#define strcmp			_sys_strcmp
#define strcpy			_sys_strcpy
#define strlen			_sys_strlen
#define strncat			_sys_strncat
#define strncmp			_sys_strncmp
#define strncpy			_sys_strncpy
#define strrchr			_sys_strrchr
#define tolower			_sys_tolower
#define toupper			_sys_toupper


inline void* operator new( size_t size ) { return _sys_malloc( size ); }
inline void* operator new[] ( size_t size ) { return _sys_malloc( size ); }
inline void* operator new( unsigned int size, unsigned int ) { return _sys_malloc( size ); }
inline void operator delete( void* ptr ) { _sys_free( ptr ); }
inline void operator delete[] ( void* ptr ) { _sys_free( ptr ); }


#define UNALIGNED __unaligned
template <typename RTLP_T, size_t N>
char( *RtlpNumberOf( UNALIGNED RTLP_T( &)[ N ] ) )[ N ];

#define RTL_NUMBER_OF_V2(A) (sizeof(*RtlpNumberOf(A)))
#define ARRAYSIZE(A)    RTL_NUMBER_OF_V2(A)

int sys_dbg_read_process_memory( uint64_t address, void* data, size_t size );
int sys_dbg_write_process_memory( uint64_t address, void* data, size_t size );
bool ReadFile( const char *File, char *Buffer, size_t Size );
bool WriteFile( const char *File, char *Buffer, size_t Size );
bool DirectoryExist( const char* Directory );
bool FileExist( const char* File );
void CreateDirectory( const char* Directory );
int GetFileSize( const char* File );
void CreateFilename( char* Output, const char* filename );