#pragma once
#include "heye/arch/memory.hpp"
#include "heye/arch/mtrr.hpp"
#include "heye/arch/paging.hpp"

namespace heye
{
static constexpr auto pt_enties   = 512;
static constexpr auto page_walk_4 = 3;

struct page_table_t
{
    pml4_t   pml4[pt_enties];
    pdpt_t   pdpt[pt_enties];
    pd_2mb_t pd  [pt_enties][pt_enties];
};

struct ept_t final
{
    ept_t ();
    ~ept_t();

    eptp_t ept_pointer() const;

private:
    eptp_t ept{};
    page_table_t* page_table;
};
};
