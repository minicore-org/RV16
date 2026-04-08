# RV16 Instruction Encoding


## Instruction Encoding

| OP [15:12] | Group | Encoding form | Key instructions | Headroom |
|------------|-------|---------------|-----------------|----------|
| `0000` | Arithmetic Rd, Rs | `OP·Rd[4]·Rs[4]·Func[4]` | ADD, ADDC, SUB, SUBC, ADD32, SUB32, CMP, CMP32, MULU, MULS, <br><br>DIV\*, DIVU\*, MOD\*, MODU\* | 2/16 funcs free |
| `0001` | Logical + MOV | `OP·Rd[4]·Rs[4]·Func[4]` | AND, OR, XOR, NOT, NEG, TST, MOV, MOV32, <br><br>MOV PC `MOV Rd,PC`, Rs must be zero, *trap* otherwise, PC is of the current instruction | 7/16 free |
| `0010` | Shift / bit / extend / indirect | see sub-table below | SHL, SHR, ASR, ROR, ROL, RRX (rotate right through carry, 17-bit rotate), CLZ, CTZ, BSWAP, POPCOUNT, TRUNC, SEXT, CALLR, JMPR, SETCC, SHL.V\*, SHR.V\*, ASR.V\* | see sub-table |
| `0011` | Arithmetic immediate | `OP·Rd[4]·imm6[6]·Func[2]` | ADDI, SUBI, LDI, CMPI, #imm6 is signed | 0/4 free |
| `0100` | Logical immediate | `OP·Rd[4]·imm6[6]·Func[2]` | ANDI, ORI, XORI, , #imm6 is unsigned | 1/4 free |
| `0101` | Load Wide immediate | `OP·Rd[2]·imm10[10]` | LDI.10 (Rd ∈ R0–R3), #imm10 is signed | - |
| `0110` | Load base | `OP·Rd[4]·Rs[4]·SzSg[2]·Mode[2]` | LD[.far], LD.UB[.far], LD.SB[.far], POP (SP post-inc)  | SzSg[2] = 00: word, 01: *trap*, 10: byte zero extended, 11: byte signed extended. Mode[2] = 00: base, 01: post-inc, 10: pre-dec (SP only *trap* otherwise), 11: far (SzSg valid) |
| `0111` | Store base | `OP·Rs[4]·Rd[4]·Sz·0·Mode[2]` | ST[.far], ST.B[.far], PUSH (SP pre-dec) | Sz = 0: word, 1: byte. Mode[2] = 00: base, 01: post-inc, 10: pre-dec, 11: far (Sz valid). bit[2] must be 0, trap if set |
| `1000` | Load word + offset | `OP·Rd[4]·Rs[3]·off5[5]` | LD offset (Rs is the base register, Rs ∈ R0–R7), EA = Rs + (offset*2) | — |
| `1001` | Load byte + offset | `OP·Rd[4]·Rs[3]·Sg·off4[4]` | LD.UB / LD.SB offset (Rs is the base register, Rs ∈ R0–R7), EA = Rs + offset | — |
| `1010` | Store word + offset | `OP·Rs[4]·Rd[3]·off5[5]` | ST offset (Rd is the base register, Rd ∈ R0–R7), EA = Rd + (offset*2) | — |
| `1011` | Store byte + offset | `OP·Rs[4]·Rd[3]·off4[4]` | ST.B offset (Rd is the base register, Rd ∈ R0–R7), EA = Rd + offset | — |
| `1100` | Conditional branch | `OP·Cond[4]·off8[8]` | BR (all conditions), EA = PC + (offset*2) | 7/16 cond slots free |
| `1101` | Unconditional branch | `OP·Func[1]·off11[11]` | BR (uncond), RCALL, EA = PC + (offset*2) | — |
| `1110` | 32-bit prefix | `OP·00·Func[6]·Rg[4]` + `operand[16]` | CALL, JMP, ADDI.16, LDI.16, LD addr16, ST addr16, LR (ext+), SC (ext+), Rg[4] source or destination register, second word `Operand[16]` contains address (#imm) or other 16-bit operand values. | 64 - 8 = 56 free Funcs |
| `1110` | FMA / MAC | `OP·01·00·Ra[4]·Rb[4]` + `Rd[4]·000000000000` | FMA / MAC, `Rd:Rd+1 ← Rd:Rd+1 + Ra * Rb`, bit[11:8] of the first word must be `0100`, *trap* otherwise, bit[11:0] of the second word must be 0s, otherwise *trap* | - | 
| `1110` | 32-bit reserved | `OP·10·Reserved[10]` + `Operand[16]` | Reserved | - |
| `1110` | 32-bit reserved | `OP·11·Reserved[10]` + `Operand[16]` | Reserved | - | 
| `1111` | SWI | `OP·00·imm[10]` | SWI #imm10 | - |
| `1111` | PUSHM | `OP·01·imm[10]` | PUSHM #mask10, maps to R13..R4, bit N = R(N+4), highest registers push first | - |
| `1111` | POPM | `OP·10·imm[10]` | POPM #mask10, maps to R13..R4, bit N = R(N+4), lowest registers pop first | - |
| `1111` | System / control | `OP·11·0·Func[9]` | RET, NOP, HLT, WFI, FENCE | 507/512 free |
| `1111` | SWAPW | `OP·11·1·0·Rd[4]Rs[4]` | SWAPW | - |
| `1111` | Reserved | `OP·11·1·1·Reserved[8]` | Reserved | - |

#### `0010` sub-group dispatch (bits [10:8])

Bits [15:12] = `0010`. The hardware muxes on bits [10:8] after the primary opcode decode to select one of the encoding forms.

| Bits [10:8] | Sub-group | Encoding form | Instructions |
|-------------|-----------|---------------|--------------|
| `000` | Shift / bit / extend | `OP·Rd[4]·000·Func[5]` | SHL, SHR, ASR, ROL, ROR, CLZ, CTZ, BSWAP, POPCOUNT, TRUNC, SEXT — 21/32 free |
| `001` | Indirect call | `OP·Rs[4]·001·0000` | CALLR Rs — jump to Rs, write return address to R12 / LR; Rs can be R15 (RESET). Lower 4 bits must be zero, *trap* if not |
| `010` | Indirect jump | `OP·Rs[4]·010·0000` | JMPR Rs — jump to Rs; Rs can be R15 (RESET). Lower 4 bits must be zero, *trap* if not |
| `011` | Set on condition | `OP·Rd[4]·011·Cond[4]` | SETCC Rd, Cond — Rd ← 1 if flags satisfy Cond, else 0; Cond reuses branch condition encoding |
| `100` | Variable shift left | `OP·Rd[4]·100·Rs[4]` | SHL.V Rd, Rs — shift Rd left by Rs *(ext)* |
| `101` | Variable shift right | `OP·Rd[4]·101·Rs[4]` | SHR.V Rd, Rs — logical shift Rd right by Rs *(ext)* |
| `110` | Variable shift arith | `OP·Rd[4]·110·Rs[4]` | ASR.V Rd, Rs — arithmetic shift Rd right by Rs *(ext)* |
| `111` | — | — | reserved |

**Notes:**
- SETCC does not modify flags. Writing to R15 (zero sink) makes SETCC a no-op, consistent with the rest of the ISA.
- Variable shifts (SHL.V, SHR.V, ASR.V) are marked *(ext)* — reserved for a future extension and not implemented in V0/V1.




---
## compiler-rt runtime library interface

The following operations have no hardware instruction and are lowered to
`compiler-rt` library calls by LLVM automatically:

| C operation | Library call | Status |
|-------------|-------------|--------|
| `a / b` signed 16-bit | `__divhi3` | Software; hardware slot reserved |
| `a / b` unsigned 16-bit | `__udivhi3` | Software; hardware slot reserved |
| `a % b` signed | `__modhi3` | Software |
| `a % b` unsigned | `__umodhi3` | Software |
| 32-bit signed divide | `__divsi3` | Software |
| 32-bit unsigned divide | `__udivsi3` | Software |
| Soft-float add | `__addsf3` | Software; uses CLZ/CTZ internally |
| Soft-float multiply | `__mulsf3` | Software; uses CLZ/CTZ internally |
| `__builtin_clz` | `__clzsi2` | **Hardware CLZ — maps directly** |
| `__builtin_ctz` | `__ctzsi2` | **Hardware CTZ — maps directly** |
| `__builtin_popcount` | `__popcountsi2` | **Hardware POPCOUNT — maps directly** |
| `__builtin_bswap16` | inline | **Hardware BSWAP — maps directly** |

The hardware CLZ, CTZ, POPCOUNT, and BSWAP instructions mean that soft-float
operations, bit-manipulation code, and cryptographic routines all execute
efficiently without pulling in slow iterative software loops.

---

*Document generated from RV16 design session. For community discussion: minicore.org*  
*Last updated: [date]*
