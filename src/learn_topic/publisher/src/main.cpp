#include "TopicPublisher.h"
#include <memory>

int main(int argc, char * argv[])
{
    //
    rclcpp::init(argc, argv);

    //
    auto topic_publisher = std::make_shared<learn_topic::TopicPublisher>();

    //
    rclcpp::spin(topic_publisher);

    //
    rclcpp::shutdown();

    //
    return 0;
}
