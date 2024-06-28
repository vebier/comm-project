#include "MysqlDao.h"
#include"ConfigMgr.h"

MysqlDao::~MysqlDao()
{
	_pool->Close();
}

int MysqlDao::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
    auto con = _pool->GetConection();
    try
    {
        if (con == nullptr)
        {
            return false;
        }
        //准备调用存储过程
        std::unique_ptr<sql::PreparedStatement> stmt(con->_con->prepareStatement("CALL reg_user(?,?,?,@result)"));
        //手动存储
        stmt->setString(1, name);
        stmt->setString(2, email);
        stmt->setString(3, pwd);
        //执行存储过程
        stmt->execute();

        // 如果存储过程设置了会话变量或有其他方式获取输出参数的值，你可以在这里执行SELECT查询来获取它们
        // 例如，如果存储过程设置了一个会话变量@result来存储输出结果，可以这样获取：
            std::unique_ptr<sql::Statement> stmtResult(con->_con->createStatement());
            std::unique_ptr<sql::ResultSet> res(stmtResult->executeQuery("SELECT @result AS result")); 
            if (res->next()) {
            int result = res->getInt("result");
            std::cout << "Result: " << result << std::endl;
            _pool->RetConection(std::move(con));
            return result;
        }
        _pool->RetConection(std::move(con));
        return -1;
    }
    catch (sql::SQLException& e)
    {
        _pool->RetConection(std::move(con));
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return -1;
    }

}

MysqlDao::MysqlDao()
{
    auto& cfg = ConfigMgr::GetIns();
    const auto& host = cfg["Mysql"]["Host"];
    const auto& port = cfg["Mysql"]["Port"];
    const auto& pwd = cfg["Mysql"]["Passwd"];
    const auto& schema = cfg["Mysql"]["Schema"];
    const auto& user = cfg["Mysql"]["User"];
    _pool.reset(new SqlPool(host + ":" + port, user, pwd, schema, 5));
}
