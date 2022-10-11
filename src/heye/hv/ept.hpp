#pragma once
#include "heye/arch/memory.hpp"
#include "heye/arch/mtrr.hpp"
#include "heye/arch/paging.hpp"

static constexpr auto pt_enties   = 512;
static constexpr auto page_walk_4 = 3;

struct page_table_t
{
    pml4_t   pml4[pt_enties];
    pdpt_t   pdpt[pt_enties];
    pd_2mb_t pd  [pt_enties][pt_enties];
};

struct ept_t final : public std::singleton<ept_t>
{
    friend struct std::singleton<ept_t>;

    inline eptp_t ept_pointer() { return ept; }

protected:
    ept_t();
    ~ept_t();

private:
    eptp_t ept{};
    page_table_t* page_table;
};
