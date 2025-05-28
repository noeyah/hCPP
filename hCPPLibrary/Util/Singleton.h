#pragma once

/* 싱글톤 매크로
* type = 클래스
* 기본 생성자여야 하며, 내부 정적 코드에서 접근 가능해야 함
* 복사/이동 불가
* -> 복사 생성자, 복사 대입 연산자, 이동 생성자, 이동 대입 연산자 삭제
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

