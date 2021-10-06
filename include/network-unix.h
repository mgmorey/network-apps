#ifndef NETWORK_UNIX_H
#define NETWORK_UNIX_H

#ifndef _WIN32

#include <sys/un.h>     // SUN_LEN(), sockaddr_un

#ifndef SUN_LEN
#define SUN_LEN(sun)	(sizeof *(sun) + std::strlen((sun)->sun_path))
#endif

#endif

#endif
