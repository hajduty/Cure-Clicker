#pragma once

namespace modify
{
	inline int decreaseBy;							// DECREASE BY
	inline int modifyChance = 50;
	inline bool decreaseInvert = false;
	inline int widthRange = 20;						// HOW MUCH TO ADD OR DECREASE 
	inline int widthMin = 20;						// MIN MS VALUE TO MODIFY
	inline int widthMax = 130;						// MAX MS VALUE TO MODIFY
}

namespace binds
{
	inline int leftBind = { 0 };
	inline int rightBind = { 0 };
	inline int potfillBind = { 0 };
	inline int shiftBind = { 160 };
	inline int hideBind = { 45 };
	inline int throwpotBind = { 0 };
}

namespace vars
{
	inline int clicksTemp[5000];						// TEMP ARRAY TO SHOW GRAPH
	inline int clicks[5000];						// CLICKS
	inline int recordedClicks[5000];				// ALL CURRENT RECORDED CLICKS
	inline int currentClickAmount;					// AMOUNT OF CLICKS IN CURRENT CPS ARRAY
	inline bool recordGame = false;					// RECORD GAME
	inline int crntLeftclick;						// CURRENT CLICK
	inline int crntRightclick;						// CURRENT CLICK
	inline float jrange = 3;						// JITTER RANGE
	inline bool jitter = false;						// JITTER TOGGLE
	inline bool lockL = false;						// LOCK LEFT CLICKER (ALWAYS CLICK)
	inline int loadedClicksAmount;					// AMOUNT OF CLICKS IN LOADED TXT FILE
	inline static char fName[128] = "filename";		// SAVE FILENAME
	inline int sessionClicks;						// CLICKS THIS SESSION
	inline double averageCps;						// AVERAGE CPS
	inline float leftBoost = 1;						// LEFT CPS DIVISOR
	inline POINT pos = { 0, 0 };					// SCREEN POSITION
	inline float rightBoost = 1;					// RIGHT CPS DIVISOR
	inline float blockhit;							// BLOCKHIT CHANCE
	inline bool mcOnly = true;						// MINECRAFT ONLY TOGGLE
	inline bool breakBlock = false;					// BREAK BLOCKS WHILE AUTO CLICKING TOGGLE (Flags on some servers)
	inline bool eat = false;						// EAT FOOD WHILE RIGHT CLICKING TOGGLE (Might flag on some servers)
	inline bool invOnly = true;						// INVENTORY ONLY TOGGLE (true == doesn't work)
	inline bool shiftDis = true;					// DISABLES ALL CLICKING WHILE SHIFTING (PANIC BUTTON)
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
	inline float maxBoost = 10;						// MAX BOOST
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
	inline int msLimitMax = 2000;					// MS LIMIT WHEN RECORDING CLICKS
	inline int msLimitMin = 5;						// MS LIMIT WHEN RECORDING CLICKS
	inline int cps = 0;								// ?
	inline int screen[2] = { 0,0 };
	inline int shuffles = 0;						// SESSION SHUFFLES
	inline bool modify = false;						// MODIFY CLICKS WINDOW
	inline bool watermark = false;					// WATERMARK WINDOW
}

namespace record
{
	inline int time1Total = 1;						// TIME WHEN FIRST CLICK (in milliseconds)
	inline int time2Total = 1;						// TIME WHEN SECOND CLICK (in milliseconds) 
	inline int ms = 0;								// THE TIME BETWEEN FIRST AND SECOND CLICK
	inline int click = 1;							// ?
}