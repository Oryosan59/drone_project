#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <iostream>
#include <sstream>
#include <stdint.h>

// 受信データの構造体
struct GamepadData
{
    int leftThumbX;
    int leftThumbY;
    int rightThumbX;
    int rightThumbY;
    int LT;
    int RT;
    // ボタンの状態を表すビットフィールド
    uint16_t buttons; // 16ビットの整数型でボタンの状態を表す
};

// ボタンの定義 (SharpDX.XInputの順番に合わせる)
enum GamepadButton
{
    None = 0x0000,
    DPadUp = 0x0001,
    DPadDown = 0x0002,
    DPadLeft = 0x0004,
    DPadRight = 0x0008,
    Start = 0x0016,
    Back = 0x0032,
    LeftShoulder = 0x0256,
    RightShoulder = 0x0512,
    A = 0x8192,
    B = 0x4096,
    X = 0x16384,
    Y = 0x32768
};

// 受信データをパース
GamepadData parseGamepadData(const std::string &data);

#endif
