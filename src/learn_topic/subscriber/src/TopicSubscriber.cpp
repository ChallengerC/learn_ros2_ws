#include "learn_topic_subscriber/TopicSubscriber.h"

namespace learn_topic
{
    TopicSubscriber::TopicSubscriber()
    :Node("topic_subscriber")
    {
        //
        msgcallback_ = std::bind(&TopicSubscriber::msgcallback,
                                 this,
                                 std::placeholders::_1);

        //
        subscriber = this->create_subscription<std_msgs::msg::String>("/learn_topic", 10, msgcallback_);
    }

    //
    TopicSubscriber::~TopicSubscriber()
    {
        RCLCPP_INFO(this->get_logger(), "TopicSubscriber 释放资源");
    }

    // 处理订阅到的消息
    void TopicSubscriber::msgcallback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "接收到的消息: '%s'", msg->data.c_str());
    }
}
