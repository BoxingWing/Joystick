//
// Created by boxingwang on 22-11-22.
//

#ifndef JOYSTICK_JOYSTICK_H
#define JOYSTICK_JOYSTICK_H

#include "stdint.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

struct ButtonState{
    int16_t X; // 1 for pressed, 0 for released
    int16_t Y;
    int16_t A;
    int16_t B;
    int16_t up;
    int16_t down;
    int16_t left;
    int16_t right;
    int16_t LB;
    int16_t RB;
    double leftAxis[2]; // first for up and down, second for left and right. Up and right are positive, values are normalized.
    double rightAxis[2];
};

class JoyStick {
private:
    int joy_fd; // handle of the joystick
    unsigned char NumOfAxix; // axis number
    unsigned char NumOfButton; // button number
    unsigned char ButtonState;
    unsigned char AxisState;
    char joystick_name[1024];
    struct js_event js; // event record of the joystick
    short int value[64];
    unsigned char axisbuttonNumber[64];
    int LoopCount;
    struct ButtonState ButtonStateOld;
    struct ButtonState ButtonStateNow;
    void eventRead();
    int returnJSInfo(int16_t * valueReturn, int16_t * numberReturn);
public:
    JoyStick(const char * devFileName);
    ~JoyStick();
    struct ButtonState getButtonState();
    unsigned char returnButtonState();
    unsigned char returnAxisState();
};


#endif //JOYSTICK_JOYSTICK_H
