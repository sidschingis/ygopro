#ifndef DEVPRO_NETWORK_H
#define DEVPRO_NETWORK_H

//#include "config.h"
namespace ygo {
	struct STOC_Sleeve {
		unsigned short player;
		unsigned short site[256];
		unsigned short dir[256];
	};

	struct STOC_HS_PlayerEnter_DevPro {
		unsigned short name[20];
		unsigned char pos;
		unsigned short elo[5];
	};
}
#endif