/*
 *  This file is part of pom-ng.
 *  Copyright (C) 2012 Guy Martin <gmsoft@tuxicoman.be>
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
#include <pom-ng/ptype_uint8.h>
#include <netinet/icmp6.h>

#include "proto_icmp6.h"


struct mod_reg_info* proto_icmp6_reg_info() {

	static struct mod_reg_info reg_info = { 0 };
	reg_info.api_ver = MOD_API_VER;
	reg_info.register_func = proto_icmp6_mod_register;
	reg_info.unregister_func = proto_icmp6_mod_unregister;
	reg_info.dependencies = "ptype_uint8";

	return &reg_info;
}

static int proto_icmp6_mod_register(struct mod_reg *mod) {

	static struct proto_pkt_field fields[PROTO_ICMP6_FIELD_NUM + 1] = { { 0 } };
	fields[0].name = "type";
	fields[0].value_type = ptype_get_type("uint8");
	fields[0].description = "Type";
	fields[1].name = "code";
	fields[1].value_type = ptype_get_type("uint8");
	fields[1].description = "Code";

	static struct proto_reg_info proto_icmp6 = { 0 };
	proto_icmp6.name = "icmp6";
	proto_icmp6.api_ver = PROTO_API_VER;
	proto_icmp6.mod = mod;
	proto_icmp6.pkt_fields = fields;

	// No contrack here

	proto_icmp6.process = proto_icmp6_process;

	if (proto_register(&proto_icmp6) == POM_OK)
		return POM_OK;

	return POM_ERR;

}


static int proto_icmp6_process(struct proto *proto, struct packet *p, struct proto_process_stack *stack, unsigned int stack_index) {

	struct proto_process_stack *s = &stack[stack_index];

	if (s->plen < sizeof(struct icmp6_hdr))
		return PROTO_INVALID;

	struct icmp6_hdr *ihdr = s->pload;

	PTYPE_UINT8_SETVAL(s->pkt_info->fields_value[proto_icmp6_field_type], ihdr->icmp6_type);
	PTYPE_UINT8_SETVAL(s->pkt_info->fields_value[proto_icmp6_field_code], ihdr->icmp6_code);

	return PROTO_OK;
}

static int proto_icmp6_mod_unregister() {

	return proto_unregister("icmp6");
}
