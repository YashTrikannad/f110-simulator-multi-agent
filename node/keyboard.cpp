#include <ros/ros.h>

#include <std_msgs/String.h>

#include <termios.h>

#include <stdio.h>
#include <signal.h>

// for printing
#include <iostream>

#include "f110_simulator/config.h"

static volatile sig_atomic_t keep_running = 1;

void sigHandler(int not_used)
{
    keep_running = 0;
}

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "keyboard");
    // Initialize Node Handle
    ros::NodeHandle n = ros::NodeHandle("~");

    // Initialze publisher
    std::string keyboard_topic;
    n.getParam("keyboard_topic", keyboard_topic);

    std::array<ros::Publisher, config::n_agents> key_pubs{};
    for(int i=0; i < (int)config::n_agents; i++)
    {
        const auto new_keyboard_topic = keyboard_topic + "_" + std::to_string(i+1);
        std::cout << "nkt : " << new_keyboard_topic << std::endl;
        key_pubs[i] = n.advertise<std_msgs::String>(new_keyboard_topic, 10);
    }

    static struct termios oldt, newt;
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    tcsetattr( STDIN_FILENO, 0, &newt);

    struct sigaction act;
    act.sa_handler = sigHandler;
    sigaction(SIGINT, &act, NULL);
    

    std_msgs::String msg;
    int c;
    int active_car_idx = 1;

    while ((ros::ok()) && (keep_running))
    {
        // get the character pressed
        c = getchar();
        if(c < 10 && c >= 0)
        {
            active_car_idx = c;
        }
        // Publish the character 
        msg.data = c;
        std::cout << "HELLO ACTIVE CAR " << active_car_idx << std::endl;
        key_pubs[active_car_idx-1].publish(msg);
    }

    tcsetattr( STDIN_FILENO, 0, &oldt);
    
    return 0;
}