# RV16 Licensing Policy

> **Note:** This document describes the licensing structure for the RV16 project.  
> It is intended as a guide for users, contributors, and commercial partners.  
> It is not legal advice. Consult a qualified attorney before making licensing decisions  
> for commercial or institutional use.

---

## Overview

The RV16 project consists of two distinct components, each licensed separately to  
balance maximum openness for hobbyists and educators with sustainability for the project.

| Component | License |
|-----------|---------|
| Software (LLVM backend, simulator, MicroPython port, toolchain, documentation) | MIT License |
| RTL (FPGA Verilog/SystemVerilog source, testbenches, synthesis scripts) | CERN Open Hardware Licence Version 2 — Weakly Reciprocal (CERN-OHL-W-2.0) |

Commercial use of the RTL is available under a separate Commercial License.  
See the [Commercial License](#commercial-license) section below.

---

## Software — MIT License

All software components of the RV16 project are released under the MIT License.  
This includes:

- LLVM/Clang compiler backend
- Assembler and linker scripts
- Browser-based and standalone ISA simulator
- MicroPython port
- Debugging tools and GDB support stubs
- Example programs, libraries, and HAL drivers
- Documentation source files

### MIT License Text

```
MIT License

Copyright (c) [year] [your name / minicore.org]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

The MIT License places no restrictions on commercial or non-commercial use.  
Anyone may use, modify, and distribute the software freely, including in  
proprietary products, with no obligation beyond attribution.

---

## RTL — CERN-OHL-W-2.0

The RV16 hardware description (RTL) is released under the  
**CERN Open Hardware Licence Version 2 — Weakly Reciprocal (CERN-OHL-W-2.0)**.

This includes:

- CPU core Verilog / SystemVerilog source
- Pipeline implementation
- Memory interface and bus logic
- PIO / peripheral RTL blocks
- FPGA constraint and synthesis scripts
- Testbenches and hardware simulation models

The full license text is available at:  
https://ohwr.org/cern_ohl_w_v2.txt

### What CERN-OHL-W-2.0 permits

- **Use** — You may use the RV16 RTL freely in personal, educational, research,  
  and non-commercial projects with no fee or prior approval required.
- **Study** — You may read, analyse, and learn from the RTL without restriction.
- **Modification** — You may modify the RTL for your own purposes.
- **Distribution** — You may distribute the original or modified RTL, provided  
  you do so under the same CERN-OHL-W-2.0 license and include all required notices.
- **Incorporation** — You may incorporate the RV16 core into a larger design  
  (for example, an FPGA project with custom peripherals) without being required  
  to release that larger design under CERN-OHL-W-2.0, provided the RV16 core  
  itself is clearly identified and its license is preserved.

### What CERN-OHL-W-2.0 requires

- **Attribution** — You must retain all copyright notices, author credits,  
  and the CERN-OHL-W-2.0 license notice in any distribution.
- **Contribution of core modifications** — If you modify the RV16 RTL itself  
  (the CPU core, pipeline, or peripheral blocks as distributed here) and distribute  
  or manufacture hardware based on those modifications, you must make those  
  modifications available under CERN-OHL-W-2.0. This ensures that improvements  
  to the core flow back to the community.
- **No additional restrictions** — You may not apply additional legal terms that  
  restrict the rights granted by this license.

### What CERN-OHL-W-2.0 does not require

- You are **not** required to open-source a larger design that merely *incorporates*  
  the RV16 core as a component. The reciprocal requirement applies only to  
  modifications of the RV16 RTL itself.
- You are **not** required to publish internal research results, thesis work,  
  or course materials that use RV16 as a platform. Use in teaching and research  
  is fully permitted and encouraged.
- You are **not** required to seek approval before using, modifying, or distributing  
  the RTL for non-commercial purposes.

### Note for universities and research institutions

CERN-OHL-W-2.0 is designed to be compatible with standard university IP policies.  
Unlike strongly reciprocal licenses (such as GPL or CERN-OHL-S), it does not  
require that an entire research project or product be open-sourced simply because  
it incorporates the RV16 core. Institutions may:

- Use RV16 in coursework, lab exercises, and student projects without restriction.
- Modify the RTL for research purposes and publish modifications at their discretion  
  (though sharing improvements with the community is strongly encouraged).
- Build and distribute derivative hardware (for example, custom FPGA boards or  
  custom RV16-based SoC designs) for educational or research purposes under the  
  same license.

If your institution has specific licensing requirements or IP policy constraints,  
please contact us at **licensing@minicore.org** to discuss options.

---

## Commercial License

Commercial use of the RV16 RTL is available under a separate Commercial License.

### Who needs a commercial license

A commercial license is required if you intend to:

- Incorporate the RV16 RTL (in original or modified form) into a product or service  
  that generates revenue, whether through direct sale, licensing, subscription,  
  or any other commercial arrangement.
- Manufacture or distribute hardware containing the RV16 core for sale.
- Offer a hosted or embedded service based on RV16 without releasing your  
  modifications to the core under CERN-OHL-W-2.0.
- Use the RV16 RTL in an ASIC or structured ASIC design intended for commercial sale.

### Who does not need a commercial license

- Hobbyists and makers using RV16 in personal projects, even on commercially  
  available FPGA development boards.
- Universities, colleges, schools, and other accredited educational institutions  
  using RV16 for teaching or academic research.
- Non-profit organisations using RV16 for non-revenue-generating purposes.
- Open source projects releasing their work under a compatible open license.
- Individuals or organisations evaluating RV16 for potential commercial use,  
  provided no commercial deployment occurs during evaluation.

### What the commercial license provides

- The right to incorporate the RV16 RTL into commercial products without the  
  obligation to release core modifications under CERN-OHL-W-2.0.
- The right to keep product-specific modifications to the RV16 core confidential.
- A defined support and maintenance relationship with the RV16 project team.
- Eligibility to sponsor specific RTL features or peripheral implementations  
  aligned with your product requirements.

### How to obtain a commercial license

Contact us at **richard@minicore.org** with a brief description of your  
intended use. Commercial license terms are negotiated on a case-by-case basis  
to reflect the scale and nature of the use.

---

## Contributor License Agreement (CLA)

To maintain the ability to offer a commercial license alongside the open source  
RTL license, the RV16 project requires all contributors to sign a  
**Contributor License Agreement (CLA)** before pull requests to the RTL  
repository are accepted.

The CLA grants the RV16 project the right to distribute your contribution  
under CERN-OHL-W-2.0 and to include it in commercially licensed versions.  
It does **not** transfer copyright ownership — you retain full copyright  
over your contributions.

Contributors retain the right to use their own contributions in any way they  
choose, including in their own projects under any license.

A CLA is **not** required for contributions to the software repositories  
(LLVM backend, simulator, MicroPython port, etc.), which are MIT licensed  
and do not require dual-licensing.

CLA signing instructions will be available at **minicore.org/contribute**  
prior to the first public RTL release.

---

## Summary table

| Use case | Software (MIT) | RTL (CERN-OHL-W-2.0) | Commercial license needed? |
|----------|---------------|----------------------|---------------------------|
| Personal hobbyist project | ✓ Free | ✓ Free | No |
| University coursework | ✓ Free | ✓ Free | No |
| Academic research | ✓ Free | ✓ Free | No |
| Open source project | ✓ Free | ✓ Free (compatible license) | No |
| Non-profit use | ✓ Free | ✓ Free | No |
| Commercial evaluation | ✓ Free | ✓ Free (evaluation only) | No |
| Commercial product (closed) | ✓ Free | Contact us | Yes |
| Commercial ASIC / SoC | ✓ Free | Contact us | Yes |
| Commercial FPGA IP core | ✓ Free | Contact us | Yes |
| Revenue-generating service | ✓ Free | Contact us | Yes |

---

## Contact

| Purpose | Contact |
|---------|---------|
| General licensing questions | licensing@minicore.org |
| Commercial license enquiries | licensing@minicore.org |
| University and institutional use | licensing@minicore.org |
| Community and contribution | minicore.org/contribute |

---

*RV16 is a project of minicore.org. This document was last updated [date].*  
*This document is provided for informational purposes only and does not constitute  
legal advice. Consult a qualified attorney for guidance specific to your situation.*