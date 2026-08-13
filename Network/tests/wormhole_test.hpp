#pragma once
#include <systemc.h>
#include "../src/Router.hpp"

// We will not run this test automatically in sc_main because SystemC does not support multiple sc_start runs gracefully 
// without complex reset logic. To run this, replace the objects in sc_main with this test.
