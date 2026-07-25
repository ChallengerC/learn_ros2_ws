#include "LearnServer.h"
#include <memory>

int main(int argc, char* argv[])
{
    //
    rclcpp::init(argc, argv);

    //
    auto server = std::make_shared<learn_service::LearnServer>();

    rclcpp::spin(server);

    //
    rclcpp::shutdown();

    return 0;
}