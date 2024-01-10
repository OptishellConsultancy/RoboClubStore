
==================================================
PL-2303 CheckChipVersion Tool Program v1.0.2.0
For Windows OS Only
==================================================

System Requirement:
  . USB 1.1/2.0/3.0 Host Controller
  . Device using PL2303HX/X/EA/RA/SA/TA/TB version chips
  . Device using PL2303GC/GS/GB/GT/GL/GE, PL2323 version chips
  . PL-2303 Windows Driver Installer v1.8.0 or above
  . Supports the following Windows OS Family:
	- Windows 7 (32 & 64 bit)
	- Windows 8/8.1 (32 & 64 bit) 
	- Windows 10 (32 & 64 bit) 
	NOTE: 
	  - Windows 11 is NOT supported in TA/TB chip versions 
	  - Windows 8 is NOT supported in HXA/XA chip versions (will show yellow mark Error Code 10).
	  - Windows 8 is supported only in following chip versions:
	      - PL2303HXD (HX Rev D)
	      - PL2303EA (ESD protection)
	      - PL2303RA (Built-in transceiver)
	      - PL2303SA (SOP8 package)
	      - PL2303TA (PL2303HXA/XA replacement)
	      - PL2303TB (12 GPIO)
	      - PL2303GC (PL2303HXD replacement)
	      - PL2303GB 
	      - PL2303GT (Built-in transceiver)
	      - PL2303GE (ESD protection)
	      - PL2303GS 
 
Supported device ID and product strings:
  . VID_067B&PID_2303 for "Prolific USB-to-Serial Comm Port"
  . VID_067B&PID_2304 for "Prolific USB-to-Serial Comm Port" (PL2303TB chip)
  . VID_067B&PID_23A3 for "Prolific PL2303GC USB Serial COM Port"
  . VID_067B&PID_23B3 for "Prolific PL2303GB USB Serial COM Port"
  . VID_067B&PID_23C3 for "Prolific PL2303GT USB Serial COM Port"
  . VID_067B&PID_23D3 for "Prolific PL2303GL USB Serial COM Port"
  . VID_067B&PID_23E3 for "Prolific PL2303GE USB Serial COM Port"
  . VID_067B&PID_23F3 for "Prolific PL2303GS USB Serial COM Por"
  . VID_067B&PID_23GD for "Prolific PL2320 USB CDC Serial Port"
  . Other VID/PID based on PL2303 chip. 


===========================
How to Run Tool Program
===========================
1. Install PL-2303 Windows Driver Installer v1.8.0 or above.
2. Plug PL2303 USB Device and go to Device Manager to check COM Port number. 
3. Run PL2303CheckChipVersion tool and set COM Port number. 
4. Click Check button to show PL-2303 chip version. 

NOTE: This program cannot detect HXA/XA chip in Windows 8 and above.


Download Latest Driver here: 
http://www.prolific.com.tw/US/ShowProduct.aspx?p_id=225&pcid=41

========================================
Prolific Technology Inc.
http://www.prolific.com.tw


