/*
 * (C) Copyright 2004
 * Pierre Aubert, Staubli Faverges , <p.aubert@staubli.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/************************************************************************
  Get Parameters for the video mode:
  The default video mode can be defined in CONFIG_SYS_DEFAULT_VIDEO_MODE.
  If undefined, default video mode is set to 0x301
  Parameters can be set via the variable "videomode" in the environment.
  2 diferent ways are possible:
  "videomode=301"   - 301 is a hexadecimal number describing the VESA
		      mode. Following modes are implemented:

		      Colors	640x480 800x600 1024x768 1152x864 1280x1024
		     --------+---------------------------------------------
		      8 bits |	0x301	0x303	 0x305	  0x161	    0x307
		     15 bits |	0x310	0x313	 0x316	  0x162	    0x319
		     16 bits |	0x311	0x314	 0x317	  0x163	    0x31A
		     24 bits |	0x312	0x315	 0x318	    ?	    0x31B
		     --------+---------------------------------------------
  "videomode=bootargs"
		   - the parameters are parsed from the bootargs.
		      The format is "NAME:VALUE,NAME:VALUE" etc.
		      Ex.:
		      "bootargs=video=ctfb:x:800,y:600,depth:16,pclk:25000"
		      Parameters not included in the list will be taken from
		      the default mode, which is one of the following:
		      mode:0  640x480x24
		      mode:1  800x600x16
		      mode:2  1024x768x8
		      mode:3  960x720x24
		      mode:4  1152x864x16
		      mode:5  1280x1024x8

		      if "mode" is not provided within the parameter list,
		      mode:0 is assumed.
		      Following parameters are supported:
		      x	      xres = visible resolution horizontal
		      y	      yres = visible resolution vertical
		      pclk    pixelclocks in pico sec
		      le      left_marging time from sync to picture in pixelclocks
		      ri      right_marging time from picture to sync in pixelclocks
		      up      upper_margin time from sync to picture
		      lo      lower_margin
		      hs      hsync_len length of horizontal sync
		      vs      vsync_len length of vertical sync
		      sync    see FB_SYNC_*
		      vmode   see FB_VMODE_*
		      depth   Color depth in bits per pixel
		      All other parameters in the variable bootargs are ignored.
		      It is also possible to set the parameters direct in the
		      variable "videomode", or in another variable i.e.
		      "myvideo" and setting the variable "videomode=myvideo"..
****************************************************************************/

#include <common.h>
#include "videomodes.h"

const struct ctfb_vesa_modes vesa_modes[VESA_MODES_COUNT] = {
	{0x301, RES_MODE_640x480, 8},
	{0x310, RES_MODE_640x480, 15},
	{0x311, RES_MODE_640x480, 16},
	{0x312, RES_MODE_640x480, 24},
	{0x303, RES_MODE_800x600, 8},
	{0x313, RES_MODE_800x600, 15},
	{0x314, RES_MODE_800x600, 16},
	{0x315, RES_MODE_800x600, 24},
	{0x305, RES_MODE_1024x768, 8},
	{0x316, RES_MODE_1024x768, 15},
	{0x317, RES_MODE_1024x768, 16},
	{0x318, RES_MODE_1024x768, 24},
	{0x161, RES_MODE_1152x864, 8},
	{0x162, RES_MODE_1152x864, 15},
	{0x163, RES_MODE_1152x864, 16},
	{0x307, RES_MODE_1280x1024, 8},
	{0x319, RES_MODE_1280x1024, 15},
	{0x31A, RES_MODE_1280x1024, 16},
	{0x31B, RES_MODE_1280x1024, 24},
	{0x211, RES_MODE_320x240, 16},  //add by mickeyos
};
const struct ctfb_res_modes res_mode_init[RES_MODES_COUNT] = {
	/* x	 y pixclk   le	ri  up	lo   hs vs  s  vmode */
	{640, 480, 39721, 40, 24, 32, 11, 96, 2, 0, FB_VMODE_NONINTERLACED},
	{800, 600, 27778, 64, 24, 22, 1, 72, 2, 0, FB_VMODE_NONINTERLACED},
	{1024, 768, 15384, 168, 8, 29, 3, 144, 4, 0, FB_VMODE_NONINTERLACED},
	{960, 720, 13100, 160, 40, 32, 8, 80, 4, 0, FB_VMODE_NONINTERLACED},
	{1152, 864, 12004, 200, 64, 32, 16, 80, 4, 0, FB_VMODE_NONINTERLACED},
	{1280, 1024, 9090, 200, 48, 26, 1, 184, 3, 0, FB_VMODE_NONINTERLACED},
	{320, 240, 100000, 19, 24, 7, 5, 44, 15, 0, FB_VMODE_NONINTERLACED},  //add by mickeyos
};

/************************************************************************
 * Get Parameters for the video mode:
 */
/*********************************************************************
 * returns the length to the next seperator
 */
static int
video_get_param_len (char *start, char sep)
{
	int i = 0;
	while ((*start != 0) && (*start != sep)) {
		start++;
		i++;
	}
	return i;
}

static int
video_search_param (char *start, char *param)
{
	int len, totallen, i;
	char *p = start;
	len = strlen (param);
	totallen = len + strlen (start);
	for (i = 0; i < totallen; i++) {
		if (strncmp (p++, param, len) == 0)
			return (i);
	}
	return -1;
}

/***************************************************************
 * Get parameter via the environment as it is done for the
 * linux kernel i.e:
 * video=ctfb:x:800,xv:1280,y:600,yv:1024,depth:16,mode:0,pclk:25000,
 *	 le:56,ri:48,up:26,lo:5,hs:152,vs:2,sync:0,vmode:0,accel:0
 *
 * penv is a pointer to the environment, containing the string, or the name of
 * another environment variable. It could even be the term "bootargs"
 */

#define GET_OPTION(name,var)				\
	if(strncmp(p,name,strlen(name))==0) {		\
		val_s=p+strlen(name);			\
		var=simple_strtoul(val_s, NULL, 10);	\
	}

int video_get_params (struct ctfb_res_modes *pPar, char *penv)
{
	char *p, *s, *val_s;
	int i = 0, t;
	int bpp;
	int mode;
	/* first search for the environment containing the real param string */
	s = penv;
	if ((p = getenv (s)) != NULL) {
		s = p;
	}
	/* in case of the bootargs line, we have to start
	 * after "video=ctfb:"
	 */
	i = video_search_param (s, "video=ctfb:");
	if (i >= 0) {
		s += i;
		s += strlen ("video=ctfb:");
	}
	/* search for mode as a default value */
	p = s;
	t = 0;
	mode = 0;		/* default */
	while ((i = video_get_param_len (p, ',')) != 0) {
		GET_OPTION ("mode:", mode)
			p += i;
		if (*p != 0)
			p++;	/* skip ',' */
	}
	if (mode >= RES_MODES_COUNT)
		mode = 0;
	*pPar = res_mode_init[mode];	/* copy default values */
	bpp = 24 - ((mode % 3) * 8);
	p = s;			/* restart */
	while ((i = video_get_param_len (p, ',')) != 0) {
		GET_OPTION ("x:", pPar->xres)
			GET_OPTION ("y:", pPar->yres)
			GET_OPTION ("le:", pPar->left_margin)
			GET_OPTION ("ri:", pPar->right_margin)
			GET_OPTION ("up:", pPar->upper_margin)
			GET_OPTION ("lo:", pPar->lower_margin)
			GET_OPTION ("hs:", pPar->hsync_len)
			GET_OPTION ("vs:", pPar->vsync_len)
			GET_OPTION ("sync:", pPar->sync)
			GET_OPTION ("vmode:", pPar->vmode)
			GET_OPTION ("pclk:", pPar->pixclock)
			GET_OPTION ("depth:", bpp)
			p += i;
		if (*p != 0)
			p++;	/* skip ',' */
	}
	return bpp;
}
