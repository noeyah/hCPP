#include "DummyManager.h"
#include "Log.h"
#include "Dummy.h"

void DummyManager::CreateDummies(int count)
{
	m_dummies.clear();
	m_dummies.reserve(count);

	for (int i = 0; i < count; i++)
	{
		auto dummy = std::make_shared<Dummy>(i);
		dummy->Init();
		m_dummies.emplace_back(dummy);
	}

	LOG_INFO("Dummies {} created", m_dummies.size());
}

void DummyManager::Update()
{
	for (auto& dummy : m_dummies)
	{
		dummy->Update();
	}
}

void DummyManager::Clear()
{
	m_dummies.clear();
}
