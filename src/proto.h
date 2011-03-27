/*
 *  This file is part of pom-ng.
 *  Copyright (C) 2010 Guy Martin <gmsoft@tuxicoman.be>
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


#ifndef __PROTO_H__
#define __PROTO_H__

#include <pom-ng/proto.h>
#include "packet.h"
#include "conntrack.h"

struct proto_reg {

	struct proto_reg_info *info;
	struct proto_dependency *dep; // Corresponding dependency
	
	/// Conntrack tables
	struct conntrack_tables *ct;

	// Packet info pool
	struct packet_info_pool pkt_info_pool;

	void *priv;

	struct proto_reg *next, *prev;

};

struct proto_dependency *proto_add_dependency_by_proto(struct proto_reg *proto);
int proto_cleanup();

#endif
