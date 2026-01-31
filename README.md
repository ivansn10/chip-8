# CHIP-8 Virtual Machine & Interpreter 👾

A high-fidelity emulation of the CHIP-8 interpreted programming language. This project explores low-level system architecture, memory management, and opcode interpretation using **C# / .NET**.

---

### 💻 Technical Specifications
- **CPU Emulation:** Accurate implementation of the 35 original opcodes, including arithmetic, logical operations, and flow control.
- **Memory Mapping:** 4KB of interpreted RAM with dedicated space for font sets and program data.
- **Graphics & Input:** 64x32 monochrome display buffer and 16-key hex keypad mapping.
- **Timers:** Independent implementation of Delay and Sound timers at 60Hz.

---

### 🚀 Engineering Challenges
- **Bitwise Operations:** Intensive use of bit masking and shifting for instruction decoding.
- **Instruction Cycle:** Implementation of a fetch-decode-execute cycle with precise timing.
- **Modular Design:** Decoupled the CPU logic from the rendering engine, allowing for multiple frontend implementations (Console, SDL, or MonoGame).

---

### 🛠️ How to Run
```bash
dotnet run --rom path/to/pong.ch8
