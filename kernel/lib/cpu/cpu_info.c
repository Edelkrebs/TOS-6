#include <cpu/cpu_info.h>
#include <cpuid.h>

CPU_info cpus_info[256];
uint64_t cpu_count = 0;
uint8_t supports_apic = 0;

void setup_flags(){
	uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;

	__get_cpuid(1, &eax, &ebx, &ecx, &edx);
}
