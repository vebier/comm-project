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
            _pool->RetConection(std::move(con));
            return false;
        }
        //׼�����ô洢����
        std::unique_ptr<sql::PreparedStatement> stmt(con->_con->prepareStatement("CALL reg_user(?,?,?,@result)"));
        //�ֶ��洢
        stmt->setString(1, name);
        stmt->setString(2, email);
        stmt->setString(3, pwd);
        //ִ�д洢����
        stmt->execute();

        // ����洢���������˻Ự��������������ʽ��ȡ���������ֵ�������������ִ��SELECT��ѯ����ȡ����
        // ���磬����洢����������һ���Ự����@result���洢������������������ȡ��
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

bool MysqlDao::CheckEmail(const std::string& name, const std::string& email)
{
    auto con = _pool->GetConection();
    try
    {
        if (con == nullptr)
        {
            _pool->RetConection(std::move(con));
            return false;
        }
        //׼�����ô洢����
        std::unique_ptr<sql::PreparedStatement> stmt(con->_con->prepareStatement("SELECT email FROM user WHERE name = ?"));
        stmt->setString(1,name);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        while (res->next()) {
            std::cout << "Check Email: " << res->getString("email") << std::endl;
            if (email != res->getString("email")) {
                _pool->RetConection(std::move(con));
                return false;
            }
            _pool->RetConection(std::move(con));
            return true;
        }
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

bool MysqlDao::UpdatePwd(const std::string& name, const std::string& newpwd)
{
    auto con = _pool->GetConection();
    try {
        if (con == nullptr)
        {
            _pool->RetConection(std::move(con));
            return false;
        }
        // ׼����ѯ���
       
        std::unique_ptr<sql::PreparedStatement> stmt(con->_con->prepareStatement("UPDATE user SET pwd = ? WHERE name = ?"));
        // �󶨲���
        stmt->setString(2, name);
        stmt->setString(1, newpwd);
        // ִ�и���
        int updateCount = stmt->executeUpdate();
        std::cout << "Updated rows: " << updateCount << std::endl;
        _pool->RetConection(std::move(con));
        return true;
    }
    catch (sql::SQLException& e) {
        _pool->RetConection(std::move(con));
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
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
