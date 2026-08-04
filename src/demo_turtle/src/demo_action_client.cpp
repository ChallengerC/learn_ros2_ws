#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/nav.hpp"
#include "turtlesim/msg/pose.hpp"
#include "turtlesim/srv/spawn.hpp"
#include "geometry_msgs/msg/twist.hpp"

using base_interfaces_demo::action::Nav;

class NavActionClient : public rclcpp::Node
{
public:
    NavActionClient(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
    : Node("nav_action_client", options)
    {
        nav_action_client_ = rclcpp_action::create_client<Nav>(this, "nav");
    }

public:
    void SendGoal(float goal_x, float goal_y, float goal_theta)
    {
        if (!nav_action_client_->wait_for_action_server(std::chrono::seconds(10))) 
        {
            RCLCPP_ERROR(this->get_logger(), "无法连接到导航动作服务器！");
            return;
        }

        auto goal_msg = Nav::Goal();
        goal_msg.goal_x     = goal_x;
        goal_msg.goal_y     = goal_y;
        goal_msg.goal_theta = goal_theta;

        RCLCPP_INFO(this->get_logger(), "发送导航目标: x=%.2f, y=%.2f, theta=%.2f", 
                    goal_x, goal_y, goal_theta);

        auto send_goal_options = rclcpp_action::Client<Nav>::SendGoalOptions();

        //
        send_goal_options.goal_response_callback = std::bind(&NavActionClient::goal_response_callback, this, std::placeholders::_1);
        send_goal_options.feedback_callback      = std::bind(&NavActionClient::feedback_callback, this, std::placeholders::_1, std::placeholders::_2);
        send_goal_options.result_callback        = std::bind(&NavActionClient::result_callback, this, std::placeholders::_1);

        nav_action_client_->async_send_goal(goal_msg, send_goal_options);
    }

    void goal_response_callback(rclcpp_action::ClientGoalHandle<Nav>::SharedPtr goal_handle)
    {
        if (!goal_handle) 
        {
            RCLCPP_ERROR(this->get_logger(), "导航目标被拒绝！");
        } 
        else 
        {
            RCLCPP_INFO(this->get_logger(), "导航目标已接受，等待结果...");
        }
    }

    void feedback_callback(rclcpp_action::ClientGoalHandle<Nav>::SharedPtr,
                           const std::shared_ptr<const Nav::Feedback> feedback)
    {
        RCLCPP_INFO(this->get_logger(), "当前距离目标的距离: %.2f", feedback->distance);
    }

    void result_callback(const rclcpp_action::ClientGoalHandle<Nav>::WrappedResult & result)
    {
        switch (result.code) 
        {
            case rclcpp_action::ResultCode::SUCCEEDED:
                RCLCPP_INFO(this->get_logger(), "导航任务成功完成!");
                break;
            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_ERROR(this->get_logger(), "导航任务被中止");
                return;
            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_ERROR(this->get_logger(), "导航任务被取消");
                return;
            default:
                RCLCPP_ERROR(this->get_logger(), "未知的结果代码");
                return;

            RCLCPP_INFO(this->get_logger(), "最终乌龟位置: x=%.2f, y=%.2f, theta=%.2f", 
                    result.result->turtle_x, result.result->turtle_y, result.result->turtle_theta);
        }
    }

private:
    rclcpp_action::Client<Nav>::SharedPtr nav_action_client_;
};

int main(int argc, const char ** argv)
{
    rclcpp::init(argc, argv);

    auto client_node = std::make_shared<NavActionClient>();

    //
    client_node->SendGoal(std::stof(argv[1]), std::stof(argv[2]), std::stof(argv[3]));

    //
    rclcpp::spin(client_node);

    //
    rclcpp::shutdown();

    return 0;
}