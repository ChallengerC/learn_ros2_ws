#include "learn_topic/TopicPublisher.h"
#include <memory.h>

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
    return 0;
}