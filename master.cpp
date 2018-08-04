/*
 *@brief: Robomaster机器人视觉识别程序
*/

#include <thread> 
#include <unistd.h>
#include "opencv2/opencv.hpp"
#include <iostream>
#include "LinuxSerial.hpp"
#include <chrono>
CLinuxSerial serial(0);
void listen()
{
    while(1)
    {
        uchar data[] = {0xDA,0x00,0x00,0x00,0x00,0xDB};
        serial.ReadData(data,6);
        short Yaw = (data[1]<<8) + data[2];
        short Pitch = (data[3]<<8) + data[4];
        std::cout << "陀螺仪\tYaw: "<< Yaw/100.0 << "\tPitch: "<< Pitch/100.0 << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void publish()
{
    while(1)
    {
        unsigned char send_bytes[] = { 0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE};
        double send_data[]={1, 72.4, 20.5, 3.1};
        short* data_ptr = (short *)(send_bytes + 1); // 16位指针指向第一个数据

        data_ptr[0] = (short)send_data[0];  // bool
        data_ptr[1] = (short)send_data[1];  // x
        data_ptr[2] = (short)send_data[2];  // y
        data_ptr[3] = (short)send_data[3];  // z

        serial.WriteData(send_bytes,10);
        std::cout << "send_data..."<< std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

int main(int argc, char * argv[]){

    std::thread task1(&publish);
    std::thread task2(&listen);

    task1.join();
    task2.join();

	return EXIT_SUCCESS;
}

