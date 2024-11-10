/* SPDX-License-Identifier: GPL-2.0 */
/*
 * nfsd-specific authentication stuff.
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef LINUX_NFSD_AUTH_H
#define LINUX_NFSD_AUTH_H

/*
 * Set the current process's fsuid/fsgid etc to those of the NFS
 * client user
 */
<<<<<<< HEAD
int nfsd_setuser(struct svc_rqst *, struct svc_export *);
=======
int nfsd_setuser(struct svc_cred *cred, struct svc_export *exp);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

#endif /* LINUX_NFSD_AUTH_H */
