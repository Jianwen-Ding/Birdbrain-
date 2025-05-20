#ifndef INPUT_HPP
#define INPUT_HPP

#include "MathConsts.hpp"

// Inputs are identified by an ID system where in the input gathered is activates certain ID's.
// This allows for actual input to be abstract away from the results of said input (i.e. one person could bind kick to c while another can bind kick to d)
enum ButtonType : uint8 {
    KEY_Q = 1,
    KEY_W = 2,
    KEY_E = 3,
    KEY_R = 4,
    KEY_T = 5,
    KEY_Y = 6,
    KEY_U = 7,
    KEY_I = 8,
    KEY_O = 9,
    KEY_P = 10,
    KEY_A = 11,
    KEY_S = 12,
    KEY_D = 13,
    KEY_F = 14,
    KEY_G = 15,

};

// Represents the state of a button input throughout a single frame
struct ButtonState {
    // Whether by the end of the frame 
    bool m_endOn { false };
    // The amounts of times that it has been flipped. Does not include start.
    uint8 m_flipStates { 0 };
    
    // Times that the input state has been flipped on (does not include the start state as having flipped on)
    inline uint8 flipOnTimes() {
        if(m_endOn) {
            return 1 + (m_flipStates / 2);
        }
        else {
            return m_flipStates / 2;
        }
    }

    // Times that the input state has been flipped off (does not include the start state as having flipped off)
    inline uint8 flipOffTimes() {
        if(m_endOn) {
            return m_flipStates / 2;
        }
        else {
            return 1 + (m_flipStates / 2);
        }
    }

    // What the input state has started on
    inline bool startOn() {
        if(m_flipStates % 2 == 1) {
            return !m_endOn;
        }
        else {
            return m_endOn;
        }
    }

};

#endif