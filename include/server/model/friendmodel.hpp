#ifndef FRINEDMODEL_H
#define FRINEDMODEL_H

#include "user.hpp"
#include <vector>
using namespace std;

//维护好友信息的操作接口方法
class FriendModel
{
public:
    //添加好友关系
    void insert(int userid, int friendid);

    //返回用户好友列表 包括friendid和该id对应的信息 需要做User表和Friend表的联合查询
    vector<User> query(int friendid);
};

#endif