#pragma once
#include "Util/Singleton.h"

namespace hlib::net
{
	class WinsockSetup : public Singleton<WinsockSetup>
	{
	public:
		WinsockSetup();
		~WinsockSetup();

	private:
	};

}