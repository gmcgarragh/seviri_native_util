/*******************************************************************************
 *
 *    Copyright (C) 2014-2018 Greg McGarragh <mcgarragh@atm.ox.ac.uk>
 *
 *    This source code is licensed under the GNU General Public License (GPL),
 *    Version 3.  See the file COPYING for more details.
 *
 ******************************************************************************/

#ifndef SEVIRI_UTIL_DLM_H
#define SEVIRI_UTIL_DLM_H

/* Message Numbers */
#define SEVIRI_PREPROC_ERROR		0
#define SEVIRI_PREPROC_NOSTRINGARRAY	-1

/* Useful macro */
#define ARRLEN(arr) (sizeof(arr)/sizeof(arr[0]))

extern IDL_MSG_BLOCK msg_block;

/* Define the startup function that adds C functions to IDL along with the exit
   handler */

extern int  seviri_util_startup(void);
extern void seviri_util_exit_handler(void);

/* Define the wrapper function which calls seviri_util */
extern void IDL_CDECL seviri_preproc_dlm(int argc, IDL_VPTR argv[], char *argk);

#endif
