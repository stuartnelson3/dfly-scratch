#include <devstat.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// compile with devstat and kvm
// cc devstat.c -o devstat -ldevstat -lkvm && ./devstat

typedef struct {
	uint64_t	read;
	uint64_t	write;
	uint64_t	free;
} Bytes;

typedef struct {
	uint64_t	other;
	uint64_t	read;
	uint64_t	write;
	uint64_t	free;
} Transfers;

typedef struct {
	long 	other;
	long 	read;
	long 	write;
	long 	free;
} Duration;

typedef struct {
	char		device[DEVSTAT_NAME_LEN];
	int		unit;
	Bytes		bytes;
	Transfers	transfers;
	Duration	duration;
	long		busyTime;
	uint64_t	blocks;
} Stats;

int _get_ndevs() {
	struct statinfo current;
	int num_devices;

	current.dinfo = (struct devinfo *)calloc(1, sizeof(struct devinfo));
	if (current.dinfo == NULL)
		return -2;

	checkversion();

	if (getdevs(&current) == -1)
		return -1;

	return current.dinfo->numdevs;
}

Stats _get_stats(int i) {
	struct statinfo current;
	int num_devices;

	current.dinfo = (struct devinfo *)calloc(1, sizeof(struct devinfo));
	getdevs(&current);

	num_devices = current.dinfo->numdevs;
	Stats stats;
	uint64_t bytes_read, bytes_write, bytes_free;
	uint64_t transfers_other, transfers_read, transfers_write, transfers_free;
	uint64_t blocks;

	u_int64_t total_bytes, total_transfers, total_blocks;
	long double kb_per_transfer, transfers_per_second, mb_per_second, blocks_per_second, ms_per_transaction;

	strcpy(stats.device, current.dinfo->devices[i].device_name);
	stats.unit = current.dinfo->devices[i].unit_number;
	compute_stats(&current.dinfo->devices[i],
		NULL,
		1.0,
		&total_bytes,
		&total_transfers,
		&total_blocks,
		&kb_per_transfer,
		&transfers_per_second,
		&mb_per_second,
		&blocks_per_second,
		&ms_per_transaction);

	stats.bytes.read = total_bytes;
	stats.bytes.write = total_transfers;
	stats.bytes.free = total_blocks;
	/* stats.duration.other = kb_per_transfer; */
	/* stats.duration.read = transfers_per_second; */
	/* stats.duration.write = mb_per_second; */
	/* stats.duration.free = ms_per_transaction; */
	/* stats.blocks = total_blocks; */

	return stats;
}

int main(void) {
        int j;
        Stats s;
        j = _get_ndevs();
        for (int i = 0; i < j; ++i) {
                s = _get_stats(i);
                printf("%s%d\n", s.device, s.unit);
                printf("total bytes: %lld\n", s.bytes.read);
                printf("total transfers: %lld\n", s.bytes.write);
                printf("total blocks: %lld\n", s.bytes.free);

        }
}
