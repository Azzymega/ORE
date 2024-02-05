#pragma once
#include "../libc/stdlib.hpp"

using Object = uint64_t;
using Byte = uint8_t;

static inline void PWriteB(uint32_t port, u1 data)
{
    __asm__ volatile("outb %b0, %w1" : : "a" (data), "Nd" (port));
}

static inline u1 PReadB(uint32_t port)
{
    u1 data;
    __asm__ volatile("inb %w1, %b0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline void PWriteS(uint32_t port, u2 data)
{
    __asm__ volatile("outw %w0, %w1" : : "a" (data), "Nd" (port));
}

static inline u2 PReadS(uint32_t port)
{
    u2 data;
    __asm__ volatile("inw %w1, %w0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline void PWriteI(uint32_t port, u4 data)
{
    __asm__ volatile("outl %0, %w1" : : "a" (data), "Nd" (port));
}

static inline u4 PReadI(uint32_t port)
{
    u4 data;
    __asm__ volatile("inl %w1, %0" : "=a" (data) : "Nd" (port));
    return data;
}
