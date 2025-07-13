#include <gtest/gtest.h>

#include <string>
#include <format>
#include <DB/DBQuery.h>
#include <DB/DBConnection.h>
#include <DB/DBConnectionPool.h>

// db 설정 때문에 주석 처리!
// 필요하면 db 입력 후 사용

/*
class DatabaseTest : public ::testing::Test
{
protected:
	hlib::db::DBConnectionPool pool_;

	void SetUp() override
	{
		// db 정보 입력 필요!
		pool_.Connect(5, L"Driver={ODBC Driver 18 for SQL Server};Server=(localdb)\\MSSQLLocalDB;Database=ServerDB;Trusted_Connection=yes;");
	}

	void TearDown() override
	{
	}
};

TEST_F(DatabaseTest, ConnExe)
{
	auto query = L"									\
		DROP TABLE IF EXISTS [dbo].[Test];			\
		CREATE TABLE [dbo].[Test]					\
		(											\
			[id] INT NOT NULL PRIMARY KEY IDENTITY, \
			[name] NVARCHAR(50) NULL,				\
			[lv] INT NOT NULL,						\
			[createDate] DATETIME NULL				\
		);";

	auto conn = pool_.Pop();
	ASSERT_NE(conn, nullptr);

	EXPECT_TRUE(conn->Execute(query));
}

TEST_F(DatabaseTest, Query)
{
	for (int i = 0; i < 5; i++)
	{
		auto conn = pool_.Pop();
		ASSERT_NE(conn, nullptr);

		hlib::db::DBQuery<3, 0> bind(*conn.get(), L"INSERT INTO [dbo].[Test]([name], [lv], [createDate]) VALUES(?, ?, ?)");

		int32_t lv = i + 1;
		std::wstring name = std::format(L"이름{}", i);
		SQL_TIMESTAMP_STRUCT ts = { 2025, 5, (SQLUSMALLINT)(i + 1) };

		ASSERT_NO_THROW(bind.Param(1, name)
						.Param(2, lv)
						.Param(3, ts));

		EXPECT_TRUE(bind.Execute());
	}

	auto conn2 = pool_.Pop();
	ASSERT_NE(conn2, nullptr);

	hlib::db::DBQuery<2, 0> bind2(*conn2.get(), L"UPDATE [dbo].[Test] SET name = ? WHERE id = ?");

	int32_t id = 3;
	ASSERT_NO_THROW(bind2.Param(2, id));
	ASSERT_NO_THROW(bind2.ParamNull<std::wstring>(1));

	EXPECT_TRUE(bind2.Execute());
}


TEST_F(DatabaseTest, Select)
{
	auto conn = pool_.Pop();
	ASSERT_NE(conn, nullptr);

	hlib::db::DBQuery<1, 4> bind(*conn.get(), L"SELECT id, name, lv, createDate FROM [dbo].[Test] WHERE lv > ?");

	int32_t checkLv = 2;
	ASSERT_NO_THROW(bind.Param(1, checkLv));

	int32_t id = 0;
	WCHAR name[50];
	int32_t lv = 0;
	SQL_TIMESTAMP_STRUCT ts = {};
	ASSERT_NO_THROW(bind.Column(1, id));
	ASSERT_NO_THROW(bind.Column(2, name)
					.Column(3, lv)
					.Column(4, ts));

	EXPECT_TRUE(bind.Execute());

	while (bind.Fetch())
	{
		EXPECT_GT(lv, checkLv);
	}
}

*/