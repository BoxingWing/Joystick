//
// Created by boxingwang on 22-11-22.
//

#include "JoyStick.h"

JoyStick::JoyStick(const char * devFileName) {
    //const char *devFileName=devName.c_str();
    joy_fd=open(devFileName,O_NONBLOCK);
    // Get the name of the joystick
    ioctl(joy_fd, JSIOCGNAME(sizeof joystick_name), joystick_name);

    // Get the number of axes on the joystick
    ioctl(joy_fd, JSIOCGAXES, &NumOfAxix);

    // Get the number of buttons on the joystick
    ioctl(joy_fd, JSIOCGBUTTONS, &NumOfButton);

    eventRead();
    ButtonStateOld.A=0;
    ButtonStateOld.B=0;
    ButtonStateOld.X=0;
    ButtonStateOld.Y=0;
    ButtonStateOld.down=0;
    ButtonStateOld.up=0;
    ButtonStateOld.left=0;
    ButtonStateOld.right=0;
    ButtonStateOld.leftAxis[0]=0;
    ButtonStateOld.leftAxis[1]=0;
    ButtonStateOld.rightAxis[0]=0;
    ButtonStateOld.rightAxis[1]=0;
    ButtonStateOld.LB=0;
    ButtonStateOld.RB=0;
    ButtonStateNow=ButtonStateOld;

    ButtonState=0;
    AxisState=0;
}

void JoyStick::eventRead() {
    for (int i=0;i<64;i++)
    {
        value[i]=99;
        axisbuttonNumber[i]=99;
    }
    LoopCount=0;
    while(read(joy_fd,&js,sizeof(js))>0)
    {

        switch (js.type & ~JS_EVENT_INIT)
        {
            case JS_EVENT_BUTTON:
            {if (js.value)
                    ButtonState |= (1<<js.number);
                else
                    ButtonState &= ~(1<<js.number);
                value[LoopCount]=js.value;
                axisbuttonNumber[LoopCount]=js.number;
                LoopCount++;
                break;}
            case JS_EVENT_AXIS:
            {if (js.value>0)
                    AxisState |= (1<<2*js.number);
                else if (js.value<0)
                    AxisState |= (1<<(2*js.number+1));
                else
                {AxisState &= ~(1<<(2*js.number+1));
                    AxisState &= ~(1<<2*js.number);}
                value[LoopCount]=js.value;
                axisbuttonNumber[LoopCount]=js.number;
                LoopCount++;
                break;}
        }
    };
}

int JoyStick::returnJSInfo(int16_t *valueReturn, int16_t *numberReturn) {
    eventRead();
    for (int i=0;i<LoopCount+1;i++)
    {
        valueReturn[i]=value[i];
        numberReturn[i]=axisbuttonNumber[i];
    }
    return LoopCount;
}
/*
 Button decoder for Beitong A_Xiu_Luo pro gamepad.
 the value of each button will be held to 1 from pressed to released
 for axis:
  up and right are positive
  output value is nomalized to range of 1
  */
struct ButtonState JoyStick::getButtonState() {
    ButtonStateNow=ButtonStateOld;
    int16_t value[64];
    int16_t number[64];
    int eventCount;
    eventCount=returnJSInfo(value,number);
    if (eventCount>0)
        for(int i=0;i<eventCount;i++)
        {
            switch (number[i]) {
                case 0:
                {
                    if (value[i]==1)
                        ButtonStateNow.A=1;
                    else if (value[i]==0)
                        ButtonStateNow.A=0;
                    ButtonStateNow.leftAxis[1]=value[i]/32767.0;
                    break;
                }
                case 1:
                {
                    if (value[i]==1)
                        ButtonStateNow.B=1;
                    else if (value[i]==0)
                        ButtonStateNow.B=0;
                    ButtonStateNow.leftAxis[0]=value[i]/-32767.0;
                    break;
                }
                case 2:
                {
                    ButtonStateNow.rightAxis[1]=value[i]/32767.0;
                    break;
                }
                case 3:
                {
                    if (value[i]==1)
                        ButtonStateNow.X=1;
                    else if (value[i]==0)
                        ButtonStateNow.X=0;
                    ButtonStateNow.rightAxis[0]=value[i]/-32767.0;
                    break;
                }
                case 4:
                {
                    if (value[i]==1)
                        ButtonStateNow.Y=1;
                    else if (value[i]==0)
                        ButtonStateNow.Y=0;
                    break;
                }
                case 7:
                {
                    if (value[i]==32767)
                        ButtonStateNow.down=1;
                    else if (value[i]==-32767)
                        ButtonStateNow.up=1;
                    else if (value[i]==0)
                        {ButtonStateNow.up=0;
                        ButtonStateNow.down=0;ButtonStateNow.RB=0;}
                    else if (value[i]==1)
                        ButtonStateNow.RB=1;
                    break;
                }
                case 6:
                {
                    if (value[i]==32767)
                        ButtonStateNow.right=1;
                    else if (value[i]==-32767)
                        ButtonStateNow.left=1;
                    else if (value[i]==0)
                    {ButtonStateNow.left=0;ButtonStateNow.right=0;ButtonStateNow.LB=0;}
                    else if (value[i]==1)
                    {ButtonStateNow.LB=1;}
                    break;
                }
            }
        }
    ButtonStateOld=ButtonStateNow;
    return ButtonStateNow;
}

unsigned char JoyStick::returnButtonState() {
    return ButtonState;
}

unsigned char JoyStick::returnAxisState() {
    return AxisState;
}

JoyStick::~JoyStick() {
    close(joy_fd);
}