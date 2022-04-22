#pragma once

namespace vars
{
	inline bool recordGame = false;					// RECORD GAME
	inline int crntClick;							// CURRENT CLICK
	inline float decreaseBy;						// DECREASE BY
	inline float jrange = 3;						// JITTER RANGE
	inline bool jitter = false;						// JITTER TOGGLE
	inline bool lockL = false;						// LOCK LEFT CLICKER (ALWAYS CLICK)
	inline int amountClicks;						// AMOUNT OF CLICKS IN LOADED TXT FILE
	inline bool discord = false;					// RPC
	inline static char fName[128] = "filename";		// SAVE FILENAME
	inline int sessionClicks;						// CLICKS THIS SESSION
	inline double averageCpsL;						// AVERAGE LEFT CPS
	inline double averageCpsR;						// AVERAGE RIGHT CPS
	inline int cpsTemp[5000];						// TEMP ARRAY TO SHOW GRAPH
	inline int loadedClicks[5000];					// LOADED CLICKS
	inline int recordedClicks[5000];				// ALL CURRENT RECORDED CLICKS
	inline float leftBoost = 1;						// LEFT CPS DIVISOR
	inline POINT pos = { 0, 0 };					// SCREEN POSITION
	inline float rightBoost = 1;					// RIGHT CPS DIVISOR
	inline float blockhit;							// BLOCKHIT CHANCE
	inline int leftBind = { 0 };
	inline int rightBind = { 0 };
	inline int potfillBind = { 0 };
	inline int shiftBind = { 160 };
	inline int hideBind = { 45 };
	inline int throwpotBind = { 0 };
	inline bool mcOnly = true;						// MINECRAFT ONLY TOGGLE
	inline bool breakBlock = false;					// BREAK BLOCKS WHILE AUTO CLICKING TOGGLE (Flags on some servers)
	inline bool eat = false;						// EAT FOOD WHILE RIGHT CLICKING TOGGLE (Might flag on some servers)
	inline bool invOnly = true;						// INVENTORY ONLY TOGGLE (true == doesn't work)
	inline bool shiftDis = true;					// DISABLES ALL CLICKING WHILE SHIFTING (PANIC BUTTON)
	inline int potfillMs = 60;						// POTFILL DELAY
	inline int wtapPercent = 0;						// WTAP PERCENTAGE
	inline bool lEnabled = false;					// LEFT CLICKER TOGGLE 	
	inline bool rEnabled = false;					// RIGHT CLICKER TOGGLE
}

namespace destruct
{
	inline bool o1 = true;							// DESTRUCT OPT 1
	inline bool o2 = true;							// DESTRUCT OPT 2
	inline bool o3 = true;							// DESTRUCT OPT 3
	inline bool o4 = false;							// DESTRUCT OPT 4
}

namespace menu
{
	inline float cpsLimit = 20;						// CPS LIMIT (forgot what it do)
	inline float maxBoost = 5;						// MAX BOOST
	inline float maxCps = 16;						// MAX CPS FOR OTHER RAND
	inline float minCps = 10;						// MIN CPS FOR OTHER RAND
	inline int rand = 0;							// RANDOMIZATION OPTION
	inline int style = 1;							// STYLE OPTION
	inline int tab = 0;								// CURRENT MENU TAB
	inline int miscTab = 1;							// ?
	inline bool hide = false;						// WINDOW HIDE TOGGLE
	inline bool saveType = 0;						// FILE SAVE OPTION (1 = APPEND TO EXISTING FILE)
	inline int totalClicks;							// CLICKS RECORDED THIS SESSION
	inline bool graph = false;						// GRAPH TOGGLE
	inline bool des = false;						// DESTRUCT MENU TOGGLE
	inline int click = 1;							// ?
	inline int msLimit = 800;						// MS LIMIT WHEN RECORDING CLICKS
	inline int time1Total = 1;						// TIME WHEN FIRST CLICK (in milliseconds)
	inline int time2Total = 1;						// TIME WHEN SECOND CLICK (in milliseconds) 
	inline int ms = 0;								// THE TIME BETWEEN FIRST AND SECOND CLICK
	inline int cps = 0;								// ?
}