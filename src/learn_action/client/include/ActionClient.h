#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"

namespace learn_action
{
    //
    using base_interfaces_demo::action::Progress;

    //
    class ActionClient : public rclcpp::Node
    {
    public:
        explicit ActionClient(const rclcpp::NodeOptions & node_options = rclcpp::NodeOptions());

        ~ActionClient() = default;

    public:
        void Send_goal(int64_t num);

    private:
        //
        void goal_response_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle);

        //
        void result_callback(const rclcpp_action::ClientGoalHandle<Progress>::WrappedResult & result);

        //
        void feedback_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr,
                                const std::shared_ptr<const Progress::Feedback> feedback);

    private:
        rclcpp_action::Client<Progress>::SharedPtr client_ptr_;
    };
}