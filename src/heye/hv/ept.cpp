#include "heye/hv/ept.hpp"
#include "heye/arch/arch.hpp"
#include "heye/shared/trace.hpp"

ept_t::ept_t()
{
    // Allocate page table.
    // There is a bug in msvc compiler that will not allow you to call `new` like this: new (std::align_val_t(0)) page_table_t.
    //
    page_table = static_cast<page_table_t*>(operator new(sizeof(page_table_t), std::align_val_t{ PAGE_SIZE }));

    const auto mtrr = mtrr_descriptor();
    const auto cap  = read<msr::vmx_ept_vpid_cap>();
    // Setup EPT pointer.
    //
    ept.access_flags     = cap.ept_access_dirty;
    ept.page_walk_length = page_walk_4;
    ept.memory_type      = cap.memory_type_wb ? memory_type_t::write_back : memory_type_t::uncachable;
    ept.pml4_address     = pfn(pa_from_va(page_table->pml4));
    // Setup PML4 entries.
    //
    page_table->pml4[0].read    = true;
    page_table->pml4[0].write   = true;
    page_table->pml4[0].execute = true;
    page_table->pml4[0].pfn     = pfn(pa_from_va(page_table->pdpt));
    // Setup PDPT entries.
    //
    for (int i = 0; i < std::countof(page_table->pdpt); i++)
    {
        page_table->pdpt[i].read    = true;
        page_table->pdpt[i].write   = true;
        page_table->pdpt[i].execute = true;
        page_table->pdpt[i].pfn     = pfn(pa_from_va(&page_table->pd[i][0]));
    }
    // Setup PD and PTE entries.
    //
    for (int i = 0; i < std::countof(page_table->pd); i++)
    {
        for (int j = 0; j < std::countof(page_table->pd); j++)
        {
            const auto pfn                   = (i * 512) + j;
            page_table->pd[i][j].read        = true;
            page_table->pd[i][j].write       = true;
            page_table->pd[i][j].execute     = true;
            page_table->pd[i][j].large_page  = true;
            page_table->pd[i][j].pfn         = pfn;
            page_table->pd[i][j].memory_type = mtrr.get_type_or(pfn * 2_mb, memory_type_t::write_back);
        }
    }
}

ept_t::~ept_t()
{
    operator delete(page_table, std::align_val_t{ PAGE_SIZE });
}
