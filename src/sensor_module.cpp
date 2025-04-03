#include "sensor_module.h"
#include "udp_client.h"
#include "bindings.h"
#include <stdio.h>
#include <unistd.h>
#include <string>

// Global variables for sensor thread
static pthread_t sensor_thread;
static bool sensor_thread_running = false;
static udp_client::UDPClient *sensor_udp_client = nullptr; // UDPクライアントのポインタを追加

// Sensor data sending thread function
static void *sensor_data_thread_func(void *arg)
{
    while (sensor_thread_running)
    {
        // Get sensor data
        float temp = read_temp();
        float pressure = read_pressure();
        bool leak = read_leak();
        float adc[4];
        read_adc_all(adc, 4);
        float adc1 = read_adc(AdcChannel::Ch1);
        AxisData mag = read_mag();
        AxisData accel = read_accel();
        AxisData gyro = read_gyro();

        // Format sensor data into a string
        char data_buffer[256];
        snprintf(data_buffer, sizeof(data_buffer),
                 "Temp:%.2f,Pressure:%.2f,Leak:%s,ADC1:%.2f,ADC2:%.2f,ADC3:%.2f,ADC4:%.2f,ADC1_data:%.2f,MagX:%.2f,MagY:%.2f,MagZ:%.2f,AccelX:%.2f,AccelY:%.2f,AccelZ:%.2f,GyroX:%.2f,GyroY:%.2f,GyroZ:%.2f\n",
                 temp, pressure, leak ? "true" : "false", adc[0], adc[1], adc[2], adc[3], adc1, mag.x, mag.y, mag.z, accel.x, accel.y, accel.z, gyro.x, gyro.y, gyro.z);

        // Send sensor data via UDP
        if (sensor_udp_client)
        {
            sensor_udp_client->sendData(data_buffer);
        }

        usleep(100000); // 100ms sleep (10Hz update rate)
    }
    return NULL;
}

// Function to start the sensor data sending thread
bool start_sensor_data_thread(const char *server_ip, int server_port, int client_port)
{
    if (sensor_thread_running)
    {
        return true;
    }

    // UDPクライアントの初期化
    sensor_udp_client = new udp_client::UDPClient();
    if (!sensor_udp_client->initialize(server_ip, server_port, client_port))
    {
        delete sensor_udp_client;
        sensor_udp_client = nullptr;
        return false;
    }

    sensor_thread_running = true;
    if (pthread_create(&sensor_thread, NULL, sensor_data_thread_func, NULL) != 0)
    {
        perror("Failed to create sensor thread");
        sensor_thread_running = false;
        delete sensor_udp_client;
        sensor_udp_client = nullptr;
        return false;
    }
    return true;
}

// Function to stop the sensor data sending thread
void stop_sensor_data_thread()
{
    if (!sensor_thread_running)
    {
        return;
    }
    sensor_thread_running = false;
    pthread_join(sensor_thread, NULL);

    // UDPクライアントのクローズと削除
    if (sensor_udp_client)
    {
        sensor_udp_client->close();
        delete sensor_udp_client;
        sensor_udp_client = nullptr;
    }
}
