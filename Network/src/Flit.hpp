#pragma once

enum class flitType {
    UNICAST,
    MULTICAST,
    REDUCTION
};

struct Flit {
    int id;
    int src_x;
    int src_y;
    int dst_x;
    int dst_y;
    flitType type;
    int payload;
};
