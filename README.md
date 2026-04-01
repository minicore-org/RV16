# miniCore - Fast. Simple. Open.

[miniCore website](https://minicore.org)

**RV16 open architecture - Fast and free**

A 16-bit RISC-style soft core with a fully open stack, from RTL to compiler to MicroPython. No black boxes. No compromises.

---

## By the numbers

- **16-bit** - RV16 architecture
- **Open** - RTL to compiler
- **C + uPy** - Language support
- **iCE40** - FPGA target

---

## From silicon to Python.

Most open cores give you the RTL and leave you to figure out the rest. miniCore ships the entire vertical - ISA spec, RTL implementation, LLVM backend, assembler, linker, and MicroPython runtime. Every layer is documented, hackable, and open.

The goal isn't just a CPU. It's teaching you how every layer fits together, from gate to interpreter.

### The Full Stack

| Layer | Category | Status |
|---|---|---|
| MicroPython / C runtime | userland | coming soon |
| LLVM backend + linker | toolchain | coming soon |
| Assembler + ABI | toolchain | coming soon |
| ISA specification | RV16 | coming soon |
| RTL implementation | hardware | coming soon |
| FPGA synthesis (iCE40) | silicon | coming soon |

---

## Built for curious people.

### Makers and tinkerers

Run your own CPU on a cheap FPGA. Write C or MicroPython for hardware you fully understand. No vendor lock-in, no closed IP.

### Students and educators

Learn how an instruction travels from source code to a logic gate. Every layer is documented and designed to be read, not just used.

### Embedded engineers

A clean, auditable baseline for custom ISA extensions, PIO peripherals, or ASIC exploration. Dual-licensed for commercial use.

---

## Roadmap - Where we're going.

Building in public. Every phase is documented as it ships.

### Phase 0 - Core Infrastructure *(In progress)*

- ISA documentation
- LLVM backend (C)
- Cycle-accurate emulator
- Example programs

### Phase 1 - Soft Launch *(Up next)*

- Public GitHub release
- Community forum
- Blog and video tutorials
- MicroPython support

### Phase 2 - Hardware v1 *(Planned)*

- iCE40 FPGA board
- SPI / GPIO / timers
- Bootloader + JTAG
- Prebuilt toolchain

### Phase 3+ - Extend and Grow *(Future)*

- PIO FSM peripheral
- MUL + bitfield ops
- Structured course
- Community hackathons

---

## Follow the build.

miniCore is being built in public. Get occasional updates as new phases ship - ISA decisions, RTL notes, toolchain progress. No noise, no marketing.

---

## Join Us

The journey has just begun, and the best is ahead. Check out the blog for design updates, star the project on GitHub, and sign up for the mailing list to follow along as RV16 comes to life, moving from silicon to software.

---

*minicore.org - github.com/minicore - Dual licensed MIT / Commercial - Built in public*