#pragma once 

struct OpTable {
	unsigned int MapHash;
	unsigned int GametypeHash;
	unsigned int OpCount;
	unsigned int FunctionCount;
};

OpTable* OP_GetValue( const char* mapname, const char* gametype );