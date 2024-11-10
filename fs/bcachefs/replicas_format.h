/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _BCACHEFS_REPLICAS_FORMAT_H
#define _BCACHEFS_REPLICAS_FORMAT_H

struct bch_replicas_entry_v0 {
	__u8			data_type;
	__u8			nr_devs;
<<<<<<< HEAD
	__u8			devs[];
=======
	__u8			devs[] __counted_by(nr_devs);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
} __packed;

struct bch_sb_field_replicas_v0 {
	struct bch_sb_field	field;
	struct bch_replicas_entry_v0 entries[];
} __packed __aligned(8);

struct bch_replicas_entry_v1 {
	__u8			data_type;
	__u8			nr_devs;
	__u8			nr_required;
<<<<<<< HEAD
	__u8			devs[];
=======
	__u8			devs[] __counted_by(nr_devs);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
} __packed;

struct bch_sb_field_replicas {
	struct bch_sb_field	field;
	struct bch_replicas_entry_v1 entries[];
} __packed __aligned(8);

#define replicas_entry_bytes(_i)					\
	(offsetof(typeof(*(_i)), devs) + (_i)->nr_devs)

<<<<<<< HEAD
=======
#define replicas_entry_add_dev(e, d) ({					\
	(e)->nr_devs++;							\
	(e)->devs[(e)->nr_devs - 1] = (d);				\
})

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#endif /* _BCACHEFS_REPLICAS_FORMAT_H */
