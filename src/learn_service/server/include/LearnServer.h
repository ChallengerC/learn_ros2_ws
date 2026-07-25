/*
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建服务端；
      3-2.处理请求数据并响应结果。
    4.调用spin函数，并传入节点对象指针；
    5.释放资源。
*/
#ifndef _LEARN_SERVER_HPP_
#define _LEARN_SERVER_HPP_

#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/add_ints.hpp"

namespace learn_service
{
    //
    using AddNumsCallback = std::function<void(const base_interfaces_demo::srv::AddInts_Request::SharedPtr,
                                               base_interfaces_demo::srv::AddInts_Response::SharedPtr)>;
    
    //
    class LearnServer : public rclcpp::Node
    {
    public:
        LearnServer();

        ~LearnServer();

    private:
        void addnums(const base_interfaces_demo::srv::AddInts_Request::SharedPtr  request,
                     base_interfaces_demo::srv::AddInts_Response::SharedPtr response);

    private:
        AddNumsCallback                                                addnums_callback_;
        rclcpp::Service<base_interfaces_demo::srv::AddInts>::SharedPtr server_;
    };
}

#endif // _LEARN_SERVER_HPP_