#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

//json序列化示例1 键值对
void func1()
{
    json js;
    js["msg_type"] = 2;
    js["from"] = "sugary";
    js["to"] = "zhurui";
    js["msg"] = "I Love U";
    string sendmsg = js.dump();
    cout << sendmsg.c_str() << endl;
}

//json示例代码2 数组
string func2()
{
    json js; // 添加数组
    js["id"] = {1, 2, 3, 4, 5};
    // 添加key-value
    js["name"] = "zhang san";
    // 添加对象
    js["msg"]["zhang san"] = "hello world";
    js["msg"]["liu shuo"] = "hello china";
    // 上面等同于下面这句一次性添加数组对象
    js["msg"] = {{"zhang san", "hello world"}, {"liu shuo", "hello china"}};
    return js.dump();
}

//json示例代码3 容器
string func3()
{
    json js;
    // 直接序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;
    // 直接序列化一个map容器
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;
    string sendBuff = js.dump();
    return sendBuff;
    //cout<<js<<endl;
}

int main()
{
    //func1();
    //func2();
    // string recvBuf = func3();
    // json jsbuf = json::parse(recvBuf);
    // vector<int> vec=jsbuf["list"];
    // for(int& v:vec){
    //     cout<<v<<" ";
    // }
    // cout << jsbuf["list"] << endl;
    string recvBuf=func2();
    json jsbuf=json::parse(recvBuf);
    auto arr=jsbuf["id"];
    cout<<arr[2]<<endl;

    return 0;
}