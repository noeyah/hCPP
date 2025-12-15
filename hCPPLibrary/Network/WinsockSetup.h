#pragma once
#include "Util/Singleton.h"

namespace hlib
{
	class WinsockSetup : public Singleton<WinsockSetup>
	{
	public:
		WinsockSetup();
		~WinsockSetup();

	private:
	};

}