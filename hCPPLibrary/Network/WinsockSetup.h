#pragma once
#include "Util/Singleton.h"

namespace hlib::net
{
	class WinsockSetup
	{
		DEFINE_SINGLETON(WinsockSetup);

	public:
		WinsockSetup();
		~WinsockSetup();

	private:
	};

}