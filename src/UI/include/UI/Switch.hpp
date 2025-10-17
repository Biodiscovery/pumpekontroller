#pragma once

namespace pump_control::ui {

class Switch {
    private:
    int pin;
    
    public:
    Switch(int pin);
    void init();
    bool read();
};

}   //namepsace ui 
