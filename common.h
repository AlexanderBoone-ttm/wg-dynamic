/* SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2019 WireGuard LLC. All Rights Reserved.
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <stdlib.h>

#include <netinet/in.h>

#define MAX_CONNECTIONS 16

#define MAX_LINESIZE 4096

#define RECV_BUFSIZE 8192

static const char WG_DYNAMIC_ADDR[] = "fe80::";
static const uint16_t WG_DYNAMIC_PORT = 1337;

#define ITEMS                                                                  \
	E(WGKEY_UNKNOWN, "") /* must be the first entry */                     \
	/* CMD START */                                                        \
	E(WGKEY_REQUEST_IP, "request_ip")                                      \
	E(WGKEY_ENDCMD, "")                                                    \
	/* CMD END */                                                          \
	E(WGKEY_INCOMPLETE, "")                                                \
	E(WGKEY_IPV4, "ipv4")                                                  \
	E(WGKEY_IPV6, "ipv6")                                                  \
	E(WGKEY_LEASETIME, "leasetime")

#define E(x, y) x,
enum wg_dynamic_key { ITEMS };
#undef E
#define E(x, y) y,
static const char *const WG_DYNAMIC_KEY[] = { ITEMS };
#undef E
#undef ITEMS

struct wg_dynamic_attr {
	enum wg_dynamic_key key;
	size_t len;
	struct wg_dynamic_attr *next;
	unsigned char value[];
};

struct wg_dynamic_request {
	enum wg_dynamic_key cmd;
	uint32_t version;
	struct wg_dynamic_attr *first, *last;
};

struct wg_combined_ip {
	uint16_t family;
	union {
		struct in_addr ip4;
		struct in6_addr ip6;
	} ip;
	uint8_t cidr;
};

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

void free_wg_dynamic_request(struct wg_dynamic_request *req);
int parse_request(struct wg_dynamic_request *req, unsigned char *buf,
		  size_t len);

#endif