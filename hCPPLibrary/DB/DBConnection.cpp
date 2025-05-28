#include "DBConnection.h"
#include <sqlext.h>
#include "Util/Macro.h"
#include "Log.h"
#include "Util/StringUtil.h"

namespace hlib::db
{
	DBConnection::~DBConnection()
	{
		if (conn_ != SQL_NULL_HANDLE)
		{
			SQLFreeHandle(SQL_HANDLE_DBC, conn_);
			conn_ = SQL_NULL_HANDLE;
		}

		if (stmt_ != SQL_NULL_HANDLE)
		{
			SQLFreeHandle(SQL_HANDLE_STMT, stmt_);
			stmt_ = SQL_NULL_HANDLE;
		}
	}

	void DBConnection::Connect(SQLHENV henv, const WCHAR* connectionStr)
	{
		SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &conn_);
		if (!SQL_SUCCEEDED(ret))
		{
			CRASH("DBConnection dbc failed {}", ret);
		}

		
		SQLWCHAR outConnectionStr[MAX_PATH] = { 0 };
		SQLSMALLINT outConnectionStrLen = 0;
		ret = SQLDriverConnectW(conn_,
								NULL,
								(SQLWCHAR*)connectionStr,
								SQL_NTS,
								outConnectionStr,
								sizeof(outConnectionStr) / sizeof(SQLWCHAR),
								&outConnectionStrLen,
								SQL_DRIVER_NOPROMPT);
		if (!SQL_SUCCEEDED(ret))
		{
			CRASH("DBConnection connect failed {}", ret);
		}

		ret = SQLAllocHandle(SQL_HANDLE_STMT, conn_, &stmt_);
		if (!SQL_SUCCEEDED(ret))
		{
			CRASH("DBConnection stmt failed {}", ret);
		}
	}

	void DBConnection::Unbind()
	{
		SQLFreeStmt(stmt_, SQL_UNBIND);
		SQLFreeStmt(stmt_, SQL_RESET_PARAMS);
		SQLFreeStmt(stmt_, SQL_CLOSE);
	}

	bool DBConnection::Execute(const WCHAR* query)
	{
		SQLRETURN ret = SQLExecDirectW(stmt_, (SQLWCHAR*)query, SQL_NTSL);
		if (!SQL_SUCCEEDED(ret))
		{
			ErrorHandle(ret);
			return false;
		}

		return true;
	}

	bool DBConnection::Fetch()
	{
		SQLRETURN ret = SQLFetch(stmt_);

		if (SQL_SUCCEEDED(ret))
		{
			return true;
		}

		switch (ret)
		{
		case SQL_NO_DATA:
			return false;

		case SQL_STILL_EXECUTING:
			// 아직 완료되지 않음
			return false;

		case SQL_ERROR:
			ErrorHandle(ret);
			return false;
		}

		return false;
	}

	void DBConnection::ErrorHandle(SQLRETURN ret)
	{
		SQLWCHAR sqlState[MAX_PATH]{ 0 };
		SQLINTEGER nativeError = 0;
		SQLWCHAR errorMsg[MAX_PATH]{ 0 };
		SQLSMALLINT errorMsgLen = 0;
		SQLRETURN errorRet = 0;

		for (SQLSMALLINT i = 0; i != SQL_NO_DATA; i++)
		{
			errorRet = SQLGetDiagRecW(SQL_HANDLE_STMT,
									  stmt_,
									  i,
									  sqlState,
									  &nativeError,
									  errorMsg,
									  std::size(errorMsg),
									  &errorMsgLen);

			if (!SQL_SUCCEEDED(errorRet))
			{
				break;
			}

			auto msg = ConvertToUtf8(errorMsg);
			LOG_ERROR("DB Error - {}", msg);
		}
	}
}