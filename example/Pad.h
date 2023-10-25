#ifndef INCLUDED_PAD_H
#define INCLUDED_PAD_H

class Pad {
  public:
    enum Button {
        A, // 0按钮'd’
        B, // 1按钮或“x”
        U, // up
        D, // down
        L, // left
        R, // right
        Q, // quit。 退出按钮
    };
    // 某个按钮
    static bool isOn(Button, int playerID = 0);
    static bool isTriggered(Button, int playerID = 0);
};

#endif
