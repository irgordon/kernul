<div align="center">

<!-- Logo -->
<a href="https://kernul.org"><img src="kernul-logo.png" alt="KERNUL Logo" width="160" /></a>

**A modern operating system for people who care how things work.**

Clear. Open. Predictable.  
Build from UNIX-like principles with respect for the machine and the user.
</div>

---

<div align="center">

<!-- Tech Stack Badges -->
<img src="https://img.shields.io/badge/Language-C-blue" />
<img src="https://img.shields.io/badge/Language-Rust-orange" />
<img src="https://img.shields.io/badge/License-FOSS-brightgreen" />
<img src="https://img.shields.io/badge/Arch-x86__64-purple" />
<img src="https://img.shields.io/badge/Arch-AArch64-purple" />

</div>

---

## **Meet KERNUL.**

KERNUL is a modern, open-source operating system built for people who want **clarity instead of complexity.** Most systems are a "black box"—you use them, but you don't really know what they’re doing behind the scenes. KERNUL is different. It’s built on a single, powerful idea: **The system should never hide what it’s doing or who is in control.**

### **Intentional by Design.** 
### **Total Transparency.** 
### **Zero Bloat. All Performance.**

KERNUL is intentionally lean and easy to follow. Whether you're a curious learner, a professional creator, or a researcher, you can finally trust your system because you can actually understand it.

---

## **Why KERNUL?**

Most operating systems grow by "piling it on"—adding layer after layer of old code and messy shortcuts. KERNUL grows by **intent.** Every piece of the system earned its place.

* **You’re in the driver’s seat:** Access is only granted when you say so, never "just because."
* **No "ghost in the machine":** Actions are predictable. If you do the same thing twice, you get the same result every time.
* **Built on solid ground:** We don't hide mess behind fancy labels. The system is organized logically from the bottom up.
* **Honest answers:** If something goes wrong, KERNUL tells you exactly what happened instead of trying to "guess" a fix.

This makes KERNUL the perfect foundation for anyone who wants to learn, build, or experiment on a system that actually makes sense.

---

## **Who is it for?**

* **The Curious:** People who want to understand their computer, not negotiate with it.
* **The Creators:** Developers who value precision, clarity, and total control over their tools.
* **The Trailblazers:** Researchers exploring the future of tech without being held back by "how it’s always been done."
* **The Experts-to-be:** You don’t need to be a pro to explore KERNUL—but you’ll become one just by using it.

---

## **Current Progress**

KERNUL is currently in **very early development.** We aren't in a rush to add "features" just to check a box. Right now, our focus is on **quality over quantity.** We are perfecting the foundation, ensuring every part of the system is stable, predictable, and honest. 

Build the x86_64 bootstrap executable with Clang and LLD installed:

```sh
make kernel
make test-bootstrap
```

The output is `build/kernel/x86_64/kernul.elf`. `KERNEL_CC` and `KERNEL_LD`
can select installed tool versions. The hosted `make all` and `make test`
targets remain separate from the real architecture build.

With `xorriso`, `curl`, and `shasum` available, build the UEFI boot image:

```sh
make boot-image
make run OVMF_CODE=/path/to/uefi-code.fd OVMF_VARS=/path/to/uefi-vars-template.fd
```

The first image build downloads and checksum-verifies pinned Limine 12.9.3.
`make run` uses QEMU and matching UEFI code/variable firmware files; it copies
the variable template into `build/` before use. The image is
`build/kernel/x86_64/kernul.iso`. Startup installs the kernel-owned stack and
early descriptor tables, copies and validates Limine's memory map into
kernel-owned storage, then halts before `kernel_main()`. The adapter preserves
the 32-region boot contract and reserves the loaded image and bootloader,
firmware, and device memory. Invalid or oversized maps halt without activating
the PMM. `make run` prints guest diagnostics on the host terminal through COM1
(115200 baud, 8N1), including `kernul: boot entry (x86_64)` and
`kernul: handoff valid; stop before kernel_main`. Protocol/handoff failures and
early CPU exceptions print a fatal reason before halting. Serial output uses
bounded polling and is best effort. Common bootstrap composition is next;
this stage does not launch userspace.

---

## **Open by Design**

KERNUL is free and open-source. 

The entire project—the design, the code, and the "why" behind every decision—is out in the open. It’s built to be studied, improved, and owned by anyone who cares about how technology should work.

---
<div align="center">

**KERNUL**  
*A system that respects the machine—and the person using it.*

</div>
