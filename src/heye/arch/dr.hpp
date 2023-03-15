#pragma once

#include <cstdint>

namespace heye
{
struct dr7_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// Local enable for breakpoint in DR0.
            ///
            uint64_t dr0_enable_local   : 1;
            /// Global enable for breakpoint in DR0.
            ///
            uint64_t dr0_enable_global  : 1;
            /// Local enable for breakpoint in DR1.
            ///
            uint64_t dr1_enable_local   : 1;
            /// Global enable for breakpoint in DR1.
            ///
            uint64_t dr1_enable_global  : 1;
            /// Local enable for breakpoint in DR2.
            ///
            uint64_t dr2_enable_local   : 1;
            /// Global enable for breakpoint in DR2.
            ///
            uint64_t dr2_enable_global  : 1;
            /// Local enable for breakpoint in DR3.
            ///
            uint64_t dr3_enable_local   : 1;
            /// Global enable for breakpoint in DR3.
            ///
            uint64_t dr3_enable_global  : 1;
            /// Exact data breakpoint.
            ///
            uint64_t le                 : 1;
            /// Exact data breakpoint.
            ///
            uint64_t ge                 : 1;
            /// @brief
            ///
            uint64_t _reserved1         : 5;
            /// DR0 breakpoint type.
            ///
            uint64_t dr0_bp_type        : 2;
            /// DR0 breakpoint size in bytes.
            ///
            uint64_t dr0_bp_size        : 2;
            /// DR1 breakpoint type.
            ///
            uint64_t dr1_bp_type        : 2;
            /// DR1 breakpoint size in bytes.
            ///
            uint64_t dr1_bp_size        : 2;
            /// DR2 breakpoint type.
            ///
            uint64_t dr2_bp_type        : 2;
            /// DR2 breakpoint size in bytes.
            ///
            uint64_t dr2_bp_size        : 2;
            /// DR3 breakpoint type.
            ///
            uint64_t dr3_bp_type        : 2;
            /// DR3 breakpoint size in bytes.
            ///
            uint64_t dr3_bp_size        : 2;
        };
    };
};
};
