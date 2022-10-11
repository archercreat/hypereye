#pragma once

#include "heye/arch/asm.hpp"

#include "vmcall.hpp"
#include "vmcs.hpp"

#include <intrin.h>
#include <cstdint>

namespace vmx
{
enum class invept_t : uint64_t
{
    single_context = 1,
    all_contexts   = 2
};

enum class invvpid_t : uint64_t
{
  individual_address                = 0,
  single_context                    = 1,
  all_contexts                      = 2,
  single_context_retaining_globals  = 3,
};

struct invept_desc_t
{
    void* eptp;
    void* reserved;
};
static_assert(sizeof(invept_desc_t) == 16);

struct invvpid_desc_t
{
    uint64_t vpid : 16;
    uint64_t reserved : 48;
    uint64_t linear_address;
};
static_assert(sizeof(invvpid_desc_t) == 16);

inline uint8_t on(uint64_t pa)
{
    return __vmx_on(reinterpret_cast<uint64_t*>(&pa));
}

inline bool vmcall(vmcall_reason reason, void* a1 = nullptr, void* a2 = nullptr, void* a3 = nullptr)
{
    return asm_vmcall(reinterpret_cast<void*>(reason), a1, a2, a3);
}

inline void off()
{
    __vmx_off();
}

[[nodiscard]]
inline uint8_t clear(uint64_t pa)
{
    return __vmx_vmclear(reinterpret_cast<uint64_t*>(&pa));
}

[[nodiscard]]
inline uint8_t vmptrld(uint64_t pa)
{
    return __vmx_vmptrld(reinterpret_cast<uint64_t*>(&pa));
}

inline uint64_t read(vmcs field)
{
    uint64_t value{};
    __vmx_vmread(static_cast<uint64_t>(field), &value);
    return value;
}

[[nodiscard]]
inline uint8_t write(vmcs field, uint64_t value)
{
    return __vmx_vmwrite(static_cast<uint64_t>(field), value);
}

[[nodiscard]]
inline uint8_t launch()
{
    return asm_vmlaunch();
}

/// @brief Invalidate ept.
///
inline uint64_t invept(invept_t type, void* ept = nullptr)
{
    invept_desc_t desc{ .eptp = ept };
    return asm_invept(static_cast<uint64_t>(type), &desc);
}

/// @brief Invalidate all contexts.
///
inline uint64_t invept_all_contexts()
{
    return invept(invept_t::all_contexts);
}

inline uint64_t invvpid(invvpid_t type, invvpid_desc_t* desc = nullptr)
{
    if (desc == nullptr)
    {
        static invvpid_desc_t zero_desc{};
        desc = &zero_desc;
    }
    return asm_invvpid(static_cast<uint64_t>(type), desc);
}

inline uint64_t invvpid_individual_address(uint64_t address)
{
    invvpid_desc_t desc{ 0, 0, address };
    return invvpid(invvpid_t::individual_address, &desc);
}

inline uint64_t invvpid_all_contexts()
{
    return invvpid(invvpid_t::all_contexts);
}

// template<typename T>
// auto adjust(T value)
// {
//     if constexpr (std::is_same_v<T, msr::vmx_entry_controls>)
//     {

//     }
// }
};
