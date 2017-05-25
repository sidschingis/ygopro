#ifndef DEVPRO_EVENT_HANDLER_H
#define DEVPRO_EVENT_HANDLER_H

#include "config.h"
namespace ygo {

	class DevProEventHandler: public irr::IEventReceiver
	{
		public:
		bool OnEvent(const irr::SEvent& event);
	};


	class DevProMenuHandler : public irr::IEventReceiver
	{
		public:
		bool OnEvent(const irr::SEvent& event);
	};
}
#endif