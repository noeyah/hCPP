#pragma once
#include "WinCommon.h"
#include <sql.h>
#include <sqltypes.h>

namespace hlib::db
{
	class DBConnection
	{
	public:
		DBConnection() = default;
		~DBConnection();

		void Connect(SQLHENV henv, const WCHAR* connectionStr);
		void Unbind();

		bool Execute(const WCHAR* query);
		bool Fetch();

		void ErrorHandle(SQLRETURN ret);

		SQLHSTMT GetStmt() const { return stmt_; }

	private:
		SQLHDBC conn_ = SQL_NULL_HANDLE;
		SQLHSTMT stmt_ = SQL_NULL_HANDLE;
	};
}