/*
 * cp_time.c
 * returns user,nice,system,intr and idle values from cputime counters
 * similar to some BSDs kern.cp_time sysctl
 */

#include <stdio.h>
#include <kinfo.h>
#include <sys/sysctl.h>

int main( void ) {
  int cpu, ncpu, mib[2];
  struct kinfo_cputime cp_t[ncpu];
  uint64_t user, nice, sys, intr, idle;

  size_t len;

  mib[0] = CTL_HW;
  mib[1] = HW_NCPU;
  len = sizeof(ncpu);
  sysctl(mib, 2, &ncpu, &len, NULL, 0);

  bzero(cp_t, sizeof(struct kinfo_cputime)*ncpu);

  len = sizeof(cp_t[0])*ncpu;
  if (sysctlbyname("kern.cputime", &cp_t, &len, NULL, 0))
			err(1, "kern.cputime");

  struct clockinfo clockrate;
  size_t clockrate_size = sizeof(clockrate);
  if (sysctlbyname("kern.clockrate", &clockrate, &clockrate_size, NULL, 0) == -1 ||
                  sizeof(clockrate) != clockrate_size) {
          return -1;
  }

  user = nice = sys = intr = idle = 0;
  // Sum up cp_time for all cpus.
  for (cpu = 0; cpu < ncpu; cpu++) {
    user = cp_t[cpu].cp_user/clockrate.stathz;
    nice = cp_t[cpu].cp_nice/clockrate.stathz;
    sys  = cp_t[cpu].cp_sys/clockrate.stathz;
    intr = cp_t[cpu].cp_intr/clockrate.stathz;
    idle = cp_t[cpu].cp_idle/clockrate.stathz;

    printf("cpu%d: %llu %llu %llu %llu %llu\n", cpu, user, nice, sys, intr, idle );
  }


  return 0;
}
