#include <iostream>
#include <unistd.h>
#include <iomanip>
#include "JoyStick.h"

struct ButtonState JSButton;

int main() {
    JoyStick BeiTongTest("/dev/input/js0");

    while(1)
    {
        JSButton=BeiTongTest.getButtonState();

        std::cout<<"\r"<<"A="<<JSButton.A<<"; B="<<JSButton.B<<"; X="<<JSButton.X<<"; Y="<<JSButton.Y
        <<"; left="<<JSButton.left<<"; right="<<JSButton.right<<"; up="<<JSButton.up<<"; down="<<JSButton.down;
        std::cout<< std::fixed << std::setprecision(2);
        std::cout<<std::showpos<<std::setw(5)<<"; leftAxisUp="<<JSButton.leftAxis[0]<<"; leftAxisLeft="
        <<JSButton.leftAxis[1]<<"; rightAxisUp="<<JSButton.rightAxis[0]
        <<"; rightAxisLeft="<<JSButton.rightAxis[1]<<"; LB="<<JSButton.LB<<"; RB="<<JSButton.RB<<std::flush;

        usleep(10000);
    }


    return 0;
}
