#pragma once
#include "WinCommon.h"
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <bitset>
#include <type_traits>
#include <string>
#include <cstring>
#include <cassert>
#include "DBConnection.h"
#include "DBTypes.h"

namespace hlib::db
{
	template <size_t ParamCount, size_t ColumnCount>
	class DBQuery
	{
	public:
		DBQuery(DBConnection& conn, const WCHAR* query) : conn_(conn), query_(query)
		{
			conn_.Unbind();
		}

		bool Execute()
		{
			if (ParamCount > 0)
			{
				assert(paramBit_.all());
			}

			if (ColumnCount > 0)
			{
				assert(columnBit_.all());
			}

			return conn_.Execute(query_);
		}

		bool Fetch()
		{
			return conn_.Fetch();
		}

	public:
		template <typename T>
		DBQuery& Param(SQLUSMALLINT index, T& value);
		
		template <typename T>
		DBQuery& ParamNull(SQLUSMALLINT index);

		template <typename T>
		DBQuery& Column(SQLUSMALLINT index, T& value);
		

	private:
		DBConnection& conn_;
		const WCHAR* query_;

		std::bitset<(ParamCount > 0 ? ParamCount : 1)> paramBit_{ 0 };
		std::bitset<(ColumnCount > 0 ? ColumnCount : 1)> columnBit_{ 0 };

		SQLLEN paramInd_[ParamCount > 0 ? ParamCount : 1]{ 0 };
		SQLLEN columnInd_[ColumnCount > 0 ? ColumnCount : 1]{ 0 };
	};

	// ÅÛÇÃ¸´ ±¸Çö
	template<size_t ParamCount, size_t ColumnCount>
	template<typename T>
	inline DBQuery<ParamCount, ColumnCount>& DBQuery<ParamCount, ColumnCount>::Param(SQLUSMALLINT index, T& value)
	{
		assert(index > 0 && index <= ParamCount);

		SQLSMALLINT cType = DBTypes<T>::C_TYPE;
		SQLSMALLINT sqlType = DBTypes<T>::SQL_TYPE;
		SQLULEN len = 0;
		SQLSMALLINT decimalDigits = DBTypes<T>::DECIMAL_DIGITS;
		SQLPOINTER ptr = (SQLPOINTER)&value;
		SQLLEN bufferLen = 0;

		if constexpr (std::is_same_v<T, const WCHAR*>)
		{
			len = static_cast<SQLULEN>(std::wcslen(value));
			ptr = (SQLPOINTER)value;
			bufferLen = static_cast<SQLLEN>(std::wcslen(value) * sizeof(WCHAR));
			paramInd_[index - 1] = SQL_NTS;
		}
		else if constexpr (std::is_same_v<T, const char*>)
		{
			len = static_cast<SQLULEN>(std::strlen(value));
			ptr = (SQLPOINTER)value;
			bufferLen = static_cast<SQLLEN>(std::strlen(value));
			paramInd_[index - 1] = SQL_NTS;
		}
		else if constexpr (std::is_same_v<T, std::wstring>)
		{
			len = static_cast<SQLULEN>(value.length());
			ptr = (SQLPOINTER)value.c_str();
			bufferLen = static_cast<SQLLEN>((value.length() + 1) * sizeof(WCHAR));
			paramInd_[index - 1] = SQL_NTS;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			len = static_cast<SQLULEN>(value.length());
			ptr = (SQLPOINTER)value.c_str();
			bufferLen = static_cast<SQLLEN>(value.length() + 1);
			paramInd_[index - 1] = SQL_NTS;
		}
		else
		{
			bufferLen = sizeof(T);
		}

		SQLRETURN ret = SQLBindParameter(conn_.GetStmt(),
										 index,
										 SQL_PARAM_INPUT,
										 cType,
										 sqlType,
										 len,
										 decimalDigits,
										 ptr,
										 bufferLen,
										 &paramInd_[index - 1]);
		assert(SQL_SUCCEEDED(ret));
		paramBit_.set(index - 1);
		return *this;
	}

	template<size_t ParamCount, size_t ColumnCount>
	template<typename T>
	inline DBQuery<ParamCount, ColumnCount>& DBQuery<ParamCount, ColumnCount>::ParamNull(SQLUSMALLINT index)
	{
		assert(index > 0 && index <= ParamCount);

		SQLRETURN ret = SQLBindParameter(conn_.GetStmt(),
										 index,
										 SQL_PARAM_INPUT,
										 DBTypes<T>::C_TYPE,
										 DBTypes<T>::SQL_TYPE,
										 0,
										 0,
										 nullptr,
										 0,
										 &paramInd_[index - 1]);
		assert(SQL_SUCCEEDED(ret));
		paramBit_.set(index - 1);
		return *this;
	}

	template<size_t ParamCount, size_t ColumnCount>
	template<typename T>
	inline DBQuery<ParamCount, ColumnCount>& DBQuery<ParamCount, ColumnCount>::Column(SQLUSMALLINT index, T& value)
	{
		assert(index > 0 && index <= ColumnCount);

		SQLSMALLINT ctype = 0;
		SQLPOINTER ptr = nullptr;
		SQLLEN bufferLen = 0;

		if constexpr (std::is_arithmetic_v<T>
			|| std::is_enum_v<T>
			|| std::is_same_v<T, SQL_TIMESTAMP_STRUCT>
			|| std::is_same_v<T, SQL_DATE_STRUCT>
			|| std::is_same_v<T, SQL_TIME_STRUCT>)
		{
			ctype = DBTypes<T>::C_TYPE;
			ptr = (SQLPOINTER)&value;
			bufferLen = static_cast<SQLLEN>(sizeof(T));
		}
		else if constexpr (std::is_array_v<T>
						   && (std::is_same_v<std::remove_extent_t<T>, char>
							   || std::is_same_v<std::remove_extent_t<T>, WCHAR>))
		{
			// char[], WCHAR[]
			ctype = DBTypes<std::remove_extent_t<T>*>::C_TYPE;
			ptr = (SQLPOINTER)value;
			bufferLen = static_cast<SQLLEN>(sizeof(value));
		}
		else
		{
			static_assert(std::false_type::value, "Unsupported Type In DBQuery Column");
		}

		SQLRETURN ret = SQLBindCol(conn_.GetStmt(),
								   index,
								   ctype,
								   ptr,
								   bufferLen,
								   &columnInd_[index - 1]);
		assert(SQL_SUCCEEDED(ret));
		columnBit_.set(index - 1);
		return *this;
	}
}