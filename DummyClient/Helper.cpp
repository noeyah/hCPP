#include "Helper.h"
#include "Server.h"

namespace Helper
{
	void PushSchedule(uint64_t ms, std::function<void()> fn)
	{
		auto pScheduler = Server::Instance()->GetScheduler();
		if (!pScheduler)
			return;

		pScheduler->PushAfter(ms, fn);
	}
}
