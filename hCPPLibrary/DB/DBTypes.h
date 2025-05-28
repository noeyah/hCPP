#pragma once
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <cstdint>
#include <string>
#include <cstring>
#include <type_traits>

namespace hlib::db
{
	template <typename T>
	struct DBTypes
	{
		static_assert(std::false_type::value, "Invalid DBTypes");
	};

#pragma region 소수점
	template <>
	struct DBTypes<float>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_FLOAT;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_REAL;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<double>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_DOUBLE;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_DOUBLE;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};
#pragma endregion

#pragma region 정수형

	template <>
	struct DBTypes<bool>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_BIT;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_BIT;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<int8_t>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_STINYINT;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_TINYINT;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<int16_t>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_SSHORT;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_SMALLINT;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<int32_t>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_SLONG;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_INTEGER;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<int64_t>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_SBIGINT;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_BIGINT;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};


	template <>
	struct DBTypes<uint8_t>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_UTINYINT;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_TINYINT;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<uint16_t>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_USHORT;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_SMALLINT;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<uint32_t>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_ULONG;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_INTEGER;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<uint64_t>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_UBIGINT;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_BIGINT;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};
#pragma endregion

#pragma region 문자열

	template <>
	struct DBTypes<const WCHAR*>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_WCHAR;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_WVARCHAR;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<WCHAR*>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_WCHAR;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_WVARCHAR;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<std::wstring>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_WCHAR;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_WVARCHAR;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<const char*>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_CHAR;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_VARCHAR;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<char*>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_CHAR;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_VARCHAR;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};
	template <>
	struct DBTypes<std::string>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_CHAR;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_VARCHAR;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

#pragma endregion

#pragma region 날짜/시간

	template <>
	struct DBTypes<SQL_TIMESTAMP_STRUCT>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_TYPE_TIMESTAMP;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_TYPE_TIMESTAMP;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 3;
	};

	template <>
	struct DBTypes<SQL_DATE_STRUCT>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_TYPE_DATE;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_TYPE_DATE;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

	template <>
	struct DBTypes<SQL_TIME_STRUCT>
	{
		static constexpr SQLSMALLINT C_TYPE = SQL_C_TYPE_TIME;
		static constexpr SQLSMALLINT SQL_TYPE = SQL_TYPE_TIME;
		static constexpr SQLSMALLINT DECIMAL_DIGITS = 0;
	};

#pragma endregion


}