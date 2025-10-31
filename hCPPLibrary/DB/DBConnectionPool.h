#pragma once
#include "WinCommon.h"
#include <sql.h>
#include <mutex>
#include <vector>
#include <memory>
#include "DBConnection.h"

namespace hlib::db
{
	class DBConnectionPool
	{
	public:
		struct Deleter
		{
			DBConnectionPool* pool_;

			explicit Deleter(DBConnectionPool* pool) : pool_(pool) {}

			void operator()(DBConnection* conn) const
			{
				if (pool_ && conn)
				{
					pool_->Push(conn);
				}
				else if (conn)
				{
					delete conn;
				}
			}
		};

	public:
		DBConnectionPool() = default;
		~DBConnectionPool();

		void Connect(size_t connectionCount, const WCHAR* connectionStr);

		std::unique_ptr<DBConnection, Deleter> Pop();

	private:
		void Push(DBConnection* conn);

	private:
		SQLHENV env_ = SQL_NULL_HANDLE;
		std::vector<DBConnection*> pool_;
		std::vector<std::unique_ptr<DBConnection>> all_;
		std::mutex mtx_;
	};

	
}