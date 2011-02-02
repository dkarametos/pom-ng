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


#ifndef __POM_NG_BASE_H__
#define __POM_NG_BASE_H__

// Default return values
#define POM_OK 0
#define POM_ERR -1

#include <pom-ng/pomlog.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

#define POM_STRERROR_BUFF_SIZE 128

// Thread safe version of strerror()
char *pom_strerror(int err);

// Out of memory handler
void pom_oom_internal(size_t size, char *file, unsigned int line);
#define pom_oom(x) pom_oom_internal(x, __FILE__, __LINE__)

// Locking handlers
void pom_mutex_lock_internal(pthread_mutex_t *m, char *file, unsigned int line);
#define pom_mutex_lock(x) pom_mutex_lock_internal(x, __FILE__, __LINE__)

void pom_mutex_unlock_internal(pthread_mutex_t *m, char *file, unsigned int line);
#define pom_mutex_unlock(x) pom_mutex_unlock_internal(x, __FILE__, __LINE__)




#endif