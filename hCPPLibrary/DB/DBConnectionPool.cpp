#include "DBConnectionPool.h"
#include <sqlext.h>
#include "Util/Macro.h"

namespace hlib::db
{
	DBConnectionPool::~DBConnectionPool()
	{
		if (env_ != SQL_NULL_HANDLE)
		{
			SQLFreeHandle(SQL_HANDLE_ENV, env_);
			env_ = SQL_NULL_HANDLE;
		}

		// std::unique_ptr<DBConnection>¿¡¼­ delete
	}

	void DBConnectionPool::Connect(size_t connectionCount, const WCHAR* connectionStr)
	{
		std::lock_guard lock(mtx_);

		SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env_);
		if (!SQL_SUCCEEDED(ret))
		{
			CRASH("DBConnectionPool env failed {}", ret);
		}

		ret = SQLSetEnvAttr(env_, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);
		if (!SQL_SUCCEEDED(ret))
		{
			CRASH("DBConnectionPool odbc version failed {}", ret);
		}

		pool_.reserve(connectionCount);
		for (size_t i = 0; i < connectionCount; i++)
		{
			DBConnection* conn = new DBConnection;
			conn->Connect(env_, connectionStr);

			all_.push_back(std::unique_ptr<DBConnection>(conn));
			pool_.push_back(conn);
		}
	}

	std::unique_ptr<DBConnection, DBConnectionPool::Deleter> DBConnectionPool::Pop()
	{
		std::lock_guard lock(mtx_);

		if (pool_.empty())
		{
			return std::unique_ptr<DBConnection, DBConnectionPool::Deleter>(nullptr, DBConnectionPool::Deleter(this));
		}

		DBConnection* conn = pool_.back();
		pool_.pop_back();

		return std::unique_ptr<DBConnection, DBConnectionPool::Deleter>(conn, DBConnectionPool::Deleter(this));
	}

	void DBConnectionPool::Push(DBConnection* conn)
	{
		if (!conn)
		{
			return;
		}

		conn->Unbind();

		std::lock_guard lock(mtx_);
		pool_.push_back(conn);
	}
}