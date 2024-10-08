#include <X11/XF86keysym.h> /* to help find keycodes use xev and command xmodmap -pk */
/* See LICENSE file for copyright and license details. */

#define TERMINAL "st"
#define BROWSER "firefox"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 10, .gappx = 20};
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
// background colour
static const char col_gray1[]       = "#222222";
// inactive window border colour
static const char col_gray2[]       = "#444444";
// font colour
static const char col_gray3[]       = "#bbbbbb";
// current tag and current window font colour 
static const char col_gray4[]       = "#eeeeee";
// Top bar second colour (blue) and active window border colour
static const char col_cyan[]        = "#986dab";
//static const char col_cyan[] = "#d3869b";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       0,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { TERMINAL, NULL};
static const char *browser[] = { BROWSER, NULL};
static const char *discord[] = {"discord", NULL};
static const char *zathura[] = {"zathura", NULL};

/* function keys */ 
static const char *brupcmd[] = { "xbacklight", "-inc", "5%", NULL };
static const char *brdowncmd[] = { "xbacklight", "-dec", "5%", NULL };
static const char *upvol[]   = { "amixer", "set", "Master", "5%+",     NULL };
static const char *downvol[] = { "amixer", "set", "Master", "5%-",     NULL };
static const char *mutevol[] = { "amixer", "set", "Master", "toggle", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
    { MODKEY|ShiftMask,             XK_s,      spawn,          {.v = (const char*[]){ "flameshot",  "gui", "-c", "-s", NULL } } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          {.v = browser} },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          {.v = discord} },
	{ MODKEY|ShiftMask,             XK_z,      spawn,          {.v = zathura} },
	{ 0,          XF86XK_MonBrightnessUp,      spawn,          {.v = brupcmd} },
	{ 0,        XF86XK_MonBrightnessDown,      spawn,          {.v = brdowncmd} },
    { MODKEY|ShiftMask,			    XK_m,	   spawn,		   {.v = (const char*[]){ TERMINAL, "-e", "ncmpcpp", NULL } } },
    { 0,                XF86XK_AudioMute,      spawn,          {.v = mutevol} },
    { 0,         XF86XK_AudioLowerVolume,      spawn,          {.v = downvol} },
    { 0,         XF86XK_AudioRaiseVolume,      spawn,          {.v = upvol} },
    { 0,                XF86XK_AudioPrev,	   spawn,		   {.v = (const char*[]){ "mpc", "prev", NULL } } },
	{ 0,                XF86XK_AudioNext,	   spawn,		   {.v = (const char*[]){ "mpc",  "next", NULL } } },
	{ 0,               XF86XK_AudioPause,	   spawn,		   {.v = (const char*[]){ "mpc", "pause", NULL } } },
	{ 0,                XF86XK_AudioPlay,	   spawn,		   {.v = (const char*[]){ "mpc", "play", NULL } } },
	{ 0,                XF86XK_AudioStop,	   spawn,		   {.v = (const char*[]){ "mpc", "stop", NULL } } },
	{ 0,              XF86XK_AudioRewind,	   spawn,		   {.v = (const char*[]){ "mpc", "seek", "-10", NULL } } },
	{ 0,             XF86XK_AudioForward,	   spawn,		   {.v = (const char*[]){ "mpc", "seek", "+10", NULL } } },
    { 0,               XF86XK_AudioMedia,	   spawn,		   {.v = (const char*[]){ TERMINAL, "-e", "ncmpcpp", NULL } } },
	{ 0,                           XK_F2,	   spawn,		   {.v = (const char*[]){ "mpc", "volume", "-5", NULL } } },
	{ 0,                           XK_F3,	   spawn,		   {.v = (const char*[]){ "mpc", "volume", "+5", NULL } } },
	{ 0,                           XK_F1,	   spawn,		   {.v = (const char*[]){ "./.scripts/mpc-pause-play-dwm", NULL } } },
	{ MODKEY,             XK_bracketleft,	   spawn,		   {.v = (const char*[]){ "mpc", "seek", "-10", NULL } } },
	{ MODKEY,            XK_bracketright,	   spawn,		   {.v = (const char*[]){ "mpc", "seek", "+10", NULL } } },
	{ MODKEY|ShiftMask,   XK_bracketleft,	   spawn,		   {.v = (const char*[]){ "mpc", "seek", "-50", NULL } } },
	{ MODKEY|ShiftMask,  XK_bracketright,	   spawn,		   {.v = (const char*[]){ "mpc", "seek", "+50", NULL } } },
	{ MODKEY,              XK_apostrophe,	   spawn,		   {.v = (const char*[]){ "mpc", "seek", "0%", NULL } } },
	{ MODKEY,              XK_numbersign,	   spawn,		   {.v = (const char*[]){ "mpc", "repeat", NULL } } },
	{ MODKEY|ShiftMask,             XK_p,	   spawn,          {.v = (const char*[]){ "mpc", "pause", NULL } } },
	{ MODKEY|ShiftMask,             XK_o,	   spawn,          {.v = (const char*[]){ "mpc", "play", NULL } } },
    { MODKEY|ShiftMask,             XK_l,	   spawn,		   {.v = (const char*[]){ "mpc", "prev", NULL } } },
	{ MODKEY|ShiftMask,     XK_semicolon,	   spawn,		   {.v = (const char*[]){ "mpc",  "next", NULL } } },
	{ MODKEY|ShiftMask,    XK_numbersign,	   spawn,		   {.v = (const char*[]){ "./.scripts/dmenuunicode", NULL } } },
	{ MODKEY|ShiftMask,           XK_F12,	   spawn,		   {.v = (const char*[]){ "./.scripts/text", NULL } } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

