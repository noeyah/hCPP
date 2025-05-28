#pragma once

/* �̱��� ��ũ��
* type = Ŭ����
* �⺻ �����ڿ��� �ϸ�, ���� ���� �ڵ忡�� ���� �����ؾ� ��
* ����/�̵� �Ұ�
* -> ���� ������, ���� ���� ������, �̵� ������, �̵� ���� ������ ����
*/
#define DEFINE_SINGLETON(type)				\
public:										\
	static type& Instance()					\
	{										\
		static type instance;				\
		return instance;					\
	}										\
private:									\
	type(const type&) = delete;				\
	type& operator=(const type&) = delete;	\
	type(type&&) = delete;					\
	type& operator=(type&&) = delete;		\
private: 

