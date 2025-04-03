#include "pwm_control.h"

// 　線形補間関数
//  x: 入力値, in_min: 入力値の最小値, in_max: 入力値の最大値, out_min: 出力値の最小値, out_max: 出力値の最大値
float map(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// PWM制御関数
// channel: PWMチャネル, value: 入力値
// valueが6500以上ならPWM_MINからPWM_MAXの範囲で線形補間、6500未満ならPWM_MAXからPWM_MINの範囲で線形補間
void controlFan(int channel, int value)
{
    int pulse_width;
    float duty_cycle;

    if (value >= -6500 && value <= 6500)
    {
        pulse_width = PWM_MIN;
    }
    else if (value > 6500)
    {
        pulse_width = (int)map(value, 6500, 32767, PWM_MIN, PWM_MAX);
    }
    else
    {
        pulse_width = (int)map(value, -32768, -6500, PWM_MAX, PWM_MIN);
    }

    duty_cycle = pulse_width / 20000.0f;
    set_pwm_channel_duty_cycle(channel, duty_cycle);
    printf("PWM Ch%d: %dus (duty: %f)\n", channel, pulse_width, duty_cycle);
}
