---
title: "miniCore - Fast, Simple, Open."
slug: minicore-simple-fast-open
date: 2026-03-30T14:49:51
---
_**RV16 open architecture - Fast and free**_

A 16-bit RISC-style soft core with a fully open stack -- from RTL to compiler to MicroPython. Designed for the gap where 32-bit is too large and 8-bit isn't enough. Built on FPGA. Heading to silicon. No black boxes. No compromises.

[View on GitHub](https://claude.ai/chat/d5ca26e2-0af9-4027-885e-940944b06a44#) | [Follow the build](https://claude.ai/chat/d5ca26e2-0af9-4027-885e-940944b06a44#newsletter)

* * *

## By the numbers

*   **16-bit** - RV16 architecture
*   **Open** - RTL to compiler
*   **C + uPy** - Language support
*   **iCE40** - FPGA target
*   **ASIC-ready** -- portable RTL, no vendor primitives

* * *

## From silicon to Python.

Most open cores give you the RTL and leave you to figure out the rest. miniCore ships the entire vertical - ISA spec, RTL implementation, LLVM backend, assembler, linker, and MicroPython runtime. Every layer is documented, hackable, and open.

The goal isn't just a CPU. It's teaching you how every layer fits together, from gate to interpreter.

### The Full Stack

<!-- wp:table -->
<figure class="wp-block-table"><table class="has-fixed-layout"><thead><tr><th>Layer</th><th>Category</th><th>Status</th></tr></thead><tbody><tr><td>MicroPython / C runtime</td><td>userland</td><td>coming soon</td></tr><tr><td>LLVM backend + linker</td><td>toolchain</td><td>coming soon</td></tr><tr><td>Assembler + ABI</td><td>toolchain</td><td>coming soon</td></tr><tr><td>ISA specification</td><td>RV16</td><td>coming soon</td></tr><tr><td>RTL implementation</td><td>hardware</td><td>coming soon</td></tr><tr><td>FPGA synthesis (iCE40)</td><td>silicon</td><td>coming soon</td></tr></tbody></table></figure>
<!-- /wp:table -->


## Built for curious people.

### Makers and tinkerers

Run your own CPU on an inexpensive FPGA. Write C or MicroPython for hardware you fully understand. No vendor lock-in, no closed IP.

### Students and educators

Learn how an instruction travels from source code to a logic gate. Every layer is documented and designed to be read, not just used.

### Embedded engineers

A clean, auditable 16-bit core for companion-processor roles - handling I/O, housekeeping, and protocol offload where 32-bit is overkill and 8-bit falls short. ASIC-portable RTL with a commercial license path for serious integrators.

* * *

## Roadmap - Where we're going.

Building in public. Every phase is documented.

### Phase 0 (V0) - Core Infrastructure _(In progress)_

*   ISA definition and documentation
*   RTL implementation
*   LLVM/Clang toolchain support
*   Cycle-accurate emulator

### Phase 1 (V1) - Soft Launch + Prototype FPGA

*   Public GitHub release
*   Prototype FPGA implementation
*   Blog and video tutorials
*   MicroPython support

### Phase 2 (V2) - DIP-40 Hardware Release

*   iCE40 FPGA board (DIP-40, USB-C powered)
*   Full PIO engine
*   SPI / GPIO / timers / JTAG
*   Prebuilt toolchain

### Phase 3+ (V3+) - ASIC-Ready and Beyond

*   OpenLane / Sky130 PDK validation
*   ASIC-portable RTL release
*   Third-party integration support
*   Structured course

* * *

## Open RTL. Open tools. One architecture.

We prove and optimize the RTL on FPGA, and mature the full software stack alongside it. The RTL is the product -- the goal is open, auditable silicon that integrators, makers, and educators can build on.

## Follow the build.

miniCore is being built in public. Get occasional updates as new phases ship - ISA decisions, RTL notes, toolchain progress. No noise, no marketing.

[Subscribe via Buttondown](https://claude.ai/chat/d5ca26e2-0af9-4027-885e-940944b06a44#) <!-- Replace with your Buttondown embed or link -->

_Powered by Buttondown - No tracking - Unsubscribe anytime_

* * *

## Join Us

The journey has just begun, and the best is ahead. Check out the blog for design updates, star the project on GitHub, and sign up for the mailing list to follow along as RV16 comes to life, moving from silicon to software.

[Star on GitHub](https://claude.ai/chat/d5ca26e2-0af9-4027-885e-940944b06a44#) | [Get updates](https://claude.ai/chat/d5ca26e2-0af9-4027-885e-940944b06a44#newsletter)

* * *

_minicore.org - github.com/minicore - Dual licensed MIT / Commercial - Built in public_
