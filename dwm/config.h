/* See LICENSE file for copyright and license details. */

/* Constants */

#include<X11/XF86keysym.h>

#define TERMINAL "st"
#define TERMCLASS "st-256color"
#define BROWSER "brave"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 15;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Roboto:size=10" };
static const char dmenufont[]       = "Roboto:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
/* my_gruv */
static const char col_red[]         = "#cc241d";
static const char col_black[]       = "#282828";
static const char col_offwhite[]    = "#fafafa";
static const char col_gruvwhite[]   = "#ebdbb2";
static const char col_gruvgrey[]   = "#928374";
static const char col_gruvpurple[]   = "#B16286";
static const char col_gruvorange[]   = "#D65D0E";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gruvwhite, col_black, col_black },
	[SchemeSel]  = { col_gruvwhite, col_gruvpurple,  col_gruvpurple  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                    instance    title           tags mask     isfloating   monitor */
	{ "Gimp",                   NULL,      NULL,            0,            1,           -1 },
	{ "galculator",             NULL,      NULL,            0,            1,           -1 },
	{ "Galculator",             NULL,      NULL,            0,            1,           -1 },
	{ "lxappearance",           NULL,      NULL,            0,            1,           -1 },
	{ "Lxappearance",           NULL,      NULL,            0,            1,           -1 },
	{ "Nitrogen",               NULL,      NULL,            0,            1,           -1 },
	{ "zoom",                   NULL,      NULL,            0,            1,           -1 },
	{ "Firefox",                NULL,      NULL,            1 << 8,       0,           -1 },
	{ TERMCLASS,                NULL,      "pulsemixer",    0,       1,           -1 },
	{ TERMCLASS,                NULL,      "gotop",         0,       1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0;

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[@]",      spiral },/* first entry is default */
	{ "[M]",      monocle },
	{ "H[]",      deck },
	{ "[\\]",     dwindle },
	{ "[]=",      tile },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* personal helpers */

#define BCTL(cmd) {"brightnessctl","set",cmd,NULL}
#define MPC(cmd) {"mpc",cmd,NULL}
#define SS(cmd){"ssmaim",cmd,NULL}

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_gruvpurple, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char *pulsemixercmd[]  = {TERMINAL,"pulsemixer",NULL};
static const char *browsercmd[]  = {BROWSER,NULL};
static const char *thunderbirdcmd[]  = {"thunderbird",NULL};
static const char *instalock[]  = {"timeslock",NULL};
static const char *fileman[]  = {"pcmanfm",NULL};
static const char *touchpad_toggle[] = {"touchpad_toggle",NULL};
static const char *gotopcmd[]  = {TERMINAL,"gotop",NULL};
static const char *musplayer[]  = {TERMINAL,"ncmpcpp",NULL};
static const char *newsboat[]  = {TERMINAL,"newsboat",NULL};
static const char *brupcmd[] =    BCTL("10%+");
static const char *brdowncmd[] =  BCTL("10%-");
static const char *m_toggle[] = MPC("toggle");
static const char *m_next[] = MPC("next");
static const char *m_prev[] = MPC("prev");
static const char *exitmenu[] = {"exitmenu",NULL};
static const char *quickss[] = SS("");
static const char *windowss[] = SS("window");
static const char *windowss_clip[] = SS("windowclip");
static const char *selss[] = SS("sel");
static const char *selss_clip[] = SS("selclip");
static const char *open_menu[]  = {"open_menu",NULL};



static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ControlMask,           XK_m,      spawn,          {.v = pulsemixercmd} },
	{ MODKEY,                       XK_p,      spawn,          {.v = gotopcmd} },
	{ MODKEY,                       XK_F1,     spawn,          {.v = fileman} },
	{ MODKEY,                       XK_F2,     spawn,          {.v = browsercmd} },
	{ MODKEY,                       XK_F5,     spawn,          {.v = musplayer} },
	{ MODKEY,                       XK_F6,     spawn,          {.v = thunderbirdcmd} },
	{ MODKEY|ControlMask,           XK_p,      spawn,          {.v = m_toggle} },
	{ MODKEY|ControlMask,           XK_comma,  spawn,          {.v = m_prev} },
	{ MODKEY|ControlMask,           XK_period, spawn,          {.v = m_next} },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = instalock} },
	{ MODKEY|ControlMask,           XK_n,      spawn,          {.v = newsboat} },
	{ MODKEY|ShiftMask,             XK_x,      spawn,          {.v = exitmenu} },
    { 0,                            XK_Print,  spawn,          {.v = quickss} },
    { MODKEY,                       XK_Print,  spawn,          {.v = windowss} },
    { MODKEY|ShiftMask,             XK_Print,  spawn,          {.v = selss} },
    { MODKEY|ControlMask,           XK_Print,  spawn,          {.v = windowss_clip} },
    { MODKEY|ShiftMask|ControlMask, XK_Print,  spawn,          {.v = selss_clip} },
    { MODKEY,                       XK_c,  spawn,          {.v = quickss} },
    { MODKEY|Mod1Mask,              XK_c,  spawn,          {.v = windowss} },
    { MODKEY|ShiftMask,             XK_c,  spawn,          {.v = selss} },
    { MODKEY|ControlMask,           XK_c,  spawn,          {.v = windowss_clip} },
    { MODKEY|ShiftMask|ControlMask, XK_c,  spawn,          {.v = selss_clip} },
	{ MODKEY,                       XK_o,      spawn,          {.v = open_menu} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,		    XK_semicolon,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_apostrophe, cyclelayout,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask|Mod1Mask,    XK_c,      quit,           {0} },
    { 0, XF86XK_AudioPause,    spawn,                     {.v = m_toggle} },
    { 0, XF86XK_AudioPlay,    spawn,                     {.v = m_toggle} },
    { 0, XF86XK_MonBrightnessUp,    spawn,                     {.v = brupcmd} },
    { 0, XF86XK_MonBrightnessDown,  spawn,          {.v = brdowncmd} },
    { 0, XF86XK_TouchpadToggle,     spawn,          {.v = touchpad_toggle} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
    /*{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },*/
    /*{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },*/
    { ClkLtSymbol,          0,              Button1,        cyclelayout,      {.i = -1} },
    { ClkLtSymbol,          0,              Button3,        cyclelayout,      {.i = +1} },
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

