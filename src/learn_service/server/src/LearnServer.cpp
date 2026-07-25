#include "LearnServer.h"

namespace learn_service 
{
    LearnServer::LearnServer()
    :rclcpp::Node("topic_server")
    {
        //
        addnums_callback_ = std::bind(&LearnServer::addnums,
                                      this,
                                      std::placeholders::_1,
                                      std::placeholders::_2);
        
        //
        server_ = this->create_service<base_interfaces_demo::srv::AddInts>("add_ints",
                                                                            addnums_callback_);
        //
        RCLCPP_INFO(this->get_logger(), "add_ints服务端启动, 等待请求提交");
    }

    LearnServer::~LearnServer()
    {
        //
        RCLCPP_INFO(this->get_logger(), "服务端释放关闭");
    }

    //
    void LearnServer::addnums(const base_interfaces_demo::srv::AddInts_Request::SharedPtr  request,
                              base_interfaces_demo::srv::AddInts_Response::SharedPtr response)
    {
        response->num3 = request->num1 + request->num2;
        RCLCPP_INFO(this->get_logger(), "请求数据:(%d,%d), 响应结果:%d", 
                    request->num1, request->num2, response->num3);
    }
}