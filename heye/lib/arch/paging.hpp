#pragma once

#include <cstdint>

struct eptp_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// @brief EPT paging-structure memory type.
            /// 0 = Uncacheable (UC)
            /// 6 = Write-back  (WB)
            ///
            uint64_t memory_type      : 3;
            /// @brief This value less than the EPT page walk length.
            ///
            uint64_t page_walk_length : 3;
            /// @brief Setting this control to 1 enables accessed and dirty flags for EPT.
            ///
            uint64_t access_flags     : 1;
            /// @brief
            ///
            uint64_t _reserved1       : 5;
            /// @brief Physical address of EPT PML4 table.
            ///
            uint64_t pml4_address     : 36;
            /// @brief
            ///
            uint64_t _reserved2       : 16;
        };
    };
};

struct pml4_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// @brief Read access.
            ///
            uint64_t read             : 1;
            /// @brief Write access.
            ///
            uint64_t write            : 1;
            /// @brief Execute access.
            ///
            uint64_t execute          : 1;
            /// @brief
            ///
            uint64_t _reserved1       : 5;
            /// @brief Indicates whether software has accessed this region.
            ///
            uint64_t accessed         : 1;
            /// @brief
            ///
            uint64_t _reserved2       : 1;
            /// @brief Execute access for usermode linear address.
            ///
            uint64_t execute_usermode : 1;
            /// @brief
            ///
            uint64_t _reserved3       : 1;
            /// @brief Physical address of PDPT.
            ///
            uint64_t pfn : 36;
            /// @brief
            ///
            uint64_t _reserved4      : 16;
        };
    };
};
static_assert(sizeof(pml4_t) == sizeof(uint64_t), "EPT PML4 Entry size mismatch");

struct pdpt_1gb_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// @brief Read access.
            ///
            uint64_t read             : 1;
            /// @brief Write access.
            ///
            uint64_t write            : 1;
            /// @brief Execute access.
            ///
            uint64_t execute          : 1;
            /// @brief EPT memory type for this 1-GByte page.
            ///
            uint64_t memory_type      : 3;
            /// @brief Ignore PAT memory type for this 1-GByte page.
            ///
            uint64_t ignore_pat       : 1;
            /// @brief Must be 1 (otherwise, this entry references an EPT page directory).
            ///
            uint64_t large_page       : 1;
            /// @brief Indicates whether software has accessed this region.
            ///
            uint64_t accessed         : 1;
            /// @brief Indicates whether software has written to the 1-GByte page referenced by this entry.
            ///
            uint64_t dirty            : 1;
            /// @brief Execute access for usermode linear address.
            ///
            uint64_t execute_usermode : 1;
            /// @brief
            ///
            uint64_t _reserved1       : 19;
            /// @brief Physical address of PD.
            ///
            uint64_t pfn              : 18;
            /// @brief
            ///
            uint64_t _reserved2       : 15;
            /// @brief Suppress #VE.
            ///
            uint64_t suppress_ve      : 1;
        };
    };
};
static_assert(sizeof(pdpt_1gb_t) == sizeof(uint64_t), "EPT PDPT 1GB Entry size mismatch");

struct pdpt_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// @brief Read access.
            ///
            uint64_t read             : 1;
            /// @brief Write access.
            ///
            uint64_t write            : 1;
            /// @brief Execute access.
            ///
            uint64_t execute          : 1;
            /// @brief
            ///
            uint64_t _reserved1       : 5;
            /// @brief Indicates whether software has accessed this region.
            ///
            uint64_t accessed         : 1;
            /// @brief
            ///
            uint64_t _reserved2       : 1;
            /// @brief Execute access for usermode linear address.
            ///
            uint64_t execute_usermode : 1;
            /// @brief
            ///
            uint64_t _reserved3       : 1;
            /// @brief Physical address of PD.
            ///
            uint64_t pfn              : 36;
            /// @brief
            ///
            uint64_t _reserved4       : 16;
        };
    };
};
static_assert(sizeof(pdpt_t) == sizeof(uint64_t), "EPT PDPT Entry size mismatch");

struct pd_2mb_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// @brief Read access.
            ///
            uint64_t read             : 1;
            /// @brief Write access.
            ///
            uint64_t write            : 1;
            /// @brief Execute access.
            ///
            uint64_t execute          : 1;
            /// @brief EPT memory type for this 1-GByte page.
            ///
            uint64_t memory_type      : 3;
            /// @brief Ignore PAT memory type for this 1-GByte page.
            ///
            uint64_t ignore_pat       : 1;
            /// @brief Must be 1 (otherwise, this entry references an EPT page table).
            ///
            uint64_t large_page       : 1;
            /// @brief Indicates whether software has accessed this region.
            ///
            uint64_t accessed         : 1;
            /// @brief Indicates whether software has written to the 1-GByte page referenced by this entry.
            ///
            uint64_t dirty            : 1;
            /// @brief Execute access for usermode linear address.
            ///
            uint64_t execute_usermode : 1;
            /// @brief
            ///
            uint64_t _reserved1       : 10;
            /// @brief Physical address of PD.
            ///
            uint64_t pfn              : 27;
            /// @brief
            ///
            uint64_t _reserved2       : 15;
            /// @brief Suppress #VE.
            ///
            uint64_t suppress_ve      : 1;
        };
    };
};
static_assert(sizeof(pd_2mb_t) == sizeof(uint64_t), "EPT PD 2MB Entry size mismatch");

struct pd_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// @brief Read access.
            ///
            uint64_t read             : 1;
            /// @brief Write access.
            ///
            uint64_t write            : 1;
            /// @brief Execute access.
            ///
            uint64_t execute          : 1;
            /// @brief
            ///
            uint64_t _reserved1       : 5;
            /// @brief Indicates whether software has accessed this region.
            ///
            uint64_t accessed         : 1;
            /// @brief
            ///
            uint64_t _reserved2       : 1;
            /// @brief Execute access for usermode linear address.
            ///
            uint64_t execute_usermode : 1;
            /// @brief
            ///
            uint64_t _reserved3       : 1;
            /// @brief Physical address of PD.
            ///
            uint64_t pfn : 36;
            /// @brief
            ///
            uint64_t _reserved4       : 16;
        };
    };
};
static_assert(sizeof(pd_t) == sizeof(uint64_t), "EPT PD Entry size mismatch");

struct pte_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// @brief Read access.
            ///
            uint64_t read             : 1;
            /// @brief Write access.
            ///
            uint64_t write            : 1;
            /// @brief Execute access.
            ///
            uint64_t execute          : 1;
            /// @brief
            ///
            uint64_t memory_type      : 3;
            /// @brief
            ///
            uint64_t ignore_pat       : 1;
            /// @brief
            ///
            uint64_t _reserved1       : 1;
            /// @brief Indicates whether software has accessed this region.
            ///
            uint64_t accessed         : 1;
            /// @brief Indicates whether software has modified this region.
            ///
            uint64_t dirty            : 1;
            /// @brief Execute access for usermode linear address.
            ///
            uint64_t execute_usermode : 1;
            /// @brief
            ///
            uint64_t _reserved2       : 1;
            /// @brief Physical address of PD.
            ///
            uint64_t pfn : 36;
            /// @brief
            ///
            uint64_t _reserved3       : 15;
            /// @brief Suppress #VE.
            ///
            uint64_t suppress_ve      : 1;
        };
    };
};
static_assert(sizeof(pte_t) == sizeof(uint64_t), "EPT PTE Entry size mismatch");
