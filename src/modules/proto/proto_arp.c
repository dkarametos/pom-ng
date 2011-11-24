/*
 *  This file is part of pom-ng.
 *  Copyright (C) 2011 Guy Martin <gmsoft@tuxicoman.be>
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

#include <pom-ng/ptype.h>
#include <pom-ng/proto.h>
#include <pom-ng/ptype_mac.h>
#include <pom-ng/ptype_uint16.h>
#include <pom-ng/ptype_ipv4.h>

#include "proto_arp.h"


// ptype for fields value template
static struct ptype *ptype_mac = NULL, *ptype_uint16 = NULL, *ptype_ipv4 = NULL;

struct mod_reg_info* proto_arp_reg_info() {
	static struct mod_reg_info reg_info;
	memset(&reg_info, 0, sizeof(struct mod_reg_info));
	reg_info.api_ver = MOD_API_VER;
	reg_info.register_func = proto_arp_mod_register;
	reg_info.unregister_func = proto_arp_mod_unregister;
	reg_info.dependencies = "ptype_uint16, ptype_mac, ptype_ipv4";

	return &reg_info;
}


static int proto_arp_mod_register(struct mod_reg *mod) {

	ptype_mac = ptype_alloc("mac");
	ptype_uint16 = ptype_alloc("uint16");
	ptype_ipv4 = ptype_alloc("ipv4");
	
	if (!ptype_mac || !ptype_uint16 || !ptype_ipv4)
		goto err;

	static struct proto_pkt_field fields[PROTO_ARP_FIELD_NUM + 1];
	memset(fields, 0, sizeof(struct proto_pkt_field) * (PROTO_ARP_FIELD_NUM + 1));
	fields[0].name = "oper";
	fields[0].value_template = ptype_uint16;
	fields[0].description = "Operation";
	fields[1].name = "sender_hw_addr";
	fields[1].value_template = ptype_mac;
	fields[1].description = "Sender hardware address";
	fields[2].name = "sender_proto_addr";
	fields[2].value_template = ptype_ipv4;
	fields[2].description = "Sender protocol address";
	fields[3].name = "target_hw_addr";
	fields[3].value_template = ptype_mac;
	fields[3].description = "Target hardware address";
	fields[4].name = "target_proto_addr";
	fields[4].value_template = ptype_ipv4;
	fields[4].description = "Target protocol address";


	static struct proto_reg_info proto_arp;
	memset(&proto_arp, 0, sizeof(struct proto_reg_info));
	proto_arp.name = "arp";
	proto_arp.api_ver = PROTO_API_VER;
	proto_arp.mod = mod;
	proto_arp.pkt_fields = fields;

	// No contrack here

	proto_arp.process = proto_arp_process;

	if (proto_register(&proto_arp) == POM_OK)
		return POM_OK;

err:
	proto_arp_mod_unregister();
	return POM_ERR;

}

static int proto_arp_process(struct proto *proto, struct packet *p, struct proto_process_stack *stack, unsigned int stack_index) {

	struct proto_process_stack *s = &stack[stack_index];

	if (sizeof(struct arp_packet) > s->plen)
		return PROTO_INVALID;

	struct arp_packet *apkt = s->pload;

	if (ntohs(apkt->hw_type) != 0x1)
		// We only support arp for ethernet links for now
		return PROTO_INVALID;

	if (ntohs(apkt->proto_type) != 0x0800)
		// We only support arp for IPv4 addresses
		return PROTO_INVALID;

	if (apkt->hw_addr_len != 6)
		// Ethernet addresses are 6 bytes long
		return PROTO_INVALID;

	if (apkt->proto_addr_len != 4)
		// IPv4 addresses are 4 bytes long
		return PROTO_INVALID;

	PTYPE_UINT16_SETVAL(s->pkt_info->fields_value[proto_arp_field_oper], ntohs(apkt->oper));
	PTYPE_MAC_SETADDR(s->pkt_info->fields_value[proto_arp_field_sender_hw_addr], apkt->sender_hw_addr);
	PTYPE_IPV4_SETADDR(s->pkt_info->fields_value[proto_arp_field_sender_proto_addr], apkt->sender_proto_addr);
	PTYPE_MAC_SETADDR(s->pkt_info->fields_value[proto_arp_field_target_hw_addr], apkt->target_hw_addr);
	PTYPE_IPV4_SETADDR(s->pkt_info->fields_value[proto_arp_field_target_proto_addr], apkt->target_proto_addr);


	return PROTO_OK;

}


static int proto_arp_mod_unregister() {

	int res = proto_unregister("arp");

	if (ptype_mac) {
		ptype_cleanup(ptype_mac);
		ptype_mac = NULL;
	}

	if (ptype_uint16) {
		ptype_cleanup(ptype_uint16);
		ptype_uint16 = NULL;
	}

	if (ptype_ipv4) {
		ptype_cleanup(ptype_ipv4);
		ptype_ipv4 = NULL;
	}
		
	return res;
}
