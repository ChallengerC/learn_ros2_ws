#include "LearnClient.h"

namespace learn_service
{
    LearnClient::LearnClient()
    :rclcpp::Node("topic_client")
    {
        client_ = this->create_client<base_interfaces_demo::srv::AddInts>("add_ints");
        RCLCPP_INFO(this->get_logger(),"客户端创建，等待连接服务端！");
    }

    LearnClient::~LearnClient()
    {
    }

    //
    bool LearnClient::connect_server()
    {
        while (!client_->wait_for_service(std::chrono::seconds(1)))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"强制退出！");
                return false;
            }

            RCLCPP_INFO(this->get_logger(),"服务连接中，请稍候...");
        }
        return true;
    }

    //
    rclcpp::Client<base_interfaces_demo::srv::AddInts>::FutureAndRequestId 
    LearnClient::send_request(int32_t num1, int32_t num2)
    {
        //
        auto request = std::make_shared<base_interfaces_demo::srv::AddInts::Request>();
        {
            request->num1 = num1;
            request->num2 = num2;
        }

        return client_->async_send_request(request);
    }

}
