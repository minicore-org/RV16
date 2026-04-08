# RV16 Instruction Summary

## Register File

| Registers | Purpose                         |
|-----------|---------------------------------|
| R0–R3     | Return values / arguments       |
| R4–R7     | General purpose (caller-saved)  |
| R8-R11    | General purpose <br>R11 may be used as FP (callee saved) |
| R12       | Link Register (LR)              |
| R13       | Data Page register. Far memory (callee saved) |
| R14       | Stack pointer (SP)              |
| R15       | Zero                            |

- 2 read ports + 1 write port per register
- Register pairs used for 32-bit operations
  - Even:odd only, R0:R1, R2:R3, R4:R5, R6:R7, R8:R9, R10:R11
    - Encoding maps the even registers: 0:R0, 1:R2, 2:R4, 3:R6, 4:R8, 5:R10
    - In assembly, only specify the even register in the operand, e.g. `ADD32 R6,R8`
    - R10:R11 valid only if R11 is not used as FP
  - ADD32, SUB32, CMP32, MOV32 are 32-bits operations `op Rd{:Rd+1},Rs{:Rs+1}`
  - MULU, MULS: `op Rd{:Rd+1},Rs`
- R13 / DP, only bits[7-0] are used. Bits [15-8] read/write ignored by hardware. Interrupt handlers must save/restore all 16 bits of R13 for full context save, even though only bits[7:0] are architecturally meaningful.
- Orthogonal design: general registers usable as pointers
- Zero register, read-only, write has no effect

---

## Flag Key

| Symbol | Meaning |
|--------|---------|
| `*` | updated based on result |
| `-` | unchanged |
| `0` | always cleared |
| `1` | always set |
| `u` | undefined / unpredictable |

Flags: **V** = overflow, **N** = negative, **C** = carry, **Z** = zero

---

## Arithmetic

| Instruction | Operands | Operation | V | N | C | Z | Notes |
|-------------|----------|-----------|---|---|---|---|-------|
| ADD | Rd, Rs | Rd ← Rd + Rs | * | * | * | * | |
| ADDC | Rd, Rs | Rd ← Rd + Rs + C | * | * | * | * | add with carry |
| ADDI | Rd, #imm6 | Rd ← Rd + imm6 | * | * | * | * | imm6 sign-extended |
| ADDI.16 | Rd, #imm16 | Rd ← Rd + imm16 | * | * | * | * | 32-bit instruction |
| SUB | Rd, Rs | Rd ← Rd - Rs | * | * | * | * | |
| SUBC | Rd, Rs | Rd ← Rd - Rs - C | * | * | * | * | subtract with carry |
| SUBI | Rd, #imm6 | Rd ← Rd - imm6 | * | * | * | * | imm6 sign-extended |
| CMP | Rd, Rs | Rd - Rs, result discarded | * | * | * | * | flags only |
| CMPI | Rd, #imm6 | Rd - imm6, result discarded | * | * | * | * | flags only, imm6 sign-extended |
| NEG | Rd, Rs | Rd ← 0 - Rs | * | * | * | * | |
| ADD32 | Rd, Rs | Rd:Rd+1 ← Rd:Rd+1 + Rs:Rs+1 | * | * | * | * | even reg pairs only |
| SUB32 | Rd, Rs | Rd:Rd+1 ← Rd:Rd+1 - Rs:Rs+1 | * | * | * | * | even reg pairs only |
| CMP32 | Rd, Rs | Rd:Rd+1 - Rs:Rs+1, discarded | * | * | * | * | flags only, even reg pairs |
| MULU | Rd, Rs | Rd:Rd+1 ← Rd * Rs (unsigned) | 0 | 0 | 0 | * | ARM convention: V,N,C cleared |
| MULS | Rd, Rs | Rd:Rd+1 ← Rd * Rs (signed) | 0 | 0 | 0 | * | ARM convention: V,N,C cleared |
| DIV\* | Rd, Rs | Rd ← Rd / Rs (signed) | - | - | - | * | remainder in Rd+1, rsvd V1 |
| DIVU\* | Rd, Rs | Rd ← Rd / Rs (unsigned) | - | - | - | * | remainder in Rd+1, rsvd V1 |
| MOD\* | Rd, Rs | Rd ← Rd mod Rs (signed) | - | - | - | * | rsvd V1 |
| MODU\* | Rd, Rs | Rd ← Rd mod Rs (unsigned) | - | - | - | * | rsvd V1 |
| FMA | Rd, Ra, Rb | Rd:Rd+1 ← Rd:Rd+1 + Ra * Rb | - | - | - | * | 32-bit instruction, even reg pairs |

---

## Logical

| Instruction | Operands | Operation | V | N | C | Z | Notes |
|-------------|----------|-----------|---|---|---|---|-------|
| AND | Rd, Rs | Rd ← Rd & Rs | 0 | * | - | * | |
| ANDI | Rd, #imm6 | Rd ← Rd & imm6 | 0 | * | - | * | imm6 zero-extended |
| OR | Rd, Rs | Rd ← Rd \| Rs | 0 | * | - | * | |
| ORI | Rd, #imm6 | Rd ← Rd \| imm6 | 0 | * | - | * | imm6 zero-extended |
| XOR | Rd, Rs | Rd ← Rd ^ Rs | 0 | * | - | * | |
| XORI | Rd, #imm6 | Rd ← Rd ^ imm6 | 0 | * | - | * | imm6 zero-extended |
| NOT | Rd, Rs | Rd ← ~Rs | 0 | * | - | * | |
| TST | Rd, Rs | Rd & Rs, result discarded | 0 | * | - | * | flags only |

---

## Shift / Bit / Extend

All shift and rotate instructions operate on a single bit at a time.
C always receives the last bit shifted or rotated out.

| Instruction | Operands | Operation | V | N | C | Z | Notes |
|-------------|----------|-----------|---|---|---|---|-------|
| SHL | Rd | Rd ← Rd << 1 | 0 | * | * | * | C ← bit[15] of Rd |
| SHR | Rd | Rd ← Rd >> 1 (logical) | 0 | * | * | * | C ← bit[0] of Rd |
| ASR | Rd | Rd ← Rd >> 1 (arithmetic) | 0 | * | * | * | C ← bit[0] of Rd, sign preserved |
| ROL | Rd | Rd ← Rd rotated left 1 | 0 | * | * | * | C ← bit[15], bit[0] ← old bit[15] |
| ROR | Rd | Rd ← Rd rotated right 1 | 0 | * | * | * | C ← bit[0], bit[15] ← old bit[0] |
| RRX | Rd | Rd ← (C << 15) \| (Rd >> 1) | 0 | * | * | * | C ← bit[0] of Rd; rotate through carry |
| SHL.V\* | Rd, Rs | Rd ← Rd << Rs | 0 | * | * | * | variable shift, ext |
| SHR.V\* | Rd, Rs | Rd ← Rd >> Rs (logical) | 0 | * | * | * | variable shift, ext |
| ASR.V\* | Rd, Rs | Rd ← Rd >> Rs (arithmetic) | 0 | * | * | * | variable shift, ext |
| CLZ | Rd | Rd ← count leading zeros of Rd | 0 | * | 0 | * | Z reflects result; CLZ(0) = 16 |
| CTZ | Rd | Rd ← count trailing zeros of Rd | 0 | * | 0 | * | Z reflects result; CTZ(0) = 16 |
| BSWAP | Rd | Rd ← byte-swap Rd | 0 | * | - | * | swap high and low bytes |
| POPCOUNT | Rd | Rd ← count of set bits in Rd | 0 | * | 0 | * | |
| SEXT | Rd | Rd ← sign-extend Rd[7:0] to 16 bits | 0 | * | - | * | |
| TRUNC | Rd | Rd ← zero-extend Rd[7:0] to 16 bits | 0 | * | - | * | clears upper byte |
| SETCC | Rd, Cond | Rd ← 1 if Cond true, else 0 | - | - | - | - | flags unchanged; Cond reuses branch encoding |

---

## Move / Load Immediate

| Instruction | Operands | Operation | V | N | C | Z | Notes |
|-------------|----------|-----------|---|---|---|---|-------|
| MOV | Rd, Rs | Rd ← Rs | - | * | - | * | |
| MOV32 | Rd, Rs | Rd:Rd+1 ← Rs:Rs+1 | - | * | - | * | N reflects sign of high word (Rd); even reg pairs |
| MOV PC | Rd | Rd ← PC | - | - | - | - | PC = address of this instruction |
| LDI | Rd, #imm6 | Rd ← imm6 | - | * | - | * | imm6 sign-extended |
| LDI.10 | Rd, #imm10 | Rd ← imm10 | - | * | - | * | imm10 signed; Rd ∈ R0-R3 only |
| LDI.16 | Rd, #imm16 | Rd ← imm16 | - | * | - | * | 32-bit instruction |

---

## Load

Z flag reflects the loaded value. V, N, C unchanged on all loads.
POP does not affect any flags.

| Instruction | Operands | Operation | V | N | C | Z | Notes |
|-------------|----------|-----------|---|---|---|---|-------|
| LD | Rd, [Rs] | Rd ← MEM[Rs] | - | - | - | * | base only |
| LD | Rd, [Rs]+ | Rd ← MEM[Rs], Rs ← Rs+2 | - | - | - | * | post-increment |
| LD | Rd, [-SP] | SP ← SP-2, Rd ← MEM[SP] | - | - | - | * | pre-dec, SP only; trap if Rs ≠ SP |
| LD | Rd, [Rs+off] | Rd ← MEM[Rs + off*2] | - | - | - | * | Rs ∈ R0-R7; off5 unsigned |
| LD.far | Rd, [Rs] | Rd ← MEM[DP:Rs] | - | - | - | * | EA = DP<<16 \| Rs |
| LD.UB | Rd, [Rs] | Rd ← zero-extend(MEM[Rs]) | - | - | - | * | |
| LD.UB | Rd, [Rs]+ | Rd ← zero-extend(MEM[Rs]), Rs ← Rs+1 | - | - | - | * | post-increment |
| LD.UB | Rd, [Rs+off] | Rd ← zero-extend(MEM[Rs+off]) | - | - | - | * | Rs ∈ R0-R7; off4 unsigned |
| LD.UB.far | Rd, [Rs] | Rd ← zero-extend(MEM[DP:Rs]) | - | - | - | * | |
| LD.SB | Rd, [Rs] | Rd ← sign-extend(MEM[Rs]) | - | - | - | * | |
| LD.SB | Rd, [Rs]+ | Rd ← sign-extend(MEM[Rs]), Rs ← Rs+1 | - | - | - | * | post-increment |
| LD.SB | Rd, [Rs+off] | Rd ← sign-extend(MEM[Rs+off]) | - | - | - | * | Rs ∈ R0-R7; off4 unsigned |
| LD.SB.far | Rd, [Rs] | Rd ← sign-extend(MEM[DP:Rs]) | - | - | - | * | |
| POP | Rd | Rd ← MEM[SP], SP ← SP+2 | - | - | - | - | pseudo for LD Rd, [SP]+ |

---

## Store

Store instructions do not affect any flags.

| Instruction | Operands | Operation | V | N | C | Z | Notes |
|-------------|----------|-----------|---|---|---|---|-------|
| ST | Rs, [Rd] | MEM[Rd] ← Rs | - | - | - | - | base only |
| ST | Rs, [Rd]+ | MEM[Rd] ← Rs, Rd ← Rd+2 | - | - | - | - | post-increment |
| ST | Rs, [-Rd] | Rd ← Rd-2, MEM[Rd] ← Rs | - | - | - | - | pre-decrement |
| ST | Rs, [Rd+off] | MEM[Rd + off*2] ← Rs | - | - | - | - | Rd ∈ R0-R7; off5 unsigned |
| ST.far | Rs, [Rd] | MEM[DP:Rd] ← Rs | - | - | - | - | EA = DP<<16 \| Rd |
| ST.B | Rs, [Rd] | MEM[Rd] ← Rs[7:0] | - | - | - | - | |
| ST.B | Rs, [Rd]+ | MEM[Rd] ← Rs[7:0], Rd ← Rd+1 | - | - | - | - | post-increment |
| ST.B | Rs, [Rd+off] | MEM[Rd+off] ← Rs[7:0] | - | - | - | - | Rd ∈ R0-R7; off4 unsigned |
| ST.B.far | Rs, [Rd] | MEM[DP:Rd] ← Rs[7:0] | - | - | - | - | |
| PUSH | Rs | SP ← SP-2, MEM[SP] ← Rs | - | - | - | - | pseudo for ST Rs, [-SP] |
| PUSHM | #mask10 | push R13-R4 per mask | - | - | - | - | highest reg first; bit[9]=R13, bit[0]=R4 |
| POPM | #mask10 | pop R4-R13 per mask | - | - | - | - | lowest reg first; bit[0]=R4, bit[9]=R13 |

---

## Branch / Call / Return

Branch instructions do not affect any flags.
All offsets are word offsets (offset * 2 added to PC).

| Instruction | Operands | Operation | V | N | C | Z | Notes |
|-------------|----------|-----------|---|---|---|---|-------|
| BR | Cond, #off8 | PC ← PC + off*2 if Cond | - | - | - | - | ±256 byte reach |
| BR | #off11 | PC ← PC + off*2 | - | - | - | - | unconditional; ±4KB reach |
| RCALL | #off11 | LR ← PC+2, PC ← PC + off*2 | - | - | - | - | ±4KB reach |
| CALLR | Rs | LR ← PC+2, PC ← Rs | - | - | - | - | LR = R12; Rs can be R15 (call addr 0) |
| JMPR | Rs | PC ← Rs | - | - | - | - | Rs can be R15 (jump to addr 0) |
| CALL | #addr16 | LR ← PC+2, PC ← addr16 | - | - | - | - | 32-bit instruction; LR = R12 |
| JMP | #addr16 | PC ← addr16 | - | - | - | - | 32-bit instruction |
| RET | | PC ← LR | - | - | - | - | pseudo for JMPR R12 |

---

## System / Control

System instructions do not affect flags unless noted.

| Instruction | Operands | Operation | V | N | C | Z | Notes |
|-------------|----------|-----------|---|---|---|---|-------|
| SWI | #imm10 | software interrupt | - | - | - | - | 1024 syscall slots |
| SWAPW | Rd, [Rs] | temp←MEM[Rs], MEM[Rs]←Rd, Rd←temp | - | - | - | - | atomic word swap |
| FENCE | | memory barrier | - | - | - | - | C11 atomics / DMA coherency |
| WFI | | wait for interrupt | - | - | - | - | low power mode |
| HLT | | halt processor | - | - | - | - | |
| NOP | | no operation | - | - | - | - | |

---

## Condition Codes

Used by BR (conditional) and SETCC. Reuses the same 4-bit encoding in both.

| Cond | Code | Condition | Flags tested |
|------|------|-----------|--------------|
| EQ | `0000` | equal | Z = 1 |
| NE | `0001` | not equal | Z = 0 |
| LT | `0010` | less than (signed) | N ≠ V |
| GE | `0011` | greater or equal (signed) | N = V |
| LE | `0100` | less or equal (signed) | Z = 1 or N ≠ V |
| GT | `0101` | greater than (signed) | Z = 0 and N = V |
| LTU | `0110` | less than (unsigned) | C = 0 |
| GEU | `0111` | greater or equal (unsigned) | C = 1 |
| LEU | `1000` | less or equal (unsigned) | C = 0 or Z = 1 |
| GTU | `1001` | greater than (unsigned) | C = 1 and Z = 0 |
| MI | `1010` | minus / negative | N = 1 |
| PL | `1011` | plus / positive or zero | N = 0 |
| VS | `1100` | overflow set | V = 1 |
| VC | `1101` | overflow clear | V = 0 |
| AL | `1110` | always (unconditional) | — |
| NV | `1111` | never (reserved) | — |

---

## Notes

- `*` reserved instructions (DIV, DIVU, MOD, MODU) are not implemented in V0/V1.
- `*(ext)` variable shift instructions (SHL.V, SHR.V, ASR.V) are reserved for a future extension, not implemented in V0/V1.
- **32-bit instructions** occupy two consecutive 16-bit words. The first word always has OP[15:12] = `1110`.
- All **immediate values** are sign-extended unless stated otherwise (ANDI/ORI/XORI use zero-extension).
- **Pseudo-instructions** (PUSH, POP, RET) are assembler conveniences that expand to a single real instruction.