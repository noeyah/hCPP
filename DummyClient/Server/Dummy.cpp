#include "Dummy.h"
#include "Server/Task/Connect.h"
#include "Server/Task/JoinRoom.h"
#include "Server/Task/Chat.h"
#include "Server/Task/LeaveRoom.h"
#include "Server/Task/Wait.h"
#include "Server/Task/Think.h"
#include "Server.h"
#include "Server/ServerSession.h"

using namespace hlib;

void Dummy::Init()
{
	auto pSelf = shared_from_this();

	m_pBT = CreateBT(
		Selector(
			Condition([pSelf]() { return pSelf->IsConnected(); },
					  Selector(
						  Condition([pSelf]() { return !pSelf->IsInRoom(); },
									// 방에 없을 때 : 입장 or 대기
									RandomSelector(
										Action<Task::JoinRoom>(pSelf),
										Action<Task::Wait>(pSelf, 5000)
									)
						  ),
						  Condition([pSelf]() {return pSelf->IsInRoom(); },
									Sequence(
										// 방에 있을 때 : 채팅 or 퇴장 or 대기 
										Action<Task::Think>(pSelf),
										Selector(
											Condition([pSelf]() { return pSelf->GetNextTask() == DummyTaskInRoom::Chat; },
													  Sequence(
														  Action<Task::Chat>(pSelf),
														  Action<Task::Wait>(pSelf, 5000)
													  )
											),
											Condition([pSelf]() { return pSelf->GetNextTask() == DummyTaskInRoom::LeaveRoom; },
													  Action<Task::LeaveRoom>(pSelf)
											),
											Action<Task::Wait>(pSelf, 1000)
										)
									)
						  )
					  )
			),
			Action<Task::Connect>(pSelf)
		)
	);
}

void Dummy::Update()
{
	if (m_pBT)
		m_pBT->Tick();
}

bool Dummy::Connect()
{
	auto pSession = Server::Instance()->Connect();
	if (!pSession)
		return false;

	m_wpSession = pSession;
	pSession->SetDummy(shared_from_this());
	return true;
}
