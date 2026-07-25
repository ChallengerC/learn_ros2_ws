#ifndef _LEARN_CLIENT_HPP_
#define _LEARN_CLIENT_HPP_

#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/add_ints.hpp"

namespace learn_service
{
    class LearnClient : public rclcpp::Node
    {
    public:
        LearnClient();

        ~LearnClient();

        //
        bool connect_server();

        //
        rclcpp::Client<base_interfaces_demo::srv::AddInts>::FutureAndRequestId send_request(int32_t num1, 
                                                                                            int32_t num2);

    private:
        rclcpp::Client<base_interfaces_demo::srv::AddInts>::SharedPtr client_;
    };
}

#endif // _LEARN_CLIENT_HPP_
