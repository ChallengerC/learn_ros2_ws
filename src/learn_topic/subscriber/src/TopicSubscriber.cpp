#include "TopicSubscriber.h"

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
        subscriber = this->create_subscription<base_interfaces_demo::msg::Student>("/topic_stu", 10, msgcallback_);
    }

    //
    TopicSubscriber::~TopicSubscriber()
    {
        RCLCPP_INFO(this->get_logger(), "TopicSubscriber 释放资源");
    }

    // 处理订阅到的消息
    void TopicSubscriber::msgcallback(const base_interfaces_demo::msg::Student msg)
    {
        // RCLCPP_INFO(this->get_logger(), "接收到的消息: '%s'", msg->data.c_str());
        RCLCPP_INFO(this->get_logger(), "学生信息接收：name=%s, age =%d, height= %.2f",
                    msg.name.c_str(), msg.age, msg.height);
    }
}
