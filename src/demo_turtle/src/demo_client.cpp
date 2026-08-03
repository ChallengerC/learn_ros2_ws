#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/distance.hpp"
#include "turtlesim/msg/pose.hpp"
#include "turtlesim/srv/spawn.hpp"

class DistanceClient : public rclcpp::Node
{
public:
    DistanceClient() : Node("distance_client")
    {
        //
        distance_client_ = this->create_client<base_interfaces_demo::srv::Distance>("distance");
    }

public:
    void SendPose(float x, float y, float theta)
    {
        //
        auto request = std::make_shared<base_interfaces_demo::srv::Distance::Request>();
        request->x = x;
        request->y = y;
        request->theta = theta;

        //
        while (!distance_client_->wait_for_service(std::chrono::seconds(1))) {
            if (!rclcpp::ok()) {
                RCLCPP_ERROR(this->get_logger(), "服务未启动，客户端退出");
                return;
            }
            RCLCPP_INFO(this->get_logger(), "等待服务启动...");
        }

        //
        auto result_future = distance_client_->async_send_request(request);

        // 等待结果
        if (rclcpp::FutureReturnCode::SUCCESS == 
            rclcpp::spin_until_future_complete(this->get_node_base_interface(), result_future)) 
        {
            auto response = result_future.get();
            RCLCPP_INFO(this->get_logger(), "乌龟1与乌龟2的距离: %.2f", response->distance);
        } else 
        {
            RCLCPP_ERROR(this->get_logger(), "服务调用失败");
        }
    }

private:
    rclcpp::Client<base_interfaces_demo::srv::Distance>::SharedPtr distance_client_;
};

int main(int argc, char** argv)
{
    //
    rclcpp::init(argc, argv);

    //
    auto distance_client = std::make_shared<DistanceClient>();

    // 发送乌龟的坐标
    float x     = atof(argv[1]);
    float y     = atof(argv[2]);
    float theta = atof(argv[3]);

    //
    distance_client->SendPose(x, y, theta);

    //
    rclcpp::shutdown();

    return 0;
}