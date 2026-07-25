#include "../include/ActionClient.h"

//
namespace learn_action 
{
    //
    ActionClient::ActionClient(const rclcpp::NodeOptions & node_options)
    :rclcpp::Node("action_client", node_options)
    {
        this->client_ptr_ = rclcpp_action::create_client<Progress>(this,"get_sum");
    }

    //
    void ActionClient::Send_goal(int64_t num)
    {
        if (!this->client_ptr_) 
        {
            RCLCPP_ERROR(this->get_logger(), "动作客户端未被初始化。");
        }

        if (!this->client_ptr_->wait_for_action_server(std::chrono::seconds(10))) 
        {
            RCLCPP_ERROR(this->get_logger(), "服务连接失败！");
            return;
        }

        auto goal_msg = Progress::Goal();
        goal_msg.num  = num;
        RCLCPP_INFO(this->get_logger(), "发送请求数据！");

        //
        auto send_goal_options = rclcpp_action::Client<Progress>::SendGoalOptions();
        {
            //
            send_goal_options.goal_response_callback =std::bind(&ActionClient::goal_response_callback, 
                                                                this, 
                                                                std::placeholders::_1);

            send_goal_options.feedback_callback =std::bind(&ActionClient::feedback_callback, 
                                                        this, 
                                                        std::placeholders::_1, 
                                                        std::placeholders::_2);

            send_goal_options.result_callback =std::bind(&ActionClient::result_callback,
                                                        this, 
                                                        std::placeholders::_1);
        }

        //
        auto goal_handle_future = this->client_ptr_->async_send_goal(goal_msg, send_goal_options);
    }

    void ActionClient::goal_response_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle)
    {
        if (!goal_handle) 
        {
            RCLCPP_ERROR(this->get_logger(), "目标请求被服务器拒绝！");
        } 
        else 
        {
            RCLCPP_INFO(this->get_logger(), "目标被接收，等待结果中");
        }
    }

    void ActionClient::result_callback(const rclcpp_action::ClientGoalHandle<Progress>::WrappedResult & result)
    {
        switch (result.code) 
        {
            case rclcpp_action::ResultCode::SUCCEEDED:
                break;
            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_ERROR(this->get_logger(), "任务被中止");
                return;
            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_ERROR(this->get_logger(), "任务被取消");
                return;
            default:
                RCLCPP_ERROR(this->get_logger(), "未知异常");
                return;
        }

        RCLCPP_INFO(this->get_logger(), "任务执行完毕，最终结果: %ld", result.result->sum);
    }

    void ActionClient::feedback_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr,
                                         const std::shared_ptr<const Progress::Feedback> feedback)
    {
        //
        int32_t progress = (int32_t)(feedback->progress * 100);
        RCLCPP_INFO(this->get_logger(), "当前进度: %d%%", progress);
    }
}