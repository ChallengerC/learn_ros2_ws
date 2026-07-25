#include "ActionServer.h"
#include <memory>

int main(int argc, const char ** argv)
{
    rclcpp::init(argc, argv);

    //
    auto action_server = std::make_shared<learn_action::ActionServer>();
    rclcpp::spin(action_server);

    //
    rclcpp::shutdown();

    return 0;
}