#include "control.hpp"

#include "hv/hypervisor.hpp"
#include "shared/trace.hpp"

bool run()
{
    return hypervisor::get().enter();
}
