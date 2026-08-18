/* Translation Schema:
    1. Outgoing (AXI -> NoC/Flit)
    input args:
    - Address
    - AWUSER Sideband
        - addr mask
        - collective opcode
    - Payload

    output args:
    - dst
    - x mask
    - y mask
    - opcode
    - Payload

    2. Incoming (NoC/Flit -> AXI)
    input args:
    - src
    - x mask
    - y mask
    - Payload

    output args:
    - Address
    - AWUSER Sideband
        - addr mask
        - collective opcode
    - Payload
 */

typedef struct {
    int addr_mask;
    int collective_opcode;
} AWUSER


class NI : public SimObject {
    private:
};