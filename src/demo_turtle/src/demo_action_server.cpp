#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/nav.hpp"
#include "turtlesim/msg/pose.hpp"
#include "turtlesim/srv/spawn.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <thread>

using base_interfaces_demo::action::Nav;

class NavActionServer : public rclcpp::Node
{
public:
    NavActionServer(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
    : Node("nav_action_server", options)
    {
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>("turtle1/pose", 10, 
            std::bind(&NavActionServer::pose_callback, this, std::placeholders::_1));

        cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);

        nav_action_server_ = rclcpp_action::create_server<Nav>(
            this,
            "nav",
            std::bind(&NavActionServer::handle_goal,     this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&NavActionServer::handle_cancel,   this, std::placeholders::_1),
            std::bind(&NavActionServer::handle_accepted, this, std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "nav_action_server 启动成功，等待导航目标...");
    }

private:
    //
    void pose_callback(const turtlesim::msg::Pose::SharedPtr msg)
    {
        // 更新乌龟1的位置坐标
        turtle1_pose = msg;
        RCLCPP_INFO(this->get_logger(), "乌龟1的位置坐标: x=%.2f, y=%.2f, theta=%.2f", 
        msg->x, msg->y, msg->theta);
    }

    rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &  uuid,
                                            std::shared_ptr<const Nav::Goal> goal)
    {
        (void)uuid;
        RCLCPP_INFO(this->get_logger(), "接收到导航目标: x=%.2f, y=%.2f, theta=%.2f", 
        goal->goal_x, goal->goal_y, goal->goal_theta);

        //
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle)
    {
        (void)goal_handle;
        RCLCPP_INFO(this->get_logger(), "接收到取消导航请求");
        return rclcpp_action::CancelResponse::ACCEPT;
    }

    void execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle)
    {
        RCLCPP_INFO(this->get_logger(), "开始执行导航任务...");

        //
        float goal_x = goal_handle->get_goal()->goal_x;
        float goal_y = goal_handle->get_goal()->goal_y;

        //
        auto feedback = std::make_shared<Nav::Feedback>();
        auto result = std::make_shared<Nav::Result>();

        //
        rclcpp::Rate rate(1.0);
        while(true)
        {
            if (goal_handle->is_canceling()) 
            {
                result->turtle_x     = turtle1_pose->x;
                result->turtle_y     = turtle1_pose->y;
                result->turtle_theta = turtle1_pose->theta;
                goal_handle->canceled(result);
                RCLCPP_INFO(this->get_logger(), "导航任务已取消");
                return;
            }

            // 位姿
            float turtle1_x     = turtle1_pose->x;
            float turtle1_y     = turtle1_pose->y;
            float turtle1_theta = turtle1_pose->theta;

            // 计算距离和角度
            float x_distance = goal_x - turtle1_x;
            float y_distance = goal_y - turtle1_y;
            float distance   = std::sqrt(x_distance * x_distance + y_distance * y_distance);

            // 目标方向角
            float target_angle = std::atan2(y_distance, x_distance);
            // 角度误差（归一化到 [-pi, pi]）
            float angle_error = target_angle - turtle1_theta;
            while (angle_error >  M_PI) angle_error -= 2 * M_PI;
            while (angle_error < -M_PI) angle_error += 2 * M_PI;

            // 速度控制：turtlesim 只认 linear.x(前进) 和 angular.z(转向)
            geometry_msgs::msg::Twist twist;
            double scale =0.5;
            twist.linear.x  = scale * distance;        // 距离越远，前进越快
            twist.angular.z = scale * angle_error;      // 角度偏差越大，转得越快
            cmd_vel_pub_->publish(twist);
            
            //
            if(distance < 0.1)
            {
                result->turtle_x     = turtle1_pose->x;
                result->turtle_y     = turtle1_pose->y;
                result->turtle_theta = turtle1_pose->theta;
                goal_handle->succeed(result);
                RCLCPP_INFO(this->get_logger(), "导航任务已完成");
                return;
            }

            feedback->distance = distance;

            //
            goal_handle->publish_feedback(feedback);

            rate.sleep();
        }
    }

    void handle_accepted(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle)
    {
        std::thread{std::bind(&NavActionServer::execute,this,std::placeholders::_1),
            goal_handle}.detach();
    }


private:
    turtlesim::msg::Pose::SharedPtr                         turtle1_pose = nullptr;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr   pose_sub_;
    rclcpp_action::Server<Nav>::SharedPtr                   nav_action_server_;
};

int main(int argc, char const * argv[])
{
    //
    rclcpp::init(argc, argv);

    //
    auto node = std::make_shared<NavActionServer>();

    //
    rclcpp::spin(node);

    //
    rclcpp::shutdown();

    //
    return 0;
}