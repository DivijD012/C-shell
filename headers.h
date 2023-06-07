#ifndef HEADERS_H_
#define HEADERS_H_

#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"
#define COLOR_YELLOW "\x1b[33m"

#define BILLION  1000000000L

#include <stdio.h>    
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "cd.h"
#include "prompt.h"
#include "print_wd.h"
#include "echo_this.h"
#include <sys/types.h>
#include <pwd.h>
#include "list_em.h"
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include "getpinfo.h"
#include <fcntl.h>
#include "othercmd.h"
#include <sys/wait.h>
#include <sys/time.h>
#include "hist.h"
#include "discovery.h"
#include <signal.h>
#include "jobs_user.h"
#include "send_signal.h"
#include "fore_g.h"
#include <errno.h>
#include "back_g.h"
#include "sig_handle.h"
#include <termios.h>
#include "input.h"
#include <ctype.h>
#endif