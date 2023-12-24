#ifndef FPU_H
#define FPU_H

void fpu_enable(void) {
    __asm("LDR.W R0, =0xE000ED88\n\t"
          "LDR R1, [R0]\n\t"
          "ORR R1, R1, #(0xF << 20)\n\t"
          "STR R1, [R0]");
}

#endif // FPU_H