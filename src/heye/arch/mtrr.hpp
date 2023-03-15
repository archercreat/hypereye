#pragma once
#include "memory.hpp"
#include "heye/shared/std/utility.hpp"

#include <cstdint>

namespace heye
{
struct mtrr_range
{
    uint64_t      base;
    uint64_t      size;
    memory_type_t type;
};

struct mtrr_descriptor
{
    mtrr_descriptor();

    /// Helper functions for `for` iterator.
    ///
    auto   begin() const { return &mtrr[0];                         }
    auto   end()   const { return &mtrr[size()];                    }
    size_t size()  const { return fixed_count + variable_available; }

    /// Get memory type of the physical address, return default if not found.
    ///
    memory_type_t get_type_or(uint64_t pa, memory_type_t def) const;

private:
    template<typename T> requires (std::has_id_v<T>)
    void build_mtrr(auto& index)
    {
        uint64_t offset{};

        for (auto type : read<T>().types)
        {
            mtrr[index].type = static_cast<memory_type_t>(type);
            mtrr[index].base = T::base + offset;
            mtrr[index].size = T::size + T::base + offset;
            index  += 1;
            offset += T::size;
        }
    };

    /// Architecture defined number of fixed mtrr registers.
    /// 1 register for 64k, 2 registers for 16k and 8 registers for 4k.
    /// Each register has 8 ranges as per "Fixed Range MTRRs" states.
    ///
    static constexpr auto fixed_count    = (1 + 2 + 8) * 8;
    static constexpr auto variable_count = 255;

    union
    {
        struct
        {
            mtrr_range fixed[fixed_count];
            mtrr_range variable[variable_count];
        };

        mtrr_range mtrr[fixed_count + variable_count];
    };

    size_t variable_available;
};
};
