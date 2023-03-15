#pragma once

namespace heye
{
struct vcpu_t;

using setup_cb_t    = void(*)(vcpu_t*);
using teardown_cb_t = void(*)(vcpu_t*);
using vmexit_cb_t   = bool(*)(vcpu_t*);
};
