#include "LearnClient.h"
#include <memory>

int main(int argc, char* argv[])
{
    //
    rclcpp::init(argc, argv);

    //
    auto client = std::make_shared<learn_service::LearnClient>();

    //
    bool connected = client->connect_server();
    if (!connected)
    {
        RCLCPP_WARN(client->get_logger(), "连接服务端失败！");
        rclcpp::shutdown();
        return 1;
    }

    //
    auto future = client->send_request(8, 1);

    //
    if (rclcpp::FutureReturnCode::SUCCESS == rclcpp::spin_until_future_complete(client, future))
    {
        RCLCPP_INFO(client->get_logger(), "响应结果:%d", future.get()->num3);
    }
    else
    {
        RCLCPP_ERROR(client->get_logger(), "请求失败！");
    }

    //
    rclcpp::shutdown();

    return 0;
}
