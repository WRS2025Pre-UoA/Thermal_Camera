#include <chrono>
#include <memory>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "std_msgs/msg/u_int16_multi_array.hpp"
#include <cv_bridge/cv_bridge.h>
#include "BosonUSB.cpp" 

using std::placeholders::_1;
using namespace std;

class ImagePublisher : public rclcpp::Node
{
public:
    ImagePublisher(const std::string &topic_name) : Node("image_publisher")
    {
        publisher_ = this->create_publisher<sensor_msgs::msg::Image>("topic_name", 5);
        value_publisher_ = this->create_publisher<std_msgs::msg::UInt16MultiArray>("topic_value", 5);
        timer_ = this->create_wall_timer(100ms, std::bind(&ImagePublisher::publish_image, this));
        char* new_argv[3];
        new_argv[0] = (char*)"BosonUSB"; // プログラム名を設定
        new_argv[1] = (char*)"sB";       // sBを設定
        new_argv[2] = (char*)"2";        // 引数2を設定

        // funcに新しい引数を渡す
        k =func(3, new_argv); // argcは3に設定
        
        
    }

private:
    void publish_image()
    {
        image_ = show(k, thermal16, thermal16_linear, height, width,  zoom_enable, record_enable,  thermal_sensor_name, frame);
        if (image_.empty())
            return;

        // OpenCVからROSのメッセージ形式に変換
        // std_msgs::msg::UInt16MultiArray value_msg;
        // vector<uint16_t> values;

        // for (int i=0; i<height; i++) {
        //     for (int j=0; j<width; j++) {
        //         uint16_t value1 =  image_.at<uchar>(i,j*2+1) & 0XFF ;  // High Byte
        //         uint16_t value2 =  image_.at<uchar>(i,j*2) & 0xFF  ;    // Low Byte
        //         uint16_t value3 = ( value1 << 8) + value2;
        //         values.push_back(value3);
        //     }
        //     // cout<<endl;
        // }
        // value_msg.data=values;
        // value_publisher_->publish(value_msg);
        // cout<<image_<<endl;

        auto msg = cv_bridge::CvImage(std_msgs::msg::Header(), "mono16", image_).toImageMsg();
        publisher_->publish(*msg);
        RCLCPP_INFO(this->get_logger(), "Publishing image...");
    }

    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
    rclcpp::Publisher<std_msgs::msg::UInt16MultiArray>::SharedPtr value_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    cv::Mat image_;
    v4l2_buffer k;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    
    // トピック名の指定
    std::string topic_name = "image_topic";
    if (argc > 1) {
        topic_name = argv[1]; // コマンドライン引数でトピック名を変更可能
    }

    rclcpp::spin(std::make_shared<ImagePublisher>(topic_name));
    rclcpp::shutdown();
    return 0;
}
