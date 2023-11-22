local Obj = {
    wall = 1,
    box = 2,
    player = 3,
    goal = 4,
    ground = 5,
    empty = 6,
}


local UIType = {
    Scence = 1,
    View = 2
}


local State = {
    title = 1,
    loading = 2,
    play = 3,
    menu = 4,
    clear = 5,
}

local Direction = {
    Null = 0x00,
    Up = 0x01,
    Down = 0x02,
    Left = 0x04,
    Right = 0x08,
    RightUp = 0x08 | 0x01,
    RightDown = 0x08 | 0x02,
    LeftUp = 0x04 | 0x01,
    LeftDown = 0x04 | 0x02,
}
local Keyboard = {
    A = 97,
    B = 98,
    C = 99,
    D = 100,
    E = 101,
    S = 115,
    W = 119,
    P = 112,
    Q = 113,
    R = 114,
    Space = 32,
    Shift = 131,
    One = 49,
    Two = 50,
    Three = 51,
}

local Input = {
    [Keyboard.A] = 0x1000,
    [Keyboard.W] = 0x0001,
    [Keyboard.D] = 0x0010,
    [Keyboard.S] = 0x0100,
}

local InputToDirection = {
    --xASDW
    [0x0000] = Direction.Null,
    [0x0001] = Direction.Up,
    [0x0010] = Direction.Right,
    [0x0011] = Direction.RightUp,
    [0x0100] = Direction.Down,
    [0x0101] = Direction.Null,
    [0x0110] = Direction.RightDown,
    [0x0111] = Direction.Right,
    [0x1000] = Direction.Left,
    [0x1001] = Direction.LeftUp,
    [0x1010] = Direction.Null,
    [0x1011] = Direction.Up,
    [0x1100] = Direction.LeftDown,
    [0x1101] = Direction.Left,
    [0x1110] = Direction.Down,
    [0x1111] = Direction.Null,
}

local Mode = {
    MODE_1P = 1,
    MODE_2P = 2,
    MODE_NONE = 0
}

local BlendMode = {
    BLEND_LINEAR = 0,
    BLEND_ADDITIVE = 1,
    BLEND_OPAQUE = 2,
}

Enum = {
    Obj = Obj,
    State = State,
    Direction = Direction,
    Keyboard = Keyboard,
    Input = Input,
    InputToDirection = InputToDirection,
    Mode = Mode,
    UIType = UIType,
    BlendMode = BlendMode,
}
