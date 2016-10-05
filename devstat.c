#include <devstat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// compile with devstat and kvm
// cc devstat.c -o devstat -ldevstat -lkvm && ./devstat

typedef struct {
	char		device[DEVSTAT_NAME_LEN];
	int		unit;
	uint64_t	bytes;
	uint64_t	transfers;
	uint64_t	blocks;
        long double	kb_per_transfer;
        long double	transfers_per_second;
        long double	mb_per_second;
        long double	blocks_per_second;
        long double	ms_per_transaction;
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

        stats.bytes = total_bytes;
        stats.transfers = total_transfers;
        stats.blocks = total_blocks;
        stats.kb_per_transfer =	kb_per_transfer;
        stats.transfers_per_second = transfers_per_second;
        stats.mb_per_second = mb_per_second;
        stats.blocks_per_second = blocks_per_second;
        stats.ms_per_transaction = ms_per_transaction;

        return stats;
}

int main(void) {
        int j;
        Stats s;
        j = _get_ndevs();
        for (int i = 0; i < j; ++i) {
                s = _get_stats(i);
                printf("%s%d\n", s.device, s.unit);
                printf("total bytes: %lld\n", s.bytes);
                printf("total transfers: %lld\n", s.transfers);
                printf("total blocks: %lld\n", s.blocks);

                printf("kb per transfer: %Lg\n", s.kb_per_transfer);
                printf("transfers per second: %Lg\n", s.transfers_per_second);
                printf("mb per second: %Lg\n", s.mb_per_second);
                printf("blocks per second: %Lg\n", s.blocks_per_second);
                printf("ms per transaction: %Lg\n", s.ms_per_transaction);
        }
}
