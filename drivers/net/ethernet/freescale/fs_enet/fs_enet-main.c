<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
/*
 * Combined Ethernet driver for Motorola MPC8xx and MPC82xx.
 *
 * Copyright (c) 2003 Intracom S.A.
 *  by Pantelis Antoniou <panto@intracom.gr>
 *
 * 2005 (c) MontaVista Software, Inc.
 * Vitaly Bordug <vbordug@ru.mvista.com>
 *
 * Heavily based on original FEC driver by Dan Malek <dan@embeddededge.com>
 * and modifications by Joakim Tjernlund <joakim.tjernlund@lumentis.se>
<<<<<<< HEAD
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ptrace.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
<<<<<<< HEAD
#include <linux/mii.h>
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#include <linux/ethtool.h>
#include <linux/bitops.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/phy.h>
<<<<<<< HEAD
=======
#include <linux/phylink.h>
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#include <linux/property.h>
#include <linux/of.h>
#include <linux/of_mdio.h>
#include <linux/of_net.h>
#include <linux/pgtable.h>
<<<<<<< HEAD
=======
#include <linux/rtnetlink.h>
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

#include <linux/vmalloc.h>
#include <asm/irq.h>
#include <linux/uaccess.h>

#include "fs_enet.h"

/*************************************************/

MODULE_AUTHOR("Pantelis Antoniou <panto@intracom.gr>");
MODULE_DESCRIPTION("Freescale Ethernet Driver");
MODULE_LICENSE("GPL");

static int fs_enet_debug = -1; /* -1 == use FS_ENET_DEF_MSG_ENABLE as value */
module_param(fs_enet_debug, int, 0);
MODULE_PARM_DESC(fs_enet_debug,
		 "Freescale bitmapped debugging message enable value");

#define RX_RING_SIZE	32
#define TX_RING_SIZE	64

#ifdef CONFIG_NET_POLL_CONTROLLER
static void fs_enet_netpoll(struct net_device *dev);
#endif

static void fs_set_multicast_list(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	(*fep->ops->set_multicast_list)(dev);
}

<<<<<<< HEAD
=======
static int fs_eth_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	return phylink_mii_ioctl(fep->phylink, ifr, cmd);
}

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static void skb_align(struct sk_buff *skb, int align)
{
	int off = ((unsigned long)skb->data) & (align - 1);

	if (off)
		skb_reserve(skb, align - off);
}

/* NAPI function */
static int fs_enet_napi(struct napi_struct *napi, int budget)
{
	struct fs_enet_private *fep = container_of(napi, struct fs_enet_private, napi);
<<<<<<< HEAD
	struct net_device *dev = fep->ndev;
	const struct fs_platform_info *fpi = fep->fpi;
	cbd_t __iomem *bdp;
	struct sk_buff *skb, *skbn;
	int received = 0;
	u16 pkt_len, sc;
	int curidx;
	int dirtyidx, do_wake, do_restart;
	int tx_left = TX_RING_SIZE;
=======
	const struct fs_platform_info *fpi = fep->fpi;
	struct net_device *dev = fep->ndev;
	int curidx, dirtyidx, received = 0;
	int do_wake = 0, do_restart = 0;
	int tx_left = TX_RING_SIZE;
	struct sk_buff *skb, *skbn;
	cbd_t __iomem *bdp;
	u16 pkt_len, sc;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	spin_lock(&fep->tx_lock);
	bdp = fep->dirty_tx;

	/* clear status bits for napi*/
	(*fep->ops->napi_clear_event)(dev);

<<<<<<< HEAD
	do_wake = do_restart = 0;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	while (((sc = CBDR_SC(bdp)) & BD_ENET_TX_READY) == 0 && tx_left) {
		dirtyidx = bdp - fep->tx_bd_base;

		if (fep->tx_free == fep->tx_ring)
			break;

		skb = fep->tx_skbuff[dirtyidx];

<<<<<<< HEAD
		/*
		 * Check for errors.
		 */
		if (sc & (BD_ENET_TX_HB | BD_ENET_TX_LC |
			  BD_ENET_TX_RL | BD_ENET_TX_UN | BD_ENET_TX_CSL)) {

=======
		 /* Check for errors. */
		if (sc & (BD_ENET_TX_HB | BD_ENET_TX_LC |
			  BD_ENET_TX_RL | BD_ENET_TX_UN | BD_ENET_TX_CSL)) {
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			if (sc & BD_ENET_TX_HB)	/* No heartbeat */
				dev->stats.tx_heartbeat_errors++;
			if (sc & BD_ENET_TX_LC)	/* Late collision */
				dev->stats.tx_window_errors++;
			if (sc & BD_ENET_TX_RL)	/* Retrans limit */
				dev->stats.tx_aborted_errors++;
			if (sc & BD_ENET_TX_UN)	/* Underrun */
				dev->stats.tx_fifo_errors++;
			if (sc & BD_ENET_TX_CSL)	/* Carrier lost */
				dev->stats.tx_carrier_errors++;

			if (sc & (BD_ENET_TX_LC | BD_ENET_TX_RL | BD_ENET_TX_UN)) {
				dev->stats.tx_errors++;
				do_restart = 1;
			}
<<<<<<< HEAD
		} else
			dev->stats.tx_packets++;
=======
		} else {
			dev->stats.tx_packets++;
		}
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

		if (sc & BD_ENET_TX_READY) {
			dev_warn(fep->dev,
				 "HEY! Enet xmit interrupt and TX_READY.\n");
		}

<<<<<<< HEAD
		/*
		 * Deferred means some collisions occurred during transmit,
=======
		/* Deferred means some collisions occurred during transmit,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		 * but we eventually sent the packet OK.
		 */
		if (sc & BD_ENET_TX_DEF)
			dev->stats.collisions++;

		/* unmap */
		if (fep->mapped_as_page[dirtyidx])
			dma_unmap_page(fep->dev, CBDR_BUFADDR(bdp),
				       CBDR_DATLEN(bdp), DMA_TO_DEVICE);
		else
			dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
					 CBDR_DATLEN(bdp), DMA_TO_DEVICE);

<<<<<<< HEAD
		/*
		 * Free the sk buffer associated with this last transmit.
		 */
=======
		/* Free the sk buffer associated with this last transmit. */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (skb) {
			dev_kfree_skb(skb);
			fep->tx_skbuff[dirtyidx] = NULL;
		}

<<<<<<< HEAD
		/*
		 * Update pointer to next buffer descriptor to be transmitted.
=======
		/* Update pointer to next buffer descriptor to be transmitted.
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		 */
		if ((sc & BD_ENET_TX_WRAP) == 0)
			bdp++;
		else
			bdp = fep->tx_bd_base;

<<<<<<< HEAD
		/*
		 * Since we have freed up a buffer, the ring is no longer
		 * full.
=======
		/* Since we have freed up a buffer, the ring is no longer full.
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		 */
		if (++fep->tx_free == MAX_SKB_FRAGS)
			do_wake = 1;
		tx_left--;
	}

	fep->dirty_tx = bdp;

	if (do_restart)
		(*fep->ops->tx_restart)(dev);

	spin_unlock(&fep->tx_lock);

	if (do_wake)
		netif_wake_queue(dev);

<<<<<<< HEAD
	/*
	 * First, grab all of the stats for the incoming packet.
=======
	/* First, grab all of the stats for the incoming packet.
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	 * These get messed up if we get called due to a busy condition.
	 */
	bdp = fep->cur_rx;

	while (((sc = CBDR_SC(bdp)) & BD_ENET_RX_EMPTY) == 0 &&
	       received < budget) {
		curidx = bdp - fep->rx_bd_base;

<<<<<<< HEAD
		/*
		 * Since we have allocated space to hold a complete frame,
=======
		/* Since we have allocated space to hold a complete frame,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		 * the last indicator should be set.
		 */
		if ((sc & BD_ENET_RX_LAST) == 0)
			dev_warn(fep->dev, "rcv is not +last\n");

<<<<<<< HEAD
		/*
		 * Check for errors.
		 */
=======
		/* Check for errors. */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (sc & (BD_ENET_RX_LG | BD_ENET_RX_SH | BD_ENET_RX_CL |
			  BD_ENET_RX_NO | BD_ENET_RX_CR | BD_ENET_RX_OV)) {
			dev->stats.rx_errors++;
			/* Frame too long or too short. */
			if (sc & (BD_ENET_RX_LG | BD_ENET_RX_SH))
				dev->stats.rx_length_errors++;
			/* Frame alignment */
			if (sc & (BD_ENET_RX_NO | BD_ENET_RX_CL))
				dev->stats.rx_frame_errors++;
			/* CRC Error */
			if (sc & BD_ENET_RX_CR)
				dev->stats.rx_crc_errors++;
			/* FIFO overrun */
			if (sc & BD_ENET_RX_OV)
				dev->stats.rx_crc_errors++;

			skbn = fep->rx_skbuff[curidx];
		} else {
			skb = fep->rx_skbuff[curidx];

<<<<<<< HEAD
			/*
			 * Process the incoming frame.
			 */
=======
			/* Process the incoming frame */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			dev->stats.rx_packets++;
			pkt_len = CBDR_DATLEN(bdp) - 4;	/* remove CRC */
			dev->stats.rx_bytes += pkt_len + 4;

			if (pkt_len <= fpi->rx_copybreak) {
				/* +2 to make IP header L1 cache aligned */
				skbn = netdev_alloc_skb(dev, pkt_len + 2);
<<<<<<< HEAD
				if (skbn != NULL) {
					skb_reserve(skbn, 2);	/* align IP header */
					skb_copy_from_linear_data(skb,
						      skbn->data, pkt_len);
					swap(skb, skbn);
					dma_sync_single_for_cpu(fep->dev,
						CBDR_BUFADDR(bdp),
						L1_CACHE_ALIGN(pkt_len),
						DMA_FROM_DEVICE);
=======
				if (skbn) {
					skb_reserve(skbn, 2);	/* align IP header */
					skb_copy_from_linear_data(skb, skbn->data,
								  pkt_len);
					swap(skb, skbn);
					dma_sync_single_for_cpu(fep->dev,
								CBDR_BUFADDR(bdp),
								L1_CACHE_ALIGN(pkt_len),
								DMA_FROM_DEVICE);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
				}
			} else {
				skbn = netdev_alloc_skb(dev, ENET_RX_FRSIZE);

				if (skbn) {
					dma_addr_t dma;

					skb_align(skbn, ENET_RX_ALIGN);

<<<<<<< HEAD
					dma_unmap_single(fep->dev,
						CBDR_BUFADDR(bdp),
						L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
						DMA_FROM_DEVICE);

					dma = dma_map_single(fep->dev,
						skbn->data,
						L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
						DMA_FROM_DEVICE);
=======
					dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
							 L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
							 DMA_FROM_DEVICE);

					dma = dma_map_single(fep->dev, skbn->data,
							     L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
							     DMA_FROM_DEVICE);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
					CBDW_BUFADDR(bdp, dma);
				}
			}

<<<<<<< HEAD
			if (skbn != NULL) {
=======
			if (skbn) {
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
				skb_put(skb, pkt_len);	/* Make room */
				skb->protocol = eth_type_trans(skb, dev);
				received++;
				netif_receive_skb(skb);
			} else {
				dev->stats.rx_dropped++;
				skbn = skb;
			}
		}

		fep->rx_skbuff[curidx] = skbn;
		CBDW_DATLEN(bdp, 0);
		CBDW_SC(bdp, (sc & ~BD_ENET_RX_STATS) | BD_ENET_RX_EMPTY);

<<<<<<< HEAD
		/*
		 * Update BD pointer to next entry.
		 */
=======
		/* Update BD pointer to next entry */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if ((sc & BD_ENET_RX_WRAP) == 0)
			bdp++;
		else
			bdp = fep->rx_bd_base;

		(*fep->ops->rx_bd_done)(dev);
	}

	fep->cur_rx = bdp;

	if (received < budget && tx_left) {
		/* done */
		napi_complete_done(napi, received);
		(*fep->ops->napi_enable)(dev);

		return received;
	}

	return budget;
}

<<<<<<< HEAD
/*
 * The interrupt handler.
=======
/* The interrupt handler.
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
 * This is called from the MPC core interrupt.
 */
static irqreturn_t
fs_enet_interrupt(int irq, void *dev_id)
{
	struct net_device *dev = dev_id;
<<<<<<< HEAD
	struct fs_enet_private *fep;
	u32 int_events;
	u32 int_clr_events;
	int nr, napi_ok;
	int handled;
=======
	u32 int_events, int_clr_events;
	struct fs_enet_private *fep;
	int nr, napi_ok, handled;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	fep = netdev_priv(dev);

	nr = 0;
	while ((int_events = (*fep->ops->get_int_events)(dev)) != 0) {
		nr++;

		int_clr_events = int_events;
		int_clr_events &= ~fep->ev_napi;

		(*fep->ops->clear_int_events)(dev, int_clr_events);

		if (int_events & fep->ev_err)
			(*fep->ops->ev_error)(dev, int_events);

		if (int_events & fep->ev) {
			napi_ok = napi_schedule_prep(&fep->napi);

			(*fep->ops->napi_disable)(dev);
			(*fep->ops->clear_int_events)(dev, fep->ev_napi);

<<<<<<< HEAD
			/* NOTE: it is possible for FCCs in NAPI mode    */
			/* to submit a spurious interrupt while in poll  */
			if (napi_ok)
				__napi_schedule(&fep->napi);
		}

=======
			/* NOTE: it is possible for FCCs in NAPI mode
			 * to submit a spurious interrupt while in poll
			 */
			if (napi_ok)
				__napi_schedule(&fep->napi);
		}
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}

	handled = nr > 0;
	return IRQ_RETVAL(handled);
}

void fs_init_bds(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
<<<<<<< HEAD
	cbd_t __iomem *bdp;
	struct sk_buff *skb;
=======
	struct sk_buff *skb;
	cbd_t __iomem *bdp;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	int i;

	fs_cleanup_bds(dev);

<<<<<<< HEAD
	fep->dirty_tx = fep->cur_tx = fep->tx_bd_base;
	fep->tx_free = fep->tx_ring;
	fep->cur_rx = fep->rx_bd_base;

	/*
	 * Initialize the receive buffer descriptors.
	 */
	for (i = 0, bdp = fep->rx_bd_base; i < fep->rx_ring; i++, bdp++) {
		skb = netdev_alloc_skb(dev, ENET_RX_FRSIZE);
		if (skb == NULL)
=======
	fep->dirty_tx = fep->tx_bd_base;
	fep->cur_tx = fep->tx_bd_base;
	fep->tx_free = fep->tx_ring;
	fep->cur_rx = fep->rx_bd_base;

	/* Initialize the receive buffer descriptors */
	for (i = 0, bdp = fep->rx_bd_base; i < fep->rx_ring; i++, bdp++) {
		skb = netdev_alloc_skb(dev, ENET_RX_FRSIZE);
		if (!skb)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			break;

		skb_align(skb, ENET_RX_ALIGN);
		fep->rx_skbuff[i] = skb;
<<<<<<< HEAD
		CBDW_BUFADDR(bdp,
			dma_map_single(fep->dev, skb->data,
				L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
				DMA_FROM_DEVICE));
=======
		CBDW_BUFADDR(bdp, dma_map_single(fep->dev, skb->data,
						 L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
						 DMA_FROM_DEVICE));
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		CBDW_DATLEN(bdp, 0);	/* zero */
		CBDW_SC(bdp, BD_ENET_RX_EMPTY |
			((i < fep->rx_ring - 1) ? 0 : BD_SC_WRAP));
	}
<<<<<<< HEAD
	/*
	 * if we failed, fillup remainder
	 */
=======

	/* if we failed, fillup remainder */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	for (; i < fep->rx_ring; i++, bdp++) {
		fep->rx_skbuff[i] = NULL;
		CBDW_SC(bdp, (i < fep->rx_ring - 1) ? 0 : BD_SC_WRAP);
	}

<<<<<<< HEAD
	/*
	 * ...and the same for transmit.
	 */
=======
	/* ...and the same for transmit. */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	for (i = 0, bdp = fep->tx_bd_base; i < fep->tx_ring; i++, bdp++) {
		fep->tx_skbuff[i] = NULL;
		CBDW_BUFADDR(bdp, 0);
		CBDW_DATLEN(bdp, 0);
		CBDW_SC(bdp, (i < fep->tx_ring - 1) ? 0 : BD_SC_WRAP);
	}
}

void fs_cleanup_bds(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	struct sk_buff *skb;
	cbd_t __iomem *bdp;
	int i;

<<<<<<< HEAD
	/*
	 * Reset SKB transmit buffers.
	 */
	for (i = 0, bdp = fep->tx_bd_base; i < fep->tx_ring; i++, bdp++) {
		if ((skb = fep->tx_skbuff[i]) == NULL)
=======
	/* Reset SKB transmit buffers. */
	for (i = 0, bdp = fep->tx_bd_base; i < fep->tx_ring; i++, bdp++) {
		skb = fep->tx_skbuff[i];
		if (!skb)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			continue;

		/* unmap */
		dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
<<<<<<< HEAD
				skb->len, DMA_TO_DEVICE);
=======
				 skb->len, DMA_TO_DEVICE);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

		fep->tx_skbuff[i] = NULL;
		dev_kfree_skb(skb);
	}

<<<<<<< HEAD
	/*
	 * Reset SKB receive buffers
	 */
	for (i = 0, bdp = fep->rx_bd_base; i < fep->rx_ring; i++, bdp++) {
		if ((skb = fep->rx_skbuff[i]) == NULL)
=======
	/* Reset SKB receive buffers */
	for (i = 0, bdp = fep->rx_bd_base; i < fep->rx_ring; i++, bdp++) {
		skb = fep->rx_skbuff[i];
		if (!skb)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			continue;

		/* unmap */
		dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
<<<<<<< HEAD
			L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
			DMA_FROM_DEVICE);
=======
				 L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
				 DMA_FROM_DEVICE);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

		fep->rx_skbuff[i] = NULL;

		dev_kfree_skb(skb);
	}
}

<<<<<<< HEAD
/**********************************************************************************/

#ifdef CONFIG_FS_ENET_MPC5121_FEC
/*
 * MPC5121 FEC requeries 4-byte alignment for TX data buffer!
 */
=======
#ifdef CONFIG_FS_ENET_MPC5121_FEC
/* MPC5121 FEC requires 4-byte alignment for TX data buffer! */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static struct sk_buff *tx_skb_align_workaround(struct net_device *dev,
					       struct sk_buff *skb)
{
	struct sk_buff *new_skb;

	if (skb_linearize(skb))
		return NULL;

	/* Alloc new skb */
	new_skb = netdev_alloc_skb(dev, skb->len + 4);
	if (!new_skb)
		return NULL;

	/* Make sure new skb is properly aligned */
	skb_align(new_skb, 4);

	/* Copy data to new skb ... */
	skb_copy_from_linear_data(skb, new_skb->data, skb->len);
	skb_put(new_skb, skb->len);

	/* ... and free an old one */
	dev_kfree_skb_any(skb);

	return new_skb;
}
#endif

static netdev_tx_t
fs_enet_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
<<<<<<< HEAD
	cbd_t __iomem *bdp;
	int curidx;
	u16 sc;
	int nr_frags;
	skb_frag_t *frag;
	int len;
#ifdef CONFIG_FS_ENET_MPC5121_FEC
	int is_aligned = 1;
	int i;
=======
	int curidx, nr_frags, len;
	cbd_t __iomem *bdp;
	skb_frag_t *frag;
	u16 sc;
#ifdef CONFIG_FS_ENET_MPC5121_FEC
	int i, is_aligned = 1;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	if (!IS_ALIGNED((unsigned long)skb->data, 4)) {
		is_aligned = 0;
	} else {
		nr_frags = skb_shinfo(skb)->nr_frags;
		frag = skb_shinfo(skb)->frags;
		for (i = 0; i < nr_frags; i++, frag++) {
			if (!IS_ALIGNED(skb_frag_off(frag), 4)) {
				is_aligned = 0;
				break;
			}
		}
	}

	if (!is_aligned) {
		skb = tx_skb_align_workaround(dev, skb);
		if (!skb) {
<<<<<<< HEAD
			/*
			 * We have lost packet due to memory allocation error
=======
			/* We have lost packet due to memory allocation error
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			 * in tx_skb_align_workaround(). Hopefully original
			 * skb is still valid, so try transmit it later.
			 */
			return NETDEV_TX_BUSY;
		}
	}
#endif

	spin_lock(&fep->tx_lock);

<<<<<<< HEAD
	/*
	 * Fill in a Tx ring entry
	 */
=======
	/* Fill in a Tx ring entry */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	bdp = fep->cur_tx;

	nr_frags = skb_shinfo(skb)->nr_frags;
	if (fep->tx_free <= nr_frags || (CBDR_SC(bdp) & BD_ENET_TX_READY)) {
		netif_stop_queue(dev);
		spin_unlock(&fep->tx_lock);

<<<<<<< HEAD
		/*
		 * Ooops.  All transmit buffers are full.  Bail out.
=======
		/* Ooops.  All transmit buffers are full.  Bail out.
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		 * This should not happen, since the tx queue should be stopped.
		 */
		dev_warn(fep->dev, "tx queue full!.\n");
		return NETDEV_TX_BUSY;
	}

	curidx = bdp - fep->tx_bd_base;

	len = skb->len;
	dev->stats.tx_bytes += len;
	if (nr_frags)
		len -= skb->data_len;
<<<<<<< HEAD
	fep->tx_free -= nr_frags + 1;
	/*
	 * Push the data cache so the CPM does not get stale memory data.
	 */
	CBDW_BUFADDR(bdp, dma_map_single(fep->dev,
				skb->data, len, DMA_TO_DEVICE));
=======

	fep->tx_free -= nr_frags + 1;
	/* Push the data cache so the CPM does not get stale memory data.
	 */
	CBDW_BUFADDR(bdp, dma_map_single(fep->dev,
					 skb->data, len, DMA_TO_DEVICE));
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	CBDW_DATLEN(bdp, len);

	fep->mapped_as_page[curidx] = 0;
	frag = skb_shinfo(skb)->frags;
	while (nr_frags) {
		CBDC_SC(bdp,
			BD_ENET_TX_STATS | BD_ENET_TX_INTR | BD_ENET_TX_LAST |
			BD_ENET_TX_TC);
		CBDS_SC(bdp, BD_ENET_TX_READY);

		if ((CBDR_SC(bdp) & BD_ENET_TX_WRAP) == 0) {
			bdp++;
			curidx++;
		} else {
			bdp = fep->tx_bd_base;
			curidx = 0;
		}

		len = skb_frag_size(frag);
		CBDW_BUFADDR(bdp, skb_frag_dma_map(fep->dev, frag, 0, len,
						   DMA_TO_DEVICE));
		CBDW_DATLEN(bdp, len);

		fep->tx_skbuff[curidx] = NULL;
		fep->mapped_as_page[curidx] = 1;

		frag++;
		nr_frags--;
	}

	/* Trigger transmission start */
	sc = BD_ENET_TX_READY | BD_ENET_TX_INTR |
	     BD_ENET_TX_LAST | BD_ENET_TX_TC;

	/* note that while FEC does not have this bit
	 * it marks it as available for software use
<<<<<<< HEAD
	 * yay for hw reuse :) */
	if (skb->len <= 60)
		sc |= BD_ENET_TX_PAD;
=======
	 * yay for hw reuse :)
	 */
	if (skb->len <= 60)
		sc |= BD_ENET_TX_PAD;

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	CBDC_SC(bdp, BD_ENET_TX_STATS);
	CBDS_SC(bdp, sc);

	/* Save skb pointer. */
	fep->tx_skbuff[curidx] = skb;

	/* If this was the last BD in the ring, start at the beginning again. */
	if ((CBDR_SC(bdp) & BD_ENET_TX_WRAP) == 0)
		bdp++;
	else
		bdp = fep->tx_bd_base;
<<<<<<< HEAD
=======

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	fep->cur_tx = bdp;

	if (fep->tx_free < MAX_SKB_FRAGS)
		netif_stop_queue(dev);

	skb_tx_timestamp(skb);

	(*fep->ops->tx_kickstart)(dev);

	spin_unlock(&fep->tx_lock);

	return NETDEV_TX_OK;
}

static void fs_timeout_work(struct work_struct *work)
{
	struct fs_enet_private *fep = container_of(work, struct fs_enet_private,
						   timeout_work);
	struct net_device *dev = fep->ndev;
	unsigned long flags;
	int wake = 0;

	dev->stats.tx_errors++;

<<<<<<< HEAD
	spin_lock_irqsave(&fep->lock, flags);

	if (dev->flags & IFF_UP) {
		phy_stop(dev->phydev);
		(*fep->ops->stop)(dev);
		(*fep->ops->restart)(dev);
	}

	phy_start(dev->phydev);
=======
	/* In the event a timeout was detected, but the netdev is brought down
	 * shortly after, it no longer makes sense to try to recover from the
	 * timeout. netif_running() will return false when called from the
	 * .ndo_close() callback. Calling the following recovery code while
	 * called from .ndo_close() could deadlock on rtnl.
	 */
	if (!netif_running(dev))
		return;

	rtnl_lock();
	phylink_stop(fep->phylink);
	phylink_start(fep->phylink);
	rtnl_unlock();

	spin_lock_irqsave(&fep->lock, flags);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	wake = fep->tx_free >= MAX_SKB_FRAGS &&
	       !(CBDR_SC(fep->cur_tx) & BD_ENET_TX_READY);
	spin_unlock_irqrestore(&fep->lock, flags);

	if (wake)
		netif_wake_queue(dev);
}

static void fs_timeout(struct net_device *dev, unsigned int txqueue)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	schedule_work(&fep->timeout_work);
}

<<<<<<< HEAD
/*-----------------------------------------------------------------------------
 *  generic link-change handler - should be sufficient for most cases
 *-----------------------------------------------------------------------------*/
static void generic_adjust_link(struct  net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	struct phy_device *phydev = dev->phydev;
	int new_state = 0;

	if (phydev->link) {
		/* adjust to duplex mode */
		if (phydev->duplex != fep->oldduplex) {
			new_state = 1;
			fep->oldduplex = phydev->duplex;
		}

		if (phydev->speed != fep->oldspeed) {
			new_state = 1;
			fep->oldspeed = phydev->speed;
		}

		if (!fep->oldlink) {
			new_state = 1;
			fep->oldlink = 1;
		}

		if (new_state)
			fep->ops->restart(dev);
	} else if (fep->oldlink) {
		new_state = 1;
		fep->oldlink = 0;
		fep->oldspeed = 0;
		fep->oldduplex = -1;
	}

	if (new_state && netif_msg_link(fep))
		phy_print_status(phydev);
}


static void fs_adjust_link(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	unsigned long flags;

	spin_lock_irqsave(&fep->lock, flags);

	if(fep->ops->adjust_link)
		fep->ops->adjust_link(dev);
	else
		generic_adjust_link(dev);

	spin_unlock_irqrestore(&fep->lock, flags);
}

static int fs_init_phy(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	struct phy_device *phydev;
	phy_interface_t iface;

	fep->oldlink = 0;
	fep->oldspeed = 0;
	fep->oldduplex = -1;

	iface = fep->fpi->use_rmii ?
		PHY_INTERFACE_MODE_RMII : PHY_INTERFACE_MODE_MII;

	phydev = of_phy_connect(dev, fep->fpi->phy_node, &fs_adjust_link, 0,
				iface);
	if (!phydev) {
		dev_err(&dev->dev, "Could not attach to PHY\n");
		return -ENODEV;
	}

	return 0;
=======
static void fs_mac_link_up(struct phylink_config *config,
			   struct phy_device *phy,
			   unsigned int mode, phy_interface_t interface,
			   int speed, int duplex,
			   bool tx_pause, bool rx_pause)
{
	struct net_device *ndev = to_net_dev(config->dev);
	struct fs_enet_private *fep = netdev_priv(ndev);
	unsigned long flags;

	spin_lock_irqsave(&fep->lock, flags);
	fep->ops->restart(ndev, interface, speed, duplex);
	spin_unlock_irqrestore(&fep->lock, flags);
}

static void fs_mac_link_down(struct phylink_config *config,
			     unsigned int mode, phy_interface_t interface)
{
	struct net_device *ndev = to_net_dev(config->dev);
	struct fs_enet_private *fep = netdev_priv(ndev);
	unsigned long flags;

	spin_lock_irqsave(&fep->lock, flags);
	fep->ops->stop(ndev);
	spin_unlock_irqrestore(&fep->lock, flags);
}

static void fs_mac_config(struct phylink_config *config, unsigned int mode,
			  const struct phylink_link_state *state)
{
	/* Nothing to do */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

static int fs_enet_open(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	int r;
	int err;

<<<<<<< HEAD
	/* to initialize the fep->cur_rx,... */
	/* not doing this, will cause a crash in fs_enet_napi */
=======
	/* to initialize the fep->cur_rx,...
	 * not doing this, will cause a crash in fs_enet_napi
	 */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	fs_init_bds(fep->ndev);

	napi_enable(&fep->napi);

	/* Install our interrupt handler. */
	r = request_irq(fep->interrupt, fs_enet_interrupt, IRQF_SHARED,
			"fs_enet-mac", dev);
	if (r != 0) {
		dev_err(fep->dev, "Could not allocate FS_ENET IRQ!");
		napi_disable(&fep->napi);
		return -EINVAL;
	}

<<<<<<< HEAD
	err = fs_init_phy(dev);
=======
	err = phylink_of_phy_connect(fep->phylink, fep->dev->of_node, 0);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (err) {
		free_irq(fep->interrupt, dev);
		napi_disable(&fep->napi);
		return err;
	}
<<<<<<< HEAD
	phy_start(dev->phydev);
=======
	phylink_start(fep->phylink);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	netif_start_queue(dev);

	return 0;
}

static int fs_enet_close(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	unsigned long flags;

	netif_stop_queue(dev);
<<<<<<< HEAD
	netif_carrier_off(dev);
	napi_disable(&fep->napi);
	cancel_work_sync(&fep->timeout_work);
	phy_stop(dev->phydev);
=======
	napi_disable(&fep->napi);
	cancel_work(&fep->timeout_work);
	phylink_stop(fep->phylink);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	spin_lock_irqsave(&fep->lock, flags);
	spin_lock(&fep->tx_lock);
	(*fep->ops->stop)(dev);
	spin_unlock(&fep->tx_lock);
	spin_unlock_irqrestore(&fep->lock, flags);
<<<<<<< HEAD

	/* release any irqs */
	phy_disconnect(dev->phydev);
=======
	phylink_disconnect_phy(fep->phylink);

	/* release any irqs */
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	free_irq(fep->interrupt, dev);

	return 0;
}

<<<<<<< HEAD
/*************************************************************************/

static void fs_get_drvinfo(struct net_device *dev,
			    struct ethtool_drvinfo *info)
=======
static void fs_get_drvinfo(struct net_device *dev,
			   struct ethtool_drvinfo *info)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	strscpy(info->driver, DRV_MODULE_NAME, sizeof(info->driver));
}

static int fs_get_regs_len(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	return (*fep->ops->get_regs_len)(dev);
}

static void fs_get_regs(struct net_device *dev, struct ethtool_regs *regs,
<<<<<<< HEAD
			 void *p)
=======
			void *p)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	unsigned long flags;
	int r, len;

	len = regs->len;

	spin_lock_irqsave(&fep->lock, flags);
	r = (*fep->ops->get_regs)(dev, p, &len);
	spin_unlock_irqrestore(&fep->lock, flags);

	if (r == 0)
		regs->version = 0;
}

static u32 fs_get_msglevel(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
<<<<<<< HEAD
=======

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	return fep->msg_enable;
}

static void fs_set_msglevel(struct net_device *dev, u32 value)
{
	struct fs_enet_private *fep = netdev_priv(dev);
<<<<<<< HEAD
=======

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	fep->msg_enable = value;
}

static int fs_get_tunable(struct net_device *dev,
			  const struct ethtool_tunable *tuna, void *data)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	struct fs_platform_info *fpi = fep->fpi;
	int ret = 0;

	switch (tuna->id) {
	case ETHTOOL_RX_COPYBREAK:
		*(u32 *)data = fpi->rx_copybreak;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int fs_set_tunable(struct net_device *dev,
			  const struct ethtool_tunable *tuna, const void *data)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	struct fs_platform_info *fpi = fep->fpi;
	int ret = 0;

	switch (tuna->id) {
	case ETHTOOL_RX_COPYBREAK:
		fpi->rx_copybreak = *(u32 *)data;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

<<<<<<< HEAD
=======
static int fs_ethtool_set_link_ksettings(struct net_device *dev,
					 const struct ethtool_link_ksettings *cmd)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	return phylink_ethtool_ksettings_set(fep->phylink, cmd);
}

static int fs_ethtool_get_link_ksettings(struct net_device *dev,
					 struct ethtool_link_ksettings *cmd)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	return phylink_ethtool_ksettings_get(fep->phylink, cmd);
}

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static const struct ethtool_ops fs_ethtool_ops = {
	.get_drvinfo = fs_get_drvinfo,
	.get_regs_len = fs_get_regs_len,
	.nway_reset = phy_ethtool_nway_reset,
	.get_link = ethtool_op_get_link,
	.get_msglevel = fs_get_msglevel,
	.set_msglevel = fs_set_msglevel,
	.get_regs = fs_get_regs,
	.get_ts_info = ethtool_op_get_ts_info,
<<<<<<< HEAD
	.get_link_ksettings = phy_ethtool_get_link_ksettings,
	.set_link_ksettings = phy_ethtool_set_link_ksettings,
=======
	.get_link_ksettings = fs_ethtool_get_link_ksettings,
	.set_link_ksettings = fs_ethtool_set_link_ksettings,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	.get_tunable = fs_get_tunable,
	.set_tunable = fs_set_tunable,
};

<<<<<<< HEAD
/**************************************************************************************/

=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#ifdef CONFIG_FS_ENET_HAS_FEC
#define IS_FEC(ops) ((ops) == &fs_fec_ops)
#else
#define IS_FEC(ops) 0
#endif

static const struct net_device_ops fs_enet_netdev_ops = {
	.ndo_open		= fs_enet_open,
	.ndo_stop		= fs_enet_close,
	.ndo_start_xmit		= fs_enet_start_xmit,
	.ndo_tx_timeout		= fs_timeout,
	.ndo_set_rx_mode	= fs_set_multicast_list,
<<<<<<< HEAD
	.ndo_eth_ioctl		= phy_do_ioctl_running,
=======
	.ndo_eth_ioctl		= fs_eth_ioctl,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_set_mac_address	= eth_mac_addr,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= fs_enet_netpoll,
#endif
};

<<<<<<< HEAD
static int fs_enet_probe(struct platform_device *ofdev)
{
	const struct fs_ops *ops;
	struct net_device *ndev;
	struct fs_enet_private *fep;
	struct fs_platform_info *fpi;
	const u32 *data;
	struct clk *clk;
	int err;
	const char *phy_connection_type;
	int privsize, len, ret = -ENODEV;
=======
static const struct phylink_mac_ops fs_enet_phylink_mac_ops = {
	.mac_config = fs_mac_config,
	.mac_link_down = fs_mac_link_down,
	.mac_link_up = fs_mac_link_up,
};

static int fs_enet_probe(struct platform_device *ofdev)
{
	int privsize, len, ret = -ENODEV;
	struct fs_platform_info *fpi;
	struct fs_enet_private *fep;
	phy_interface_t phy_mode;
	const struct fs_ops *ops;
	struct net_device *ndev;
	struct phylink *phylink;
	const u32 *data;
	struct clk *clk;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	ops = device_get_match_data(&ofdev->dev);
	if (!ops)
		return -EINVAL;

	fpi = kzalloc(sizeof(*fpi), GFP_KERNEL);
	if (!fpi)
		return -ENOMEM;

	if (!IS_FEC(ops)) {
		data = of_get_property(ofdev->dev.of_node, "fsl,cpm-command", &len);
		if (!data || len != 4)
			goto out_free_fpi;

		fpi->cp_command = *data;
	}

<<<<<<< HEAD
=======
	ret = of_get_phy_mode(ofdev->dev.of_node, &phy_mode);
	if (ret) {
		/* For compatibility, if the mode isn't specified in DT,
		 * assume MII
		 */
		phy_mode = PHY_INTERFACE_MODE_MII;
	}

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	fpi->rx_ring = RX_RING_SIZE;
	fpi->tx_ring = TX_RING_SIZE;
	fpi->rx_copybreak = 240;
	fpi->napi_weight = 17;
<<<<<<< HEAD
	fpi->phy_node = of_parse_phandle(ofdev->dev.of_node, "phy-handle", 0);
	if (!fpi->phy_node && of_phy_is_fixed_link(ofdev->dev.of_node)) {
		err = of_phy_register_fixed_link(ofdev->dev.of_node);
		if (err)
			goto out_free_fpi;

		/* In the case of a fixed PHY, the DT node associated
		 * to the PHY is the Ethernet MAC DT node.
		 */
		fpi->phy_node = of_node_get(ofdev->dev.of_node);
	}

	if (of_device_is_compatible(ofdev->dev.of_node, "fsl,mpc5125-fec")) {
		phy_connection_type = of_get_property(ofdev->dev.of_node,
						"phy-connection-type", NULL);
		if (phy_connection_type && !strcmp("rmii", phy_connection_type))
			fpi->use_rmii = 1;
	}
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	/* make clock lookup non-fatal (the driver is shared among platforms),
	 * but require enable to succeed when a clock was specified/found,
	 * keep a reference to the clock upon successful acquisition
	 */
<<<<<<< HEAD
	clk = devm_clk_get(&ofdev->dev, "per");
	if (!IS_ERR(clk)) {
		ret = clk_prepare_enable(clk);
		if (ret)
			goto out_deregister_fixed_link;

		fpi->clk_per = clk;
	}

	privsize = sizeof(*fep) +
	           sizeof(struct sk_buff **) *
=======
	clk = devm_clk_get_optional_enabled(&ofdev->dev, "per");
	if (IS_ERR(clk))
		goto out_free_fpi;

	privsize = sizeof(*fep) +
		   sizeof(struct sk_buff **) *
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		     (fpi->rx_ring + fpi->tx_ring) +
		   sizeof(char) * fpi->tx_ring;

	ndev = alloc_etherdev(privsize);
	if (!ndev) {
		ret = -ENOMEM;
<<<<<<< HEAD
		goto out_put;
=======
		goto out_free_fpi;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}

	SET_NETDEV_DEV(ndev, &ofdev->dev);
	platform_set_drvdata(ofdev, ndev);

	fep = netdev_priv(ndev);
	fep->dev = &ofdev->dev;
	fep->ndev = ndev;
	fep->fpi = fpi;
	fep->ops = ops;

<<<<<<< HEAD
	ret = fep->ops->setup_data(ndev);
	if (ret)
		goto out_free_dev;
=======
	fep->phylink_config.dev = &ndev->dev;
	fep->phylink_config.type = PHYLINK_NETDEV;
	fep->phylink_config.mac_capabilities = MAC_10 | MAC_100;

	__set_bit(PHY_INTERFACE_MODE_MII,
		  fep->phylink_config.supported_interfaces);

	if (of_device_is_compatible(ofdev->dev.of_node, "fsl,mpc5125-fec"))
		__set_bit(PHY_INTERFACE_MODE_RMII,
			  fep->phylink_config.supported_interfaces);

	phylink = phylink_create(&fep->phylink_config, dev_fwnode(fep->dev),
				 phy_mode, &fs_enet_phylink_mac_ops);
	if (IS_ERR(phylink)) {
		ret = PTR_ERR(phylink);
		goto out_free_dev;
	}

	fep->phylink = phylink;

	ret = fep->ops->setup_data(ndev);
	if (ret)
		goto out_phylink;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	fep->rx_skbuff = (struct sk_buff **)&fep[1];
	fep->tx_skbuff = fep->rx_skbuff + fpi->rx_ring;
	fep->mapped_as_page = (char *)(fep->rx_skbuff + fpi->rx_ring +
				       fpi->tx_ring);

	spin_lock_init(&fep->lock);
	spin_lock_init(&fep->tx_lock);

	of_get_ethdev_address(ofdev->dev.of_node, ndev);

	ret = fep->ops->allocate_bd(ndev);
	if (ret)
		goto out_cleanup_data;

	fep->rx_bd_base = fep->ring_base;
	fep->tx_bd_base = fep->rx_bd_base + fpi->rx_ring;

	fep->tx_ring = fpi->tx_ring;
	fep->rx_ring = fpi->rx_ring;

	ndev->netdev_ops = &fs_enet_netdev_ops;
	ndev->watchdog_timeo = 2 * HZ;
	INIT_WORK(&fep->timeout_work, fs_timeout_work);
	netif_napi_add_weight(ndev, &fep->napi, fs_enet_napi,
			      fpi->napi_weight);

	ndev->ethtool_ops = &fs_ethtool_ops;

<<<<<<< HEAD
	netif_carrier_off(ndev);

=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	ndev->features |= NETIF_F_SG;

	ret = register_netdev(ndev);
	if (ret)
		goto out_free_bd;

	pr_info("%s: fs_enet: %pM\n", ndev->name, ndev->dev_addr);

	return 0;

out_free_bd:
	fep->ops->free_bd(ndev);
out_cleanup_data:
	fep->ops->cleanup_data(ndev);
<<<<<<< HEAD
out_free_dev:
	free_netdev(ndev);
out_put:
	clk_disable_unprepare(fpi->clk_per);
out_deregister_fixed_link:
	of_node_put(fpi->phy_node);
	if (of_phy_is_fixed_link(ofdev->dev.of_node))
		of_phy_deregister_fixed_link(ofdev->dev.of_node);
=======
out_phylink:
	phylink_destroy(fep->phylink);
out_free_dev:
	free_netdev(ndev);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
out_free_fpi:
	kfree(fpi);
	return ret;
}

static void fs_enet_remove(struct platform_device *ofdev)
{
	struct net_device *ndev = platform_get_drvdata(ofdev);
	struct fs_enet_private *fep = netdev_priv(ndev);

	unregister_netdev(ndev);

	fep->ops->free_bd(ndev);
	fep->ops->cleanup_data(ndev);
	dev_set_drvdata(fep->dev, NULL);
<<<<<<< HEAD
	of_node_put(fep->fpi->phy_node);
	clk_disable_unprepare(fep->fpi->clk_per);
	if (of_phy_is_fixed_link(ofdev->dev.of_node))
		of_phy_deregister_fixed_link(ofdev->dev.of_node);
=======
	phylink_destroy(fep->phylink);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	free_netdev(ndev);
}

static const struct of_device_id fs_enet_match[] = {
#ifdef CONFIG_FS_ENET_HAS_SCC
	{
		.compatible = "fsl,cpm1-scc-enet",
		.data = (void *)&fs_scc_ops,
	},
	{
		.compatible = "fsl,cpm2-scc-enet",
		.data = (void *)&fs_scc_ops,
	},
#endif
#ifdef CONFIG_FS_ENET_HAS_FCC
	{
		.compatible = "fsl,cpm2-fcc-enet",
		.data = (void *)&fs_fcc_ops,
	},
#endif
#ifdef CONFIG_FS_ENET_HAS_FEC
#ifdef CONFIG_FS_ENET_MPC5121_FEC
	{
		.compatible = "fsl,mpc5121-fec",
		.data = (void *)&fs_fec_ops,
	},
	{
		.compatible = "fsl,mpc5125-fec",
		.data = (void *)&fs_fec_ops,
	},
#else
	{
		.compatible = "fsl,pq1-fec-enet",
		.data = (void *)&fs_fec_ops,
	},
#endif
#endif
	{}
};
MODULE_DEVICE_TABLE(of, fs_enet_match);

static struct platform_driver fs_enet_driver = {
	.driver = {
		.name = "fs_enet",
		.of_match_table = fs_enet_match,
	},
	.probe = fs_enet_probe,
	.remove_new = fs_enet_remove,
};

#ifdef CONFIG_NET_POLL_CONTROLLER
static void fs_enet_netpoll(struct net_device *dev)
{
<<<<<<< HEAD
       disable_irq(dev->irq);
       fs_enet_interrupt(dev->irq, dev);
       enable_irq(dev->irq);
=======
	disable_irq(dev->irq);
	fs_enet_interrupt(dev->irq, dev);
	enable_irq(dev->irq);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}
#endif

module_platform_driver(fs_enet_driver);
