#include "gamepad.h"
#include "pwm_control.h"
#include "udp_server.h"
#include "udp_client.h"
#include "sensor_module.h" // sensor_module.h をインクルード
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    printf("Navigatorモジュールを初期化\n");
    init();

    // センサーデータ送信スレッドの初期化
    if (!start_sensor_data_thread("192.168.1.10", 12345, 12346)) // 送信先IPアドレスとポート番号を指定
    {
        std::cerr << "Failed to start sensor data thread." << std::endl;
        return -1;
    }

    // UDPクライアントの初期化
    udp_client::UDPClient client;
    if (!client.initialize("192.168.1.10", 12345, 12346))
    {
        std::cerr << "Failed to initialize UDP client." << std::endl;
        stop_sensor_data_thread(); // センサースレッドを停止
        return -1;
    }

    // UDPサーバーの初期化と起動
    udp_server::UDPServer server;
    if (!server.initialize(12345))
    {
        stop_sensor_data_thread(); // センサースレッドを停止
        client.close();
        return -1;
    }

    // 受信データ処理のコールバック関数
    auto dataReceivedCallback = [&](const std::string &receivedData)
    {
        std::cout << "受信データ: " << receivedData << std::endl;
        GamepadData gamepad = parseGamepadData(receivedData);

        // 左スティックで旋回
        if (gamepad.leftThumbX > 6500)
        {
            controlFan(0, gamepad.leftThumbX);
            controlFan(3, gamepad.leftThumbX);
        }
        else if (gamepad.leftThumbX < -6500)
        {
            controlFan(1, gamepad.leftThumbX);
            controlFan(2, gamepad.leftThumbX);
        }

        // 右スティックで移動
        if (gamepad.rightThumbX > 6500)
        {
            controlFan(0, gamepad.rightThumbX);
            controlFan(2, gamepad.rightThumbX);
        }
        else if (gamepad.rightThumbX < -6500)
        {
            controlFan(1, gamepad.rightThumbX);
            controlFan(3, gamepad.rightThumbX);
        }

        // 前進
        controlFan(4, (gamepad.rightThumbY > 6500) ? gamepad.rightThumbY : 0);
    };

    if (!server.startListening(dataReceivedCallback))
    {
        stop_sensor_data_thread(); // センサースレッドを停止
        client.close();
        server.stop();
        return -1;
    }

    printf("PWMを有効化\n");
    set_pwm_enable(true);
    set_pwm_freq_hz(50.0);

    // サーバーを動作させ続ける
    while (true)
    {
        // 終了条件の確認 (例: 特定のボタンが押されたら終了)
        // ここに終了条件の判定処理を追加
        if (gamepad.buttons & GamepadButton::Back && gamepad.buttons & GamepadButton::Start)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // サーバーの停止
    server.stop();
    set_pwm_enable(false);
    client.close();
    stop_sensor_data_thread(); // センサースレッドを停止

    return 0;
}
