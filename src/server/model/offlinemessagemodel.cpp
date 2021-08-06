#include "offlinemessagemodel.hpp"
#include "mysql.h"

//存储用户的离线消息
void OfflineMsgModel::insert(int userid, string msg)
{
    //1 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into OfflineMessage values(%d, '%s')", userid, msg.c_str());
    MySQL mysql;
    //是否连接成功
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}

//删除用户的离线消息
void OfflineMsgModel::remove(int userid)
{
    //1 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "delete from OfflineMessage where userid = %d", userid);
    MySQL mysql;
    //是否连接成功
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}

//查询用户的离线消息
vector<string> OfflineMsgModel::query(int userid)
{
    //1 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select message from OfflineMessage where userid = %d", userid);
    MySQL mysql;
    vector<string> vec;
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
                //因为只有一个字段消息，所以这里面采用row[0]
                vec.push_back(row[0]);
            }
            //释放mysql的指针资源
            mysql_free_result(res);
            return vec;
        }
    }
    //如果没有找到返回空
    return vec;
}