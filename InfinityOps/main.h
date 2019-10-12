#pragma once

#include <cellstatus.h>
#include <sys/prx.h>
#include <time.h>
#include <fastmath.h>
#include <ppu_intrinsics.h>
#include <stdarg.h>
#include <stddef.h>

#include <sys/prx.h>
#include <sys/syscall.h>
#include <sys/ppu_thread.h>
#include <sys/sys_time.h>
#include <sys/time_util.h>
#include <sys/timer.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet\in.h>
#include <arpa\inet.h>
#include <netdb.h>
#include <cell\pad\libpad.h>
#include <sys/process.h>
#include <sys/memory.h>

#pragma comment(lib, "net_stub")
#pragma comment(lib, "netctl_stub")

#include <cell/fs/cell_fs_file_api.h>
#include <cell\cell_fs.h>

#include "Utils.h"
#include "Detour.h"
#include "OpTable.h"
#include "Address.h"