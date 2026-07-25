#include "TopicPublisher.h"
#include <chrono>

namespace learn_topic
{
    TopicPublisher::TopicPublisher()
    :rclcpp::Node("topic_publisher")
    ,count_(0)
    {
        //
        publisher_ = this->create_publisher<base_interfaces_demo::msg::Student>("/topic_stu", 10);

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
        auto stumsg = base_interfaces_demo::msg::Student();
        {
            stumsg.name   = "张三";
            stumsg.age    = count_++;
            stumsg.height = 1.65;
        }

        RCLCPP_INFO(this->get_logger(), "学生信息发布：name=%s, age =%d, height= %.2f",
                    stumsg.name.c_str(), stumsg.age, stumsg.height);

        publisher_->publish(stumsg);
    }
}
