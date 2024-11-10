// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2018 Facebook

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

<<<<<<< HEAD
struct {
	__uint(type, BPF_MAP_TYPE_ARRAY);
	__uint(max_entries, 1);
	__type(key, __u32);
	__type(value, __u64);
} cg_ids SEC(".maps");

struct {
	__uint(type, BPF_MAP_TYPE_ARRAY);
	__uint(max_entries, 1);
	__type(key, __u32);
	__type(value, __u32);
} pidmap SEC(".maps");
=======
__u64 cg_id;
__u64 expected_pid;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

SEC("tracepoint/syscalls/sys_enter_nanosleep")
int trace(void *ctx)
{
	__u32 pid = bpf_get_current_pid_tgid();
<<<<<<< HEAD
	__u32 key = 0, *expected_pid;
	__u64 *val;

	expected_pid = bpf_map_lookup_elem(&pidmap, &key);
	if (!expected_pid || *expected_pid != pid)
		return 0;

	val = bpf_map_lookup_elem(&cg_ids, &key);
	if (val)
		*val = bpf_get_current_cgroup_id();
=======

	if (expected_pid == pid)
		cg_id = bpf_get_current_cgroup_id();
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	return 0;
}

char _license[] SEC("license") = "GPL";
