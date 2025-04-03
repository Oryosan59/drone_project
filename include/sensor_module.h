#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include <pthread.h>
#include <stdbool.h>

// センサーデータをUDPで送信するための関数
bool start_sensor_data_thread();

// スレッドを停止するための関数
void stop_sensor_data_thread();

#endif // SENSOR_MODULE_H
