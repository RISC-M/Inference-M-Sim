#pragma once

enum class flitType {
    UNICAST,
    MULTICAST,
    REDUCTION
};

enum class flitState {
    HEAD,
    BODY,
    TAIL,
    SINGLE
};

struct Flit {
    int id;
    int src_x;
    int src_y;
    int dst_x;
    int dst_y;
    flitType type;
    flitState state;
    int payload;
};
