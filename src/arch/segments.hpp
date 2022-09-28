#pragma once

#include <cstdint>

struct selector_t
{
    union
    {
        uint16_t flags;

        struct
        {
            /// @brief Requested Privilege Level.
            ///
            uint16_t rpl    : 2;
            /// @brief Table Indicator.
            ///
            uint16_t ti     : 1;
            /// @brief Index.
            ///
            uint16_t index  : 13;
        };
    };
};
static_assert(sizeof(selector_t) == sizeof(uint16_t), "segment selector size mismatch");

struct es_t   : public selector_t{};
struct cs_t   : public selector_t{};
struct ss_t   : public selector_t{};
struct ds_t   : public selector_t{};
struct fs_t   : public selector_t{};
struct gs_t   : public selector_t{};
struct tr_t   : public selector_t{};
struct ldtr_t : public selector_t{};

struct access_t
{
    union
    {
        uint32_t flags;

        struct
        {
            /// @brief Segment type.
            ///
            uint32_t type        : 4;
            /// @brief
            ///
            uint32_t dt          : 1;
            /// @brief Requested Privilege Level.
            ///
            uint32_t rpl         : 2;
            /// @brief Segment present.
            ///
            uint32_t present     : 1;
            /// @brief
            ///
            uint32_t _reserved1  : 4;
            /// @brief Available for use by system software.
            ///
            uint32_t avl         : 1;
            /// @brief 64-bit code segment (IA-32e mode only).
            ///
            uint32_t l           : 1;
            /// @brief Default operation size (0 = 16-bit segment; 1 = 32-bit segment).
            ///
            uint32_t db          : 1;
            /// @brief Granularity.
            ///
            uint32_t granularity : 1;
            /// @brief Segment is unsuable.
            ///
            uint32_t unusable    : 1;
        };
    };
};
static_assert(sizeof(access_t) == sizeof(uint32_t), "access_t size mismatch");

template<typename T>
struct segment_t
{
    /// @brief
    ///
    T selector;

    /// @brief
    ///
    uint32_t limit;

    /// @brief
    ///
    access_t rights;

    /// @brief
    ///
    uint64_t base;
};

struct descriptor_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// @brief Segment Limit.
            ///
            uint64_t limit_low  : 16;
            /// @brief Segment base address.
            ///
            uint64_t base_low   : 16;
            /// @brief Segment base address.
            ///
            uint64_t base_mid   : 8;
            /// @brief Segment type.
            ///
            uint64_t type       : 4;
            /// @brief Descriptor type.
            ///
            uint64_t system     : 1;
            /// @brief Descriptor privilege level.
            ///
            uint64_t dpl        : 2;
            /// @brief Segment present.
            ///
            uint64_t present    : 1;
            /// @brief Segment Limit.
            ///
            uint64_t limit_high : 4;
            /// @brief Available for use by system software.
            ///
            uint64_t avl        : 1;
            /// @brief 64-bit code segment (IA-32e mode only).
            ///
            uint64_t l          : 1;
            /// @brief Default operation size (0 = 16-bit segment; 1 = 32-bit segment).
            ///
            uint64_t db         : 1;
            /// @brief Granularity.
            ///
            uint64_t granularity: 1;
            /// @brief Segment base address.
            ///
            uint64_t base_high  : 8;
        };
    };

    /// @brief Segment base address.
    ///
    uint64_t base_upper   : 32;
    uint64_t must_be_zero : 32;

    inline uint64_t base() const
    {
        uint64_t base_address{};
        base_address |= static_cast<uint64_t>(base_low)  << 0;
        base_address |= static_cast<uint64_t>(base_mid)  << 16;
        base_address |= static_cast<uint64_t>(base_high) << 24;
        if (!system)
            base_address |= static_cast<uint64_t>(base_upper) << 32;
        return base_address;
    }

    inline uint64_t limit() const
    {
        return limit_low | limit_high << 8;
    }
};
static_assert(sizeof(descriptor_t) ==  16, "segment descriptor size mismatch");

/// @brief Descriptor table register.
///
#pragma pack(push, 1)
struct gdtr_t
{
    /// @brief
    ///
    uint16_t limit;
    /// @brief
    ///
    uint64_t base;
};
#pragma pack(pop)
static_assert(sizeof(gdtr_t) == 10, "Gdtr size mismatch");

/// @brief Same for interrupt descriptor table register.
///
struct idtr_t : public gdtr_t {};
static_assert(sizeof(idtr_t) == 10, "Idtr size mismatch");
