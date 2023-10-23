Enum = {
    wall = 1,
    box = 2,
    player = 3,
    goal = 4,
    ground = 5,
    empty = 6,
}

State = {
    title = 1,
    loading = 2,
    play = 3,
    menu = 4,
    clear = 5,
}

Direction = {
    Up = 0x01,
    Down = 0x02,
    Left = 0x04,
    Right = 0x08,
    RightUp = 0x08 & 0x01,
    RightDown = 0x08 & 0x02,
    LeftUp = 0x04 & 0x01,
    LeftDown = 0x04 & 0x02,
}
