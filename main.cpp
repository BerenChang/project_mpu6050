/**********************************************************************
* Filename    : main.cpp
* Description : Read and display data of MPU6050
* Author      : Beren
* modification: 2024/8/11
**********************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <cmath>
#include <deque>
#include <ctime>
#include <chrono>
#include <matplot/matplot.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;      //creat MPU6050 class object

int16_t ax, ay, az;     //store acceleration data
int16_t gx, gy, gz;     //store gyroscope data
int16_t temp;           //store temperature data

void setup() {
    // initialize device
    printf("Initializing I2C devices...\n");
    accelgyro.initialize();     //initialize MPU6050

    // verify connection
    printf("Testing device connections...\n");
    printf(accelgyro.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");
}

/*
void loop() {
    // read accel/gyro values of MPU6050
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    temp = accelgyro.getTemperature();
    // display accel/gyro x/y/z values
    // printf("a/g: %6hd %6hd %6hd   %6hd %6hd %6hd\n",ax,ay,az,gx,gy,gz);
    printf("a/g: %.2f g %.2f g %.2f g   %.2f d/s %.2f d/s %.2f d/s \n",(float)ax/16384,(float)ay/16384,(float)az/16384,
        (float)gx/131,(float)gy/131,(float)gz/131);
    printf("Temperature: %.2f Celcius. \n", (float)(temp/340 + 36.53));
}
*/

void loop_plot() {
    using namespace matplot;  // namespace
    using namespace std::chrono;  // namespace
    
    std::deque<float> ax_list;  // store list of acceleration
    std::deque<float> i_list;  // store index
    std::deque<float> time_list;  // store timestamp
    steady_clock::time_point start_time;  // store start time
    duration<double> time_stamp;
    float float_ax;  // store acceleration
    
    auto fig = gcf();  // for graphing
    fig->quiet_mode(true);  // graph under quiet mode
    
    start_time = steady_clock::now();
    for (size_t i = 0; i<100; ++i) {
        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        float_ax = (float)ax/16384;
        // printf("x-axis acceleration: %.2f g \n", float_ax);
        ax_list.push_back(float_ax);
        time_stamp = duration_cast<duration<double>>(steady_clock::now() - start_time);
        time_list.push_back((float)time_stamp.count());
        plot(time_list, ax_list);
        fig->draw();
        sleep(0.5);
    }
    
    for (size_t i = 100; i<1000; ++i) {
        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        float_ax = (float)ax/16384;
        // printf("x-axis acceleration: %.2f g \n", float_ax);
        ax_list.pop_front();
        ax_list.push_back(float_ax);
        time_stamp = duration_cast<duration<double>>(steady_clock::now() - start_time);
        // printf("%.2f \n", (float)time_stamp.count());
        time_list.pop_front();
        time_list.push_back((float)time_stamp.count());
        plot(time_list, ax_list);
        fig->draw();
        sleep(0.5);
    }
}

int main()
{
    setup();
    loop_plot();
    return 0;
}

