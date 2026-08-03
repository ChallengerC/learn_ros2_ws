#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/distance.hpp"
#include "turtlesim/msg/pose.hpp"
#include "turtlesim/srv/spawn.hpp"

class DistanceServer : public rclcpp::Node
{
public:
    DistanceServer() : Node("distance_server")
    {
        //
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>("turtle1/pose", 10, 
            std::bind(&DistanceServer::pose_callback, this, std::placeholders::_1));

        service_ = this->create_service<base_interfaces_demo::srv::Distance>("distance", 
            std::bind(&DistanceServer::calculate_distance, this, std::placeholders::_1, std::placeholders::_2));
    }

private:
    // 
    void pose_callback(const turtlesim::msg::Pose::SharedPtr msg)
    {
        //
        turtle1_x_ = msg->x;
        turtle1_y_ = msg->y;

        //
        RCLCPP_INFO(this->get_logger(), "乌龟1的位置坐标: x=%.2f, y=%.2f", msg->x, msg->y);
    }

    // 距离计算
    void calculate_distance(const std::shared_ptr<base_interfaces_demo::srv::Distance::Request> request,
                            std::shared_ptr<base_interfaces_demo::srv::Distance::Response>      response)
    {
        // Calculate the distance between two turtles based on their positions
        //
        float x1 = turtle1_x_;
        float y1 = turtle1_y_;
        
        //
        float x2 = request->x;
        float y2 = request->y;

        response->distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));

        //
        RCLCPP_INFO(this->get_logger(), "计算乌龟1与乌龟2的距离: %.2f", response->distance);
    }

private:
    float turtle1_x_;
    float turtle1_y_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
    rclcpp::Service<base_interfaces_demo::srv::Distance>::SharedPtr service_;
};

int main(int argc, char** argv)
{
    //
    rclcpp::init(argc, argv);

    //
    rclcpp::spin(std::make_shared<DistanceServer>());

    //
    rclcpp::shutdown();

    
    return 0;
}