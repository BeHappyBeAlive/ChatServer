#include "friendmodel.hpp"
#include "mysql.h"


//添加好友关系
void FriendModel::insert(int userid, int friendid)
{
    //1 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into Friend values(%d, %d)", userid, friendid);
    MySQL mysql;
    //是否连接成功
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}

//返回用户好友列表 包括friendid和该id对应的信息 需要做User表和Friend表的联合查询
vector<User> FriendModel::query(int userid)
{
     //1 组装sql语句
    char sql[1024] = {0};
    //多表联合查询
    sprintf(sql, "select a.id,a.name,a.state from User a inner join Friend b on b.friendid = a.id where b.userid = %d", userid);
    MySQL mysql;
    vector<User> vec;
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
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
            //释放mysql的指针资源
            mysql_free_result(res);
            return vec;
        }
    }
    //如果没有找到返回空
    return vec;
}