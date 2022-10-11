#include "heye/arch/arch.hpp"
#include "heye/shared/trace.hpp"

mtrr_descriptor::mtrr_descriptor() : variable_available(0)
{
    uint64_t index{};
    // Fill fixed range mtrr first.
    //
    build_mtrr<msr::mtrr_fix_64k>  (index);
    build_mtrr<msr::mtrr_fix_16k_0>(index);
    build_mtrr<msr::mtrr_fix_16k_1>(index);
    build_mtrr<msr::mtrr_fix_4k_0> (index);
    build_mtrr<msr::mtrr_fix_4k_1> (index);
    build_mtrr<msr::mtrr_fix_4k_2> (index);
    build_mtrr<msr::mtrr_fix_4k_3> (index);
    build_mtrr<msr::mtrr_fix_4k_4> (index);
    build_mtrr<msr::mtrr_fix_4k_5> (index);
    build_mtrr<msr::mtrr_fix_4k_6> (index);
    build_mtrr<msr::mtrr_fix_4k_7> (index);
    // Fill variable range mtrr.
    //
    for (int i = 0; i < read<msr::mtrrcap>().vnct; i++)
    {
        const auto physbase = read<msr::mtrr_physbase>(msr::mtrr_physbase::id + i * 2);
        const auto physmask = read<msr::mtrr_physmask>(msr::mtrr_physmask::id + i * 2);

        if (physmask.valid)
        {
            unsigned long length{};
            _BitScanForward64(&length, physmask.pfn);

            mtrr[index + variable_available].type = static_cast<memory_type_t>(physbase.type);
            mtrr[index + variable_available].base = (physbase.pfn << PAGE_SHIFT);
            mtrr[index + variable_available].size = (physbase.pfn + (1ull << length)) << PAGE_SHIFT;
            variable_available++;
        }
    }
}

memory_type_t mtrr_descriptor::get_type_or(uint64_t pa, memory_type_t def) const
{
    for (const auto& range : *this)
    {
        if (pa < range.base + range.size)
        {
            if (pa + 2_mb - 1 >= range.base)
            {
                def = range.type;
                if (def == memory_type_t::uncachable)
                    break;
            }
        }
    }
    return def;
}
