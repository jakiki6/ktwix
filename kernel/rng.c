#include "arch/arch.h"

int get_rng() {
	return arch_rdtsc();
}
