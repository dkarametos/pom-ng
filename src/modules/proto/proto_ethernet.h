/*
 *  This file is part of pom-ng.
 *  Copyright (C) 2010-2012 Guy Martin <gmsoft@tuxicoman.be>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __PROTO_ETHERNET_H__
#define __PROTO_ETHERNET_H__

#include <stdint.h>
#define ETH_ALEN 6

struct ether_header
{
	uint8_t  ether_dhost[ETH_ALEN];
	uint8_t  ether_shost[ETH_ALEN];
	uint16_t ether_type;
} __attribute__ ((__packed__));

#define PROTO_ETHERNET_FIELD_NUM 3

enum proto_ethernet_fields {
	proto_ethernet_field_src = 0,
	proto_ethernet_field_dst,
	proto_ethernet_field_type
};

struct mod_reg_info* proto_ethernet_reg_info();
static int proto_ethernet_init(struct proto *proto, struct registry_instance *i);
static int proto_ethernet_mod_register(struct mod_reg *mod);
static int proto_ethernet_process(struct proto *proto, struct packet *p, struct proto_process_stack *stack, unsigned int stack_index);
static int proto_ethernet_mod_unregister();

#endif
