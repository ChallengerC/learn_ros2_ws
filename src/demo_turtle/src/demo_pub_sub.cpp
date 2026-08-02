#include <rclcpp/rclcpp.hpp>
#include <turtlesim/msg/pose.hpp>
#include <geometry_msgs/msg/twist.hpp>

class PosePublisher : public rclcpp::Node
{
public:
    PosePublisher() : Node("pose_publisher")
    {
        twist_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/t2/turtle1/cmd_vel", 10);
    }

public:
    void publishPose(const turtlesim::msg::Pose::SharedPtr pose)
    {
        geometry_msgs::msg::Twist twist;
        twist.angular.z = -(pose->angular_velocity); //角速度取反
        twist.linear.x  = pose->linear_velocity;      //线速度不变
        twist_pub_->publish(twist);
    }

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr twist_pub_;
};

class PoseSubscriber : public rclcpp::Node
{
public:
    PoseSubscriber() : Node("pose_subscriber")
    {
        //
        twist_pub_ = std::make_shared<PosePublisher>();

        //
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>(
            "turtle1/pose", 10, std::bind(&PoseSubscriber::poseCallback,
                                          this, 
                                          std::placeholders::_1));
    }

private:
    void poseCallback(const turtlesim::msg::Pose::SharedPtr msg)
    {
        //
        twist_pub_->publishPose(msg);
        RCLCPP_INFO(this->get_logger(), "收到位置订阅: x=%.2f, y=%.2f, theta=%.2f, linear_velocity=%.2f, angular_velocity=%.2f",
                    msg->x, msg->y, msg->theta, msg->linear_velocity, msg->angular_velocity);
    }


private:
    std::shared_ptr<PosePublisher> twist_pub_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
};


int main(int argc, char** argv)
{
    //
    rclcpp::init(argc, argv);

    //
    auto publisher  = std::make_shared<PosePublisher>();
    auto subscriber = std::make_shared<PoseSubscriber>();

    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(publisher);
    executor.add_node(subscriber);
    executor.spin();

    rclcpp::shutdown();
    return 0;
}