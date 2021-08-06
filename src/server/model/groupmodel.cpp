#include "groupmodel.hpp"
#include "mysql.h"

//创建群组
bool GroupModel::createGroup(Group &group)
{
    //1 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into AllGroup(groupname,groupdesc) values('%s', '%s')", group.getName().c_str(), group.getDesc().c_str());
    MySQL mysql;
    //是否连接成功
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            //获取插入成功的用户数据生成的主键id
            group.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}
//加入群组
void GroupModel::addGroup(int userid, int groupid, string role)
{
    //1 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into GroupUser values(%d, %d, '%s')", groupid, userid, role.c_str());
    MySQL mysql;
    //是否连接成功
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}
//查询用户所在的群组消息
vector<Group> GroupModel::queryGroups(int userid)
{
    /*
        1. 先根据userid在GroupUser表中查出该用户属于所有的群组，获取其所属群组的信息
        2. 再根据每一个所获取的群组信息，查询属于该群组的所有用户的信息，这里需要用到GroupUser和User表进行多表联合查询
    */
    //1 组装sql语句
    char sql[1024] = {0};
    //多表联合查询
    sprintf(sql, "select a.id,a.groupname,a.groupdesc from AllGroup a inner join \
                    GroupUser b on a.id = b.groupid where b.userid = %d",
            userid);
    MySQL mysql;
    vector<Group> groupVec;
    //是否连接成功
    if (mysql.connect())
    {
        //需要释放资源res指针
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            //离线消息有可能有很多条
            MYSQL_ROW row;
            //将userid用户的所有离线消息放入vec中返回
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                Group group;
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                groupVec.push_back(group);
            }
            //释放mysql的指针资源
            mysql_free_result(res);
        }
    }

    //查询群组里面所包含的用户信息
    for (Group &group : groupVec)
    {
        //多表联合查询
        sprintf(sql, "select a.id,a.name,a.state,b.grouprole from User a inner join \
                    GroupUser b on b.userid = a.id where b.groupid = %d",
                group.getId());
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            //离线消息有可能有很多条
            MYSQL_ROW row;
            //将userid用户的所有离线消息放入vec中返回
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                GroupUser user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                user.setRole(row[3]);
                group.getUsers().push_back(user);
            }
            //释放mysql的指针资源
            mysql_free_result(res);
        }
    }

    return groupVec;
}
//根据指定的groupid查询群组中所有的用户id列表
//最后除了userid自己，其他用户都需要被发消息，这一块主要是用户群聊业务中给其他成员发消息
vector<int> GroupModel::queryGroupUsers(int userid, int groupid)
{
    char sql[1024] = {0};
    //多表联合查询
    sprintf(sql, "select userid from GroupUser where groupid = %d and userid != %d", groupid, userid);
    MySQL mysql;
    vector<int> idVec;
    //是否连接成功
    if (mysql.connect())
    {
        //需要释放资源res指针
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            //离线消息有可能有很多条
            MYSQL_ROW row;
            //将userid用户的所有离线消息放入vec中返回
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                idVec.push_back(atoi(row[0]));
            }
            //释放mysql的指针资源
            mysql_free_result(res);
        }
    }
    return idVec;
}
