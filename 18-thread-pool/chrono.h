#pragma once

#include <time.h>

typedef struct timespec chrono_t;

chrono_t chrono_start();

unsigned long chrono_nanos(chrono_t ref);

__attribute__((always_inline))
static inline unsigned long chrono_micros(chrono_t ref) {
	return (chrono_nanos(ref) + 500) / 1000;
}
