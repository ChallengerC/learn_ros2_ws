/*
    话题发布
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建发布方；
      3-2.创建定时器；
      3-3.组织消息并发布。
    4.调用spin函数，并传入节点对象指针；
    5.释放资源。
*/

#ifndef _TOPIC_PUBLISHER_H_
#define _TOPIC_PUBLISHER_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "base_interfaces_demo/msg/student.hpp"
#include <functional>

namespace learn_topic
{
    using TimerCallback = std::function<void()>;

    class TopicPublisher : public rclcpp::Node
    {
    public:
        TopicPublisher();

        ~TopicPublisher();

    private:
        void timer_callback();

    private:
        TimerCallback                                                    timecallback_;
        size_t                                                           count_;
        rclcpp::TimerBase::SharedPtr                                     timer_;
        rclcpp::Publisher<base_interfaces_demo::msg::Student>::SharedPtr publisher_;
    };

}
#endif  // _TOPIC_PUBLISHER_H_
