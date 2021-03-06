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

#ifndef __PROTO_UDP_H__
#define __PROTO_UDP_H__

#define PROTO_UDP_FIELD_NUM 2

enum proto_udp_fields {
	proto_udp_field_sport = 0,
	proto_udp_field_dport,
};

struct mod_reg_info* proto_udp_reg_info();
static int proto_udp_mod_register(struct mod_reg *mod);
static int proto_udp_init(struct proto *proto, struct registry_instance *i);
static int proto_udp_cleanup(struct proto *p);
static int proto_udp_process(struct proto *proto, struct packet *p, struct proto_process_stack *stack, unsigned int stack_index);
static int proto_udp_mod_unregister();

#endif
