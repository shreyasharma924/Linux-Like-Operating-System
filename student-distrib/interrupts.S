#define ASM     1

#define INTR_ASM_LINK(name, func)                           \
.globl name                                                 ;\
name:                                                       ;\
    pushal                                                  ;\
    pushfl                                                  ;\
    call func                                               ;\
    popfl                                                   ;\
    popal                                                   ;\
    iret                                                    ;\

INTR_ASM_LINK(keyboard, keyboard_handler);
INTR_ASM_LINK(rtc, rtc_handler);
INTR_ASM_LINK(PIT, PIT_handler);

