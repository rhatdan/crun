/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017 Giuseppe Scrivano <giuseppe@scrivano.org>
 * libocispec is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libocispec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONTAINER_H
# define CONTAINER_H

# include <config.h>
# include <oci_runtime_spec.h>
# include "error.h"

struct libcrun_context_s
{
  char *state_root;
  const char *id;
  const char *console_socket;
  char *pid_file;
  char *notify_socket;
  int preserve_fds;
  FILE *stderr;

  const char *cwd;

  int fifo_exec_wait_fd;

  int tty : 1;
  int has_fifo_exec_wait : 1;
  int systemd_cgroup : 1;
  int detach : 1;
  int no_subreaper: 1;
  int no_new_keyring : 1;
};

struct libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  oci_container *container_def;

  uid_t host_uid;
  gid_t host_gid;

  uid_t container_uid;
  gid_t container_gid;

  void *private_data;
  struct libcrun_context_s *context;
};

enum
  {
    LIBCRUN_RUN_NO_FORK = 1,
  };

typedef struct libcrun_container_s libcrun_container;

libcrun_container *libcrun_container_load (const char *path, libcrun_error_t *err);

int libcrun_container_run (libcrun_container *container, struct libcrun_context_s *context, unsigned int options, libcrun_error_t *error);

int libcrun_delete_container (struct libcrun_context_s *context, oci_container *def, const char *id, int force, libcrun_error_t *err);

int libcrun_kill_container (struct libcrun_context_s *context, const char *id, int signal, libcrun_error_t *err);

int libcrun_container_create (libcrun_container *container, struct libcrun_context_s *context, libcrun_error_t *err);

int libcrun_container_start (struct libcrun_context_s *context, const char *id, libcrun_error_t *err);

int libcrun_container_state (FILE *out, struct libcrun_context_s *context, const char *id, libcrun_error_t *err);

int libcrun_exec_container (struct libcrun_context_s *context, const char *id, int argc, char **argv, libcrun_error_t *err);

#endif
