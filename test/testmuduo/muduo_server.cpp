/*
muduo网络库给用户提供了两个主要的类：
TcpServer：用于编写服务器程序的
TcpClient：用于编写客户端程序的


epoll + 线程池
好处：
能够把网络I/O的代码和业务代码区分开
                    业务代码主要暴露：用户的连接与断开  用户的可读写事件


*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

//基于muduo网络库开发服务器程序
/*
1. 组合TcpServer对象
2. 创建EvenetLoop事件循环对象的指针
3. 明确TcpServer构造函数需要哪些参数，输出ChatServer的构造函数
4. 在当前服务器类的构造函数中，注册处理连接的回调函数和处理读写事件的回调函数
5. 设置合适的服务端线程数量，muduo会自己分配I/O线程和worker线程的数量
*/

class ChatServer
{
public:
    ChatServer(EventLoop *loop,               //事件循环
               const InetAddress &listenAddr, //IP+port
               const string &nameArg)         //服务器的名字
        : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        /*
            回调：用户的发生时间点和发生以后该做什么事情不在同一事件段
        */
        //给服务器注册用户连接的创建和断开的回调
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
        //这里的_1是占位符，是在placeholders命名空间里面的
        //用绑定器给方法onConnection绑定参数包括this和const TcpConnectionPtr&
        //传this是因为这是一个成员方法，所以会自动加上一个this，用_1占位符代表const TcpConnectionPtr&

        //给服务器注册用户读写事件回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

        //设置服务器端的线程数量
        _server.setThreadNum(4);
        //自动拿出一个线程进行处理用户的连接，其余线程处理用户读写，即一个I/O线程，三个Worker线程
    }

    //开启时间循环
    void start()
    {
        _server.start();
    }

private:
    // 专门处理用户的连接创建和断开 epoll listenfd accept
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            //上线了
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:online" << endl;
        }
        else
        {
            //下线了
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:offline" << endl;
            //一般进入telnet交互命令行快捷键是ctrl+]，然后输入quit退出
            conn->shutdown();//连接断开，回收服务器中该连接的fd资源，即close(fd)
            //_loop->quit()
        }
    }
    // 专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   Timestamp time)               // 接收到数据的时间信息
    {
        //缓冲区中接收的数据放在字符串中
        string buf = buffer->retrieveAllAsString();
        cout << "recv:data:" << buf << "time:" << time.toString() << endl;
        //将接收到的数据返回给服务器
        conn->send(buf);
    }
    /*
        //TcpServer(EventLoop* loop, const InetAddress& listenAddr);
        TcpServer(EventLoop* loop,
                    const InetAddress& listenAddr,
                    const string& nameArg,
                    Option option = kNoReusePort);
    */
    TcpServer _server; //#1
    EventLoop *_loop;  //#2 epoll
};

int main()
{
    EventLoop loop; //epoll
    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop, addr, "ChatServer");

    server.start(); //启动服务  将listenfd通过epoll_ctl添加到epoll上面去
    loop.loop();    //最后调用了epoll_wait，以阻塞的方式等待新用户的连接，处理已连接用户的读写时间等
}