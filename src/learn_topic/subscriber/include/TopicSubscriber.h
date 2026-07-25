/*
    话题订阅：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
        3-1.创建订阅方；
        3-2.处理订阅到的消息。
    4.调用spin函数，并传入节点对象指针；
    5.释放资源。
*/

#ifndef _TOPIC_SUBSCRIBER_H_
#define _TOPIC_SUBSCRIBER_H_

#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/msg/student.hpp"
#include <functional>

namespace learn_topic
{
    using MsgCallback = std::function<void(const base_interfaces_demo::msg::Student msg)>;

    class TopicSubscriber : public rclcpp::Node
    {
    public:
        TopicSubscriber();

        ~TopicSubscriber();

    private:
        void msgcallback(const base_interfaces_demo::msg::Student msg);

    private:
        MsgCallback                                                         msgcallback_;
        rclcpp::Subscription<base_interfaces_demo::msg::Student>::SharedPtr subscriber ;
    };
}

#endif  // _TOPIC_SUBSCRIBER_H_
