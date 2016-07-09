/**
 * @file
 *
 * IPv6 layer.
 */

/*
 * Copyright (c) 2010 Inico Technologies Ltd.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Ivan Delamer <delamer@inicotech.com>
 *
 *
 * Please coordinate changes and requests with Ivan Delamer
 * <delamer@inicotech.com>
 */
#ifndef LWIP_HDR_IP6_H
#define LWIP_HDR_IP6_H

#include "lwip/opt.h"

#if LWIP_IPV6  /* don't build if not configured for use in lwipopts.h */

#include "lwip/ip6_addr.h"
#include "lwip/def.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"

#include "lwip/err.h"

LWIP_EXTERN_C_BEGIN

#define IP6_HLEN 40

#define IP6_NEXTH_HOPBYHOP  0
#define IP6_NEXTH_TCP       6
#define IP6_NEXTH_UDP       17
#define IP6_NEXTH_ENCAPS    41
#define IP6_NEXTH_ROUTING   43
#define IP6_NEXTH_FRAGMENT  44
#define IP6_NEXTH_ICMP6     58
#define IP6_NEXTH_NONE      59
#define IP6_NEXTH_DESTOPTS  60
#define IP6_NEXTH_UDPLITE   136

/* The IPv6 header. */
PACK_STRUCT_BEGIN
struct ip6_hdr {
  /* version / traffic class / flow label */
  PACK_STRUCT_FIELD(u32_t _v_tc_fl);
  /* payload length */
  PACK_STRUCT_FIELD(u16_t _plen);
  /* next header */
  PACK_STRUCT_FLD_8(u8_t _nexth);
  /* hop limit */
  PACK_STRUCT_FLD_8(u8_t _hoplim);
  /* source and destination IP addresses */
  PACK_STRUCT_FLD_S(ip6_addr_p_t src);
  PACK_STRUCT_FLD_S(ip6_addr_p_t dest);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

/* Hop-by-hop router alert option. */
#define IP6_HBH_HLEN    8
#define IP6_PAD1_OPTION         0
#define IP6_PADN_ALERT_OPTION   1
#define IP6_ROUTER_ALERT_OPTION 5
#define IP6_ROUTER_ALERT_VALUE_MLD 0
PACK_STRUCT_BEGIN
struct ip6_hbh_hdr {
  /* next header */
  PACK_STRUCT_FLD_8(u8_t _nexth);
  /* header length */
  PACK_STRUCT_FLD_8(u8_t _hlen);
  /* router alert option type */
  PACK_STRUCT_FLD_8(u8_t _ra_opt_type);
  /* router alert option data len */
  PACK_STRUCT_FLD_8(u8_t _ra_opt_dlen);
  /* router alert option data */
  PACK_STRUCT_FIELD(u16_t _ra_opt_data);
  /* PadN option type */
  PACK_STRUCT_FLD_8(u8_t _padn_opt_type);
  /* PadN option data len */
  PACK_STRUCT_FLD_8(u8_t _padn_opt_dlen);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

/* Fragment header. */
#define IP6_FRAG_HLEN    8
#define IP6_FRAG_OFFSET_MASK    0xfff8
#define IP6_FRAG_MORE_FLAG      0x0001
PACK_STRUCT_BEGIN
struct ip6_frag_hdr {
  /* next header */
  PACK_STRUCT_FLD_8(u8_t _nexth);
  /* reserved */
  PACK_STRUCT_FLD_8(u8_t reserved);
  /* fragment offset */
  PACK_STRUCT_FIELD(u16_t _fragment_offset);
  /* fragmented packet identification */
  PACK_STRUCT_FIELD(u32_t _identification);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

#define IP6H_V(hdr)  ((lwip_ntohl((hdr)->_v_tc_fl) >> 28) & 0x0f)
#define IP6H_TC(hdr) ((lwip_ntohl((hdr)->_v_tc_fl) >> 20) & 0xff)
#define IP6H_FL(hdr) (lwip_ntohl((hdr)->_v_tc_fl) & 0x000fffff)
#define IP6H_PLEN(hdr) (lwip_ntohs((hdr)->_plen))
#define IP6H_NEXTH(hdr) ((hdr)->_nexth)
#define IP6H_HOPLIM(hdr) ((hdr)->_hoplim)

#define IP6H_VTCFL_SET(hdr, v, tc, fl) (hdr)->_v_tc_fl = (lwip_htonl((((u32_t)(v)) << 28) | (((u32_t)(tc)) << 20) | (fl)))
#define IP6H_PLEN_SET(hdr, plen) (hdr)->_plen = lwip_htons(plen)
#define IP6H_NEXTH_SET(hdr, nexth) (hdr)->_nexth = (nexth)
#define IP6H_HOPLIM_SET(hdr, hl) (hdr)->_hoplim = (u8_t)(hl)

struct netif *ip6_route(const ip6_addr_t *src, const ip6_addr_t *dest);
const ip_addr_t *ip6_select_source_address(struct netif *netif, const ip6_addr_t * dest);
err_t         ip6_input(struct pbuf *p, struct netif *inp);
err_t         ip6_output_if(struct pbuf *p, const ip6_addr_t *src, const ip6_addr_t *dest,
                            u8_t hl, u8_t tc, u8_t nexth, struct netif *netif);
err_t         ip6_output_if_src(struct pbuf *p, const ip6_addr_t *src, const ip6_addr_t *dest,
                            u8_t hl, u8_t tc, u8_t nexth, struct netif *netif);
#if LWIP_IPV6_MLD
err_t         ip6_options_add_hbh_ra(struct pbuf * p, u8_t nexth, u8_t value);
#endif /* LWIP_IPV6_MLD */

#define ip6_netif_get_local_ip(netif, dest) (((netif) != NULL) ? \
  ip6_select_source_address(netif, dest) : NULL)

#if IP6_DEBUG
void ip6_debug_print(struct pbuf *p);
#else
#define ip6_debug_print(p)
#endif /* IP6_DEBUG */

LWIP_EXTERN_C_END

#endif /* LWIP_IPV6 */

#endif /* LWIP_HDR_IP6_H */
