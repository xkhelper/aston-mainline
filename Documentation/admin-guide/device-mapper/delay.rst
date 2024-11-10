========
dm-delay
========

Device-Mapper's "delay" target delays reads and/or writes
<<<<<<< HEAD
and maps them to different devices.

Parameters::
=======
and/or flushs and optionally maps them to different devices.

Arguments::
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

    <device> <offset> <delay> [<write_device> <write_offset> <write_delay>
			       [<flush_device> <flush_offset> <flush_delay>]]

<<<<<<< HEAD
With separate write parameters, the first set is only used for reads.
Offsets are specified in sectors.
Delays are specified in milliseconds.

=======
Table line has to either have 3, 6 or 9 arguments:

3: apply offset and delay to read, write and flush operations on device

6: apply offset and delay to device, also apply write_offset and write_delay
   to write and flush operations on optionally different write_device with
   optionally different sector offset

9: same as 6 arguments plus define flush_offset and flush_delay explicitely
   on/with optionally different flush_device/flush_offset.

Offsets are specified in sectors.

Delays are specified in milliseconds.


>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
Example scripts
===============

::
<<<<<<< HEAD

	#!/bin/sh
	# Create device delaying rw operation for 500ms
	echo "0 `blockdev --getsz $1` delay $1 0 500" | dmsetup create delayed

::

	#!/bin/sh
	# Create device delaying only write operation for 500ms and
	# splitting reads and writes to different devices $1 $2
	echo "0 `blockdev --getsz $1` delay $1 0 0 $2 0 500" | dmsetup create delayed
=======
	#!/bin/sh
	#
	# Create mapped device named "delayed" delaying read, write and flush operations for 500ms.
	#
	dmsetup create delayed --table  "0 `blockdev --getsz $1` delay $1 0 500"

::
	#!/bin/sh
	#
	# Create mapped device delaying write and flush operations for 400ms and
	# splitting reads to device $1 but writes and flushs to different device $2
	# to different offsets of 2048 and 4096 sectors respectively.
	#
	dmsetup create delayed --table "0 `blockdev --getsz $1` delay $1 2048 0 $2 4096 400"

::
	#!/bin/sh
	#
	# Create mapped device delaying reads for 50ms, writes for 100ms and flushs for 333ms
	# onto the same backing device at offset 0 sectors.
	#
	dmsetup create delayed --table "0 `blockdev --getsz $1` delay $1 0 50 $2 0 100 $1 0 333"
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
