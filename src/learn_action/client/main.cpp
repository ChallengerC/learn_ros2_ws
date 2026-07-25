#include "ActionClient.h"
#include <memory>

int main(int argc, const char ** argv)
{
    //
    rclcpp::init(argc, argv);

    // 
    auto action_client = std::make_shared<learn_action::ActionClient>();
    action_client->Send_goal(10);

    //
    rclcpp::spin(action_client);

    //
    rclcpp::shutdown();

    return 0;
}