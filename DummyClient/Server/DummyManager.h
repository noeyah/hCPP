#pragma once
#include <vector>
#include <memory>

class Dummy;
using DummyPtr = std::shared_ptr<Dummy>;

class DummyManager
{
	std::vector<DummyPtr> m_dummies;

public:
	void CreateDummies(int count);
	void Update();
	void Clear();
};

