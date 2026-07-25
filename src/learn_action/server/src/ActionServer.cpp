#include "ActionServer.h"
#include <thread>

//
namespace learn_action
{
    ActionServer::ActionServer(const rclcpp::NodeOptions & options)
    :rclcpp::Node("action_server", options)
    {
        //
        goal_callback_ = std::bind(&ActionServer::handle_goal, 
                                   this,
                                   std::placeholders::_1,
                                   std::placeholders::_2);
        
        cancel_callback_ = std::bind(&ActionServer::handle_cancel, 
                                     this,
                                     std::placeholders::_1);

        accepted_callback_ = std::bind(&ActionServer::handle_accepted, 
                                   this,
                                   std::placeholders::_1);

        //
        this->action_server_ = rclcpp_action::create_server<Progress>(this,
                                                                      "get_sum",
                                                                      goal_callback_,
                                                                      cancel_callback_,
                                                                      accepted_callback_);
        RCLCPP_INFO(this->get_logger(),"动作服务端创建，等待请求...");
    }
    

    //
    ActionServer::~ActionServer()
    {
    }

    //
    rclcpp_action::GoalResponse
    ActionServer::handle_goal(const rclcpp_action::GoalUUID &       uuid,
                              std::shared_ptr<const Progress::Goal> goal)
    {
        (void)uuid;
        RCLCPP_INFO(this->get_logger(), "接收到动作客户端请求，请求数字为 %ld", goal->num);

        //
        if (goal->num < 1) 
        {
            return rclcpp_action::GoalResponse::REJECT;
        }

        //
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    //
    rclcpp_action::CancelResponse 
    ActionServer::handle_cancel(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle)
    {
        (void)goal_handle;
        RCLCPP_INFO(this->get_logger(), "接收到任务取消请求");
        return rclcpp_action::CancelResponse::ACCEPT;
    }

    //
    void ActionServer::execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle)
    {
        //
        RCLCPP_INFO(this->get_logger(), "开始执行任务");

        rclcpp::Rate loop_rate(10.0);

        //
        const auto goal = goal_handle->get_goal();
        auto feedback   = std::make_shared<Progress::Feedback>();
        auto result     = std::make_shared<Progress::Result>();

        //
        int64_t sum = 0;
        for(int i = 1; (i<= goal->num) && rclcpp::ok(); i++)
        {
            sum += i;

            if(goal_handle->is_canceling())
            {
                result->sum = sum;
                goal_handle->canceled(result);
                RCLCPP_INFO(this->get_logger(), "任务取消");
                return;
            }

            feedback->progress = (double_t)i / goal->num;
            goal_handle->publish_feedback(feedback);
            RCLCPP_INFO(this->get_logger(), "连续反馈中，进度：%.2f", feedback->progress);

            loop_rate.sleep();
        }

        if (rclcpp::ok()) 
        {
            result->sum = sum;
            goal_handle->succeed(result);
            RCLCPP_INFO(this->get_logger(), "任务完成！");
        }
    }

    //
    void ActionServer::handle_accepted(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle)
    {
        //
        std::thread{std::bind(&ActionServer::execute, 
                              this,
                              std::placeholders::_1), goal_handle}.detach();
    }
}