/*
 * Simple MPEG/DVB parser to achieve network/service information without initial tuning data
 *
 * Copyright (C) 2006, 2007, 2008, 2009, 2010 Winfried Koehler 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * Or, point your browser to http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 * The author can be reached at: handygewinnspiel AT gmx DOT de
 *
 * The project's page is http://wirbel.htpc-forum.de/w_scan/index2.html
 */

/* 20101001 --wk */

#include <stdio.h>
#include <stdlib.h>
#include "extended_frontend.h"
#include "scan.h"
#include "dump-mplayer.h"
#include "dump-xine.h"


void mplayer_dump_service_parameter_set (FILE *f, 
                                const char *service_name,
                                const char *provider_name,
                                struct extended_dvb_frontend_parameters *p,
                                uint16_t video_pid,
                                uint16_t *audio_pid,
                                int audio_num,
                                uint16_t *ac3_pid,
                                int ac3_num,
                                uint16_t service_id,
                                struct w_scan_flags * flags)
{
        int i;

        fprintf (f, "%s:", service_name);
        xine_dump_dvb_parameters (f, p, flags);
        fprintf (f, ":%i:%i", video_pid, audio_pid[0]);
        for (i = 1; i < audio_num; i++) {
                fprintf (f, "+%i", audio_pid[i]);
                }
        /* what about AC3 audio here? */
        fprintf (f, ":%i\n", service_id);
}

