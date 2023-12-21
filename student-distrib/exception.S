#define ASM     1

#define INTR_LINK(name, func)                               \
.globl name                                                 ;\
name:                                                       ;\
    pushal                                                  ;\
    pushfl                                                  ;\
    call func                                               ;\
    popfl                                                   ;\
    popal                                                   ;\
    iret                                                    ;\

INTR_LINK(zero_div, zero_div_func);
INTR_LINK(debug_exception, debug_exception_func);                
INTR_LINK(nmi, nmi_func);
INTR_LINK(breakpoint, breakpoint_func);
INTR_LINK(overflow, overflow_func);
INTR_LINK(bound_range, bound_range_func);
INTR_LINK(inv_opcode, inv_opcode_func);
INTR_LINK(device_na, device_na_func);
INTR_LINK(double_fault, double_fault_func);
INTR_LINK(coprocessor_segment, coprocessor_segment_func);
INTR_LINK(inv_task_state, inv_task_state_func);
INTR_LINK(segment_not_present, segment_not_present_func);
INTR_LINK(stack_seg_fault, stack_seg_fault_func);
INTR_LINK(general_protection, general_protection_func);
INTR_LINK(page_fault, page_fault_func);
INTR_LINK(floating_point, floating_point_func);
INTR_LINK(alignment_check, alignment_check_func);
INTR_LINK(machine_check, machine_check_func);
INTR_LINK(simd_floating_point, simd_floating_point_func);
