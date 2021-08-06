#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include "group.hpp"
#include <string>
#include <vector>
using namespace std;

//维护群组信息的操作接口方法
class GroupModel
{
public:
    //创建群组
    bool createGroup(Group &group);
    //加入群组
    void addGroup(int userid, int groupid, string role);
    //查询用户所在的群组消息
    vector<Group> queryGroups(int userid);
    //根据指定的groupid查询群组中所有的用户id列表
    //最后除了userid自己，其他用户都需要被发消息，这一块主要是用户群聊业务中给其他成员发消息
    vector<int> queryGroupUsers(int userid, int groupid);
};

#endif