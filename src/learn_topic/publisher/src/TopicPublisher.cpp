#include "learn_topic_publisher/TopicPublisher.h"
#include <chrono>

namespace learn_topic
{
    TopicPublisher::TopicPublisher()
    :rclcpp::Node("topic_publisher")
    ,count_(0)
    {
        //
        publisher_ = this->create_publisher<std_msgs::msg::String>("/learn_topic", 10);

        //
        timecallback_ = std::bind(&TopicPublisher::timer_callback, this);

        timer_ = this->create_wall_timer(std::chrono::milliseconds(500), timecallback_);
    }

    TopicPublisher::~TopicPublisher()
    {
        RCLCPP_INFO(this->get_logger(), "TopicPublisher 释放资源");
    }

    void TopicPublisher::timer_callback()
    {
        //
        auto msg = std_msgs::msg::String();
        msg.data = "Hello, World! " + std::to_string(count_++);

        //
        RCLCPP_INFO(this->get_logger(), "发布的消息: '%s'", msg.data.c_str());
        publisher_->publish(msg);
    }
}
