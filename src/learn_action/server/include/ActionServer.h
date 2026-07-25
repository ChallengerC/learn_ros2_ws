#ifndef _ACTION_SERVER_HPP_
#define _ACTION_SERVER_HPP_

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"
#include <functional>

namespace learn_action
{
    //
    using base_interfaces_demo::action::Progress;

    //
    using GoalCallback = std::function<rclcpp_action::GoalResponse(const rclcpp_action::GoalUUID,
                                                                   std::shared_ptr<const Progress::Goal>)>;

    using CancelCallback = std::function<rclcpp_action::CancelResponse(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>>)>;

    using AcceptedCallback = std::function<void(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>>)>;

    class ActionServer : public rclcpp::Node
    {
    public:
        explicit ActionServer(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

        ~ActionServer();

    private:
        rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &       uuid,
                                                std::shared_ptr<const Progress::Goal> goal);

        rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle);

        void execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle);

        void handle_accepted(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle);

    private:
        GoalCallback                               goal_callback_;
        CancelCallback                             cancel_callback_;
        AcceptedCallback                           accepted_callback_;
        rclcpp_action::Server<Progress>::SharedPtr action_server_;
    };
}

#endif // _ACTION_SERVER_HPP_