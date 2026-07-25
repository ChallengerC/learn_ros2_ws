#include "learn_topic_subscriber/TopicSubscriber.h"
#include <memory>

int main(int argc, char * argv[])
{
    //
    rclcpp::init(argc, argv);

    //
    auto topic_subscriber = std::make_shared<learn_topic::TopicSubscriber>();

    //
    rclcpp::spin(topic_subscriber);

    //
    rclcpp::shutdown();

    //
    return 0;
}
