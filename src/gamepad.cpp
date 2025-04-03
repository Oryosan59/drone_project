#include "gamepad.h"

// 受信データをパースする関数
GamepadData parseGamepadData(const std::string &data)
{
    GamepadData gamepad;
    std::stringstream ss(data);
    std::string token;
    int values[7] = {0}; // 配列のサイズを7に拡張
    int index = 0;

    while (std::getline(ss, token, ',') && index < 7)
    {
        try
        {
            values[index++] = std::stoi(token);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "stoiエラー: 無効なデータ (" << token << ")" << std::endl;
            return gamepad;
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << "stoiエラー: 数値が範囲外 (" << token << ")" << std::endl;
            return gamepad;
        }
    }

    gamepad.leftThumbX = values[0];
    gamepad.leftThumbY = values[1];
    gamepad.rightThumbX = values[2];
    gamepad.rightThumbY = values[3];
    gamepad.LT = values[4];
    gamepad.RT = values[5];

    // ボタンデータを uint16_t にキャスト
    gamepad.buttons = static_cast<uint16_t>(values[6]);

    return gamepad;
}
