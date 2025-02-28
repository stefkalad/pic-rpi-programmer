/*
 * Copyright (C) 2005-2020 Darron Broad
 * All rights reserved.
 *
 * This file is part of Pickle Microchip PIC ICSP.
 *
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#include "VERSION"

#include "pickle.h"

/*
 * pickle session
 */
struct pickle p = {0};

/*
 * pickle help
 */
static void
usage_pickle(void)
{
	printf("USAGE: pickle\n");
	printf("Usage information.\n\n");

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("FRONTENDS:\n"
		" INHX32\n"
		"\t\tFile or standard input.\n"
#ifdef STK500
		" STK500\n"
		"\t\tPOSIX serial or network input.\n"
#endif
		"\n");

	printf("BACKENDS:\n");
#ifdef ALLWINNER
	printf(" BPI/OPI\n"
		"\t\tLinux AllWinner Banana Pi (A20) and Orange Pi (H2+/H3) GPIO.\n");
#endif
#ifdef CP2104
	printf(" CP2104\n"
		"\t\tLinux CP2104 GPIO.\n");
#endif
#ifdef FTDI
	printf(" FTDI\n"
		"\t\tLinux FTDI USB bit-bang.\n");
#endif
#ifdef BITBANG
	printf(" GPIO-BB\n"
		"\t\tLinux GPIO bit-bang version %d.%d.\n",
		GPIO_BB_VERSION_MAJ, GPIO_BB_VERSION_MIN);
#endif
#ifdef MCP2221
	printf(" MCP2221\n"
		"\t\tLinux MCP2221 GPIO.\n");
#endif
#ifdef MCP23016
	printf(" MCP23016\n"
		"\t\tLinux MCP23016 I2C.\n");
#endif
#ifdef MCP230XX
	printf(" MCP230XX\n"
		"\t\tLinux MCP23008/17 I2C.\n");
#endif
#ifdef MCP23SXX
	printf(" MCP23SXX\n"
		"\t\tLinux MCP23S08/17 SPI.\n");
#endif
#ifdef PCF8574
	printf(" PCF8574\n"
		"\t\tLinux PCF8574 I2C.\n");
#endif
#ifdef RPI
	printf(" RPI\n"
		"\t\tLinux Raspberry Pi GPIO (Pi0/Pi1/Pi2/Pi3/Pi4).\n");
#endif
#ifdef SYSFSGPIO
	printf(" SYSFSGPIO\n"
		"\t\tLinux SysFs GPIO.\n");
#endif
#ifdef SERIAL
	printf(" TTY\n"
		"\t\tPOSIX Serial bit-bang.\n");
#endif
	printf("\n");

	printf("EXAMPLES:\n"
#ifdef P12
		" p12 SELECT DEVICE OPERATION [ARG]\n"
		"\t\t12-bit word PIC10F/12F/16F operations.\n"
#endif
#ifdef P14
		" p14 [SELECT DEVICE] [LVP] OPERATION [ARG]\n"
		"\t\t14-bit word PIC10F/12F/16F operations.\n"
#endif
#ifdef N14
		" n14 [LVP] OPERATION [ARG]\n"
		"\t\t14-bit word PIC16F operations.\n"
#endif
#ifdef P16
		" p16 [SELECT DEVICE] [LVP|HVP] OPERATION [ARG]\n"
		"\t\t16-bit word PIC18F operations.\n"
#endif
#ifdef N16
		" n16 [LVP] OPERATION [ARG]\n"
		"\t\t16-bit word PIC18F operations.\n"
#endif
#ifdef P24
		" p24 [SELECT|PARTITION DEVICE] [LVP|HVP] OPERATION [ARG]\n"
		"\t\t24-bit word PIC24/dsPIC operations.\n"
#endif
#ifdef P32
		" p32 OPERATION [ARG]\n"
		"\t\t32-bit word PIC32 operations.\n"
#endif
#ifdef PCTRL
		" pctrl RUN|STOP|RESTORE\n"
		"\t\tControl master clear.\n"
#endif
#ifdef PIO
		" pio COMMAND [ARG]\n"
		"\t\tICSPIO operations.\n"
#endif
#ifdef STK500
		" pload PROGRAM|VERIFY TTY|IP FILE [16|24|32]\n"
		"\t\tProgram or verify file on TTY or network.\n"
#endif
#ifdef PTEST
		" ptest TEST [ARG]\n"
		"\t\tHardware tests.\n"
#endif
		"\n");

	printf("VERSION:\n %s\n", VERSION);

	io_exit(EX_OK);
}

/*
 * p12 help
 */
#ifdef P12
static void
usage_p12(char *msg)
{
	printf("USAGE: p12 SELECT DEVICE OPERATION [ARG]\n");
	printf("12-bit word PIC10F/12F/16F operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" p12 %ss%select\n"
		"\t\tShow supported devices.\n", UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sb%slank\n"
		"\t\tBlank device (disable protection and bulk erase).\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sda%sta\n"
		"\t\tDisplay data flash content.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %so%ssccal\n"
		"\t\tDisplay oscillator calibration.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %so%ssccal 0x0c1a\n"
		"\t\tRestore oscillator calibration as 0x0c1a.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %svi%sew [file.hex] [raw]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
#ifdef STK500
	printf(" p12 %ss%select PIC1XFXXX /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
#endif
	printf("\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * p14 help
 */
#ifdef P14
static void
usage_p14(char *msg)
{
	printf("USAGE: p14 [SELECT DEVICE] [LVP] OPERATION [ARG]\n");
	printf("14-bit word PIC10F/12F/16F operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" p14 %ss%select\n"
		"\t\tShow supported devices.\n", UL_ON, UL_OFF);
	printf(" p14 %ss%select 16F84 OPERATION [ARG]\n"
		"\t\tSelect device PIC16F84.\n", UL_ON, UL_OFF);
	printf(" p14 %sl%svp OPERATION [ARG]\n"
		"\t\tLVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" p14 %sb%slank\n"
		"\t\tBlank device (disable protection and bulk erase).\n", UL_ON, UL_OFF);
	printf(" p14 %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF);
	printf(" p14 %sc%sonfig 0x3000\n"
		"\t\tRestore band-gap configuration as 0x3000.\n", UL_ON, UL_OFF);
	printf(" p14 %sda%sta\n"
		"\t\tDisplay data EEPROM content.\n", UL_ON, UL_OFF);
	printf(" p14 %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p14 %ser%sase eeprom | flash | id | row [n]\n"
		"\t\tErase EEPROM, flash, id or flash at row for n rows.\n", UL_ON, UL_OFF);
	printf(" p14 %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF);
	printf(" p14 %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF);
#ifdef LOADER
	printf(" p14 %slo%sader\n"
		"\t\tOutput boot loader configuration.\n", UL_ON, UL_OFF);
#endif
	printf(" p14 %so%ssccal\n"
		"\t\tDisplay oscillator calibration.\n", UL_ON, UL_OFF);
	printf(" p14 %so%ssccal 0x343c\n"
		"\t\tRestore oscillator calibration as 0x343c.\n", UL_ON, UL_OFF);
	printf(" p14 %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p14 %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p14 %svi%sew [file.hex] [raw]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF);
#ifdef STK500
	printf(" p14 /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF);
#endif
	printf("\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * n14 help
 */
#ifdef N14
static void
usage_n14(char *msg)
{
	printf("USAGE: n14 [LVP] OPERATION [ARG]\n");
	printf("14-bit PIC16F operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" n14 %ss%select\n"
		"\t\tShow supported devices.\n", UL_ON, UL_OFF);
	printf(" n14 %sl%svp OPERATION [ARG]\n"
		"\t\tLVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" n14 %sb%slank\n"
		"\t\tBlank device (disable protection and bulk erase).\n", UL_ON, UL_OFF);
	printf(" n14 %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF);
	printf(" n14 %sda%sta\n"
		"\t\tDisplay data EEPROM content.\n", UL_ON, UL_OFF);
	printf(" n14 %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF);
#if 0
	printf(" n14 %ser%sase eeprom | flash | id | row [n]\n"
		"\t\tErase EEPROM, flash, id or flash at row for n rows.\n", UL_ON, UL_OFF);
#endif
	printf(" n14 %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF);
	printf(" n14 %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF);
#ifdef LOADER
	printf(" n14 %slo%sader\n"
		"\t\tOutput boot loader configuration.\n", UL_ON, UL_OFF);
#endif
	printf(" n14 %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" n14 %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" n14 %svi%sew [file.hex] [raw]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF);
#ifdef STK500
	printf(" n14 /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF);
#endif
	printf("\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * p16 help
 */
#ifdef P16
static void
usage_p16(char *msg)
{
	printf("USAGE: p16 [SELECT DEVICE] [LVP|HVP] OPERATION [ARG]\n");
	printf("16-bit word PIC18F operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" p16 %ss%select\n"
		"\t\tShow supported devices.\n", UL_ON, UL_OFF);
	printf(" p16 %ss%select 18LF2539 OPERATION [ARG]\n"
		"\t\tSelect device PIC18LF2539.\n", UL_ON, UL_OFF);
	printf(" p16 %sl%svp OPERATION [ARG]\n"
		"\t\tLVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" p16 %sh%svp OPERATION [ARG]\n"
		"\t\tHVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" p16 %sb%slank\n"
		"\t\tBlank device (disable protection and bulk erase).\n", UL_ON, UL_OFF);
	printf(" p16 %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF);
	printf(" p16 %sda%sta\n"
		"\t\tDisplay data EEPROM content.\n", UL_ON, UL_OFF);
	printf(" p16 %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p16 %ser%sase eeprom | flash | id | row [n]\n"
		"\t\tErase EEPROM, flash, id or flash at row for n rows.\n", UL_ON, UL_OFF);
	printf(" p16 %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF);
	printf(" p16 %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF);
#ifdef LOADER
	printf(" p16 %slo%sader\n"
		"\t\tOutput boot loader configuration.\n", UL_ON, UL_OFF);
#endif
	printf(" p16 %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p16 %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p16 %svi%sew [file.hex] [raw]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF);
#ifdef STK500
	printf(" p16 /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF);
#endif
	printf("\n");
	
	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * n16 help
 */
#ifdef N16
static void
usage_n16(char *msg)
{
	printf("USAGE: n16 [LVP] OPERATION [ARG]\n");
	printf("16-bit PIC18F operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" n16 %ss%select\n"
		"\t\tShow supported devices.\n", UL_ON, UL_OFF);
	printf(" n16 %sl%svp OPERATION [ARG]\n"
		"\t\tLVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" n16 %sb%slank\n"
		"\t\tBlank device (disable protection and bulk erase).\n", UL_ON, UL_OFF);
	printf(" n16 %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF);
	printf(" n16 %sda%sta\n"
		"\t\tDisplay data EEPROM content.\n", UL_ON, UL_OFF);
	printf(" n16 %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF);
#if 0
	printf(" n14 %ser%sase eeprom | flash | id | row [n]\n"
		"\t\tErase EEPROM, flash, id or flash at row for n rows.\n", UL_ON, UL_OFF);
#endif
	printf(" n16 %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF);
	printf(" n16 %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF);
#ifdef LOADER
	printf(" n16 %slo%sader\n"
		"\t\tOutput boot loader configuration.\n", UL_ON, UL_OFF);
#endif
	printf(" n16 %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" n16 %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" n16 %svi%sew [file.hex] [raw]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF);
#ifdef STK500
	printf(" n16 /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF);
#endif
	printf("\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * p24 help
 */
#ifdef P24
static void
usage_p24(char *msg)
{
	printf("USAGE: p24 [SELECT|PARTITION DEVICE] [LVP|HVP] OPERATION [ARG]\n");
	printf("24-bit word PIC24/dsPIC operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" p24 %ss%select\n"
		"\t\tShow supported devices.\n", UL_ON, UL_OFF);
	printf(" p24 %ss%select 33EP128GP502 OPERATION [ARG]\n"
		"\t\tSelect device dsPIC33EP128GP502.\n", UL_ON, UL_OFF);
	printf(" p24 %ss%select 24FJ128GB202 OPERATION [ARG]\n"
		"\t\tSelect device PIC24FJ128GB202.\n", UL_ON, UL_OFF);
	printf(" p24 %spa%srtition 33EP64GS502 OPERATION [ARG]\n"
		"\t\tSelect and partition device dsPIC33EP64GS502.\n", UL_ON, UL_OFF);
	printf(" p24 %sl%svp OPERATION [ARG]\n"
		"\t\tLVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" p24 %sh%svp OPERATION [ARG]\n"
		"\t\tHVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" p24 %sb%slank\n"
		"\t\tBlank device (disable protection and bulk erase).\n", UL_ON, UL_OFF);
	printf(" p24 %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF);
	printf(" p24 %sda%sta\n"
		"\t\tDisplay data EEPROM content.\n", UL_ON, UL_OFF);
	printf(" p24 %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p24 %sex%sec [n] [address]\n"
		"\t\tDisplay all or n words of executive flash content from address.\n", UL_ON, UL_OFF);
	printf(" p24 %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF);
	printf(" p24 %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF);
#ifdef LOADER
	printf(" p24 %slo%sader\n"
		"\t\tOutput boot loader configuration.\n", UL_ON, UL_OFF);
#endif
	printf(" p24 %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p24 %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p24 %svi%sew [file.hex] [raw]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF);
#ifdef STK500
	printf(" p24 /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF);
#endif
	printf("\n");
	
	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * p32 help
 */
#ifdef P32
static void
usage_p32(char *msg)
{
	printf("USAGE: p32 OPERATION [ARG]\n");
	printf("32-bit word PIC32 operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" p32 %ss%select\n"
		"\t\tShow supported devices.\n", UL_ON, UL_OFF);
	printf(" p32 %sbo%sot [n] [address]\n"
		"\t\tDisplay all or n words of boot flash content from address.\n", UL_ON, UL_OFF);
	printf(" p32 %sb%slank\n"
		"\t\tBlank device (erase).\n", UL_ON, UL_OFF);
	printf(" p32 %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF);
	printf(" p32 %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p32 %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF);
	printf(" p32 %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF);
	printf(" p32 %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p32 %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p32 %svi%sew [file.hex] [raw]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF);
#ifdef STK500
	printf(" p32 /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF);
#endif
	printf("\n");
	
	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * p12/p14/p16/p24/p32 help
 */
static void
usage(char *execname, char *msg)
{
#ifdef P12
	if (strcmp("p12", execname) == 0)
		usage_p12(msg);
#endif
#ifdef P14
	if (strcmp("p14", execname) == 0)
		usage_p14(msg);
#endif
#ifdef N14
	if (strcmp("n14", execname) == 0)
		usage_n14(msg);
#endif
#ifdef P16
	if (strcmp("p16", execname) == 0)
		usage_p16(msg);
#endif
#ifdef N16
	if (strcmp("n16", execname) == 0)
		usage_n16(msg);
#endif
#ifdef P24
	if (strcmp("p24", execname) == 0)
		usage_p24(msg);
#endif
#ifdef P32
	if (strcmp("p32", execname) == 0)
		usage_p32(msg);
#endif
	usage_pickle();
}

/*
 * Are you sure?
 *
 * Confirm blanking or erasing
 */
static int
areyousure(const char *s)
{
	int c;

	printf("%s: are you sure [y/N]? ", s);

	c = fgetc(stdin);
	if (c == 'y' || c == 'Y')
		return 1;

	return 0;
}

/*
 * Reset uid if setuid
 */
static void
resetuid(void)
{
	if (getuid() != geteuid()) {
		if (setuid(getuid()) < 0) {
			printf("%s: fatal error: setuid failed\n", __func__);
			io_exit(EX_OSERR); /* Panic */
		}
	}
}

/*
 * Open device and perform command
 */
int
main(int argc, char **argv)
{
	char *execdup, *execname;
	int rc = EX_OK;

	/* Get exec name */
	execdup = (char *)strdup(argv[0]);
	if (execdup == NULL) {
		printf("%s: fatal error: strdup failed\n", __func__);
		io_exit(EX_OSERR); /* Panic */
	}
	execname = basename(execdup);
	if (execname == NULL) {
		printf("%s: fatal error: basename failed\n", __func__);
		io_exit(EX_OSERR); /* Panic */
	}

	/* Get configuration */
	getconf();
	
	/* Determine back-end */
	if (io_backend() == 0)
		usage(execname, io_error());

	/* Raise priority */
	setpriority(PRIO_PROCESS, 0, -20);

	/*
	 * Open device
	 */

	if (p.io->uid)
		resetuid();
	if (io_open() < 0)
		usage(execname, io_error());
	if (!p.io->uid)
		resetuid();
	
	/* Determine arch: 12 | 14 | 16 | 24 | 32 */
	if (pic_arch(execname) == 0)
		usage_pickle();

	/* Check args */
	if (argc < 2)
		usage(execname, "Missing arg(s)");

	/*
	 * Get args & perform operation
	 */

	/* Device selection and partition */
	int argv1 = tolower((int)argv[1][0]);
	int argv11 = tolower((int)argv[1][1]);
	if (argv1 == 's' || (argv1 == 'p' && argv11 == 'a')) {
		/* Select or partition device */
		if (argc < 3) {
			pic_selector();
			io_exit(EX_OK);
		}
		p.partition = (argv1 == 'p') ? TRUE : FALSE;
		if (mystrcasestr(argv[2], "dspic") == argv[2]) {
			strncpy(p.devicename, argv[2], STRMAX);
		} else if (mystrcasestr(argv[2], "pic") == argv[2]) {
			strncpy(p.devicename, argv[2], STRMAX);
		} else {
			int32_t temp = strtol(argv[2], NULL, 0);
			if (temp < 10 || temp > 33) {
				usage(execname, "Invalid arg [select]");
			}
			if (temp == 30 || temp == 33) {
				strcpy(p.devicename, "dspic");
				strncpy(&p.devicename[5], argv[2], STRMAX - 5);
			} else {
				strcpy(p.devicename, "pic");
				strncpy(&p.devicename[3], argv[2], STRMAX - 3);
			}
		}
		argc -= 2;
		argv += 2;
		if (argc < 2)
			usage(execname, "Missing arg(s)");
	} else if (p.pic->arch == ARCH12BIT) {
		usage(execname, "Missing select");
	}

	/* Key entry or loader output */
	argv1 = tolower((int)argv[1][0]);
	if (argv1 == 'l') {			/* LVP 32-bit key entry or loader output */
#ifdef LOADER
		if (argv11 == 'o') {		/* LOADER OUTPUT */
			if (argc > 2)
				usage(execname, "Too many args [loader]");
			pic_bootloader();
			io_exit(EX_OK);
		}
#endif
		if (p.pic->arch == ARCH12BIT) {
			/* NOT SUPPORTED */
			usage(execname, "Invalid arg [lvp]");
		}
		/* ARCH14BIT || ARCH16BIT || ARCH24BIT || ARCH32BIT */
		p.key = LVPKEY;
		argc -= 1;
		argv += 1;
		if (argc < 2)
			usage(execname, "Missing arg(s)");
	}
	else if (argv1 == 'h') {		/* HVP 32-bit key entry */
		if (p.pic->arch == ARCH12BIT || p.pic->arch == ARCH14BIT || p.pic->arch == ARCH32BIT) {
			/* NOT SUPPORTED */
			usage(execname, "Invalid arg [hvp]");
		}
		/* ARCH16BIT || ARCH24BIT */
		p.key = HVPKEY;
		argc -= 1;
		argv += 1;
		if (argc < 2)
			usage(execname, "Missing arg(s)");
	}
	else if (p.pic->arch == ARCH32BIT) {	/* LVP 32-bit key entry */
		/* ARCH32BIT */
		p.key = LVPKEY;
	}
	else {					/* No key entry */
		/* ARCH12BIT || ARCH14BIT || ARCH16BIT || ARCH24BIT */
		p.key = NOKEY;
	}

	/* Command */
	argv1 = tolower((int)argv[1][0]);
	argv11 = tolower((int)argv[1][1]);
	switch (argv1) {
	case 'b':	if (argv11 == 'o') {		/* BOOT */
				uint32_t addr = UINT32_MAX, words = UINT32_MAX;
				if (argc > 4)
					usage(execname, "Too many args [boot]");
				if (argc >= 3) {
					words = strtoul(argv[2], NULL, 0);
					if (words == 0)
						usage(execname, "Invalid arg [boot]");
				}
				if (argc == 4) {
					addr = strtoul(argv[3], NULL, 0);
				}
				pic_dumpboot(addr, words);
			} else {			/* BLANK */
				int config = 1;
				if (argc > 3)
					usage(execname, "Too many args [blank]");
				if (argc == 3) switch (argv[2][0]) {
					case 'n':
					case 'N':
					case '0': config = 0;
						break;
					case 'y':
					case 'Y':
					case '1': config = 1;
						break;
					default:usage(execname, "invalid arg [blank]");
						break;
				}
				pic_blank(config);
			}
			break;

	case 'c':	if (argc > 3)
				usage(execname, "Too many args [config]");
			if (argc == 2)
				pic_dumpconfig();
			else
				pic_writebandgap(strtoul(argv[2], NULL, 0));
			break;
	
	case 'd':	if (argv11 == 'a') {		/* DATA */
				if (argc > 2)
					usage(execname, "Too many args [data]");
				pic_dumpdata();
			} else if (argv11 == 'e') {	/* DEBUG */
				pic_debug();
			} else {			/* DUMP */
				if (argc > 2)
					usage(execname, "Too many args [dump]");
				pic_dumpdevice();
			}
			break;

	case 'e':	if (argv11 == 'r') {		/* ERASE FLASH | ID | ROW[NROWS] */
				uint32_t row = 0, nrows = 1;
				char prompt[STRLEN] = {0};
				char *endptr = NULL;

				if (argc < 3)
					usage(execname, "Missing arg [erase]");
				if (argc > 4)
					usage(execname, "Too many args [erase]");
				
				int argv2 = tolower((int)argv[2][0]);
				switch (argv2) {
				case 'i': /* IDLOCATION    */
				case 'u': /* USERID/CONFIG */
					row = PIC_ERASE_ID;
					strncpy(prompt, "Erase id", STRMAX);
					break;
				case 'c': /* CONFIG */
					row = PIC_ERASE_CONFIG;
					strncpy(prompt, "Erase config", STRMAX);
					break;
				case 'e': /* EEPROM */
					row = PIC_ERASE_EEPROM;
					strncpy(prompt, "Erase EEPROM", STRMAX);
					break;
				case 'f': /* FLASH */
					nrows = UINT32_MAX;
					strncpy(prompt, "Erase program flash", STRMAX);
					break;
				default:  /* FLASH ROW */
					{
					row = strtoul(argv[2], &endptr, 0);
					if (endptr == argv[2])
						usage(execname, "Invalid arg [erase]");
					if (argc == 4) {
						nrows = strtoul(argv[3], NULL, 0);
						if (nrows == 0)
							usage(execname, "Invalid arg [erase]");
					}
					int _rc = snprintf(prompt, STRLEN, "Erase %u row(s) at row %u", nrows, row);
					if (_rc < 0) {
						printf("%s: fatal error: snprintf failed\n", __func__);
						io_exit(EX_OSERR); /* Panic */
					} else if (_rc >= STRLEN) {
						printf("%s: fatal error: snprintf overrun\n", __func__);
						io_exit(EX_SOFTWARE); /* Panic */
					}
					}
					break;
				}
				if (areyousure(prompt))
					pic_erase(row, nrows);
			} else if (argv11 == 'x') {	/* EXECUTIVE */
				uint32_t addr = UINT32_MAX, words = UINT32_MAX;
				if (argc > 4)
					usage(execname, "Too many args [executive]");
				if (argc >= 3) {
					words = strtoul(argv[2], NULL, 0);
					if (words == 0)
						usage(execname, "Invalid arg [executive]");
				}
				if (argc == 4) {
					addr = strtoul(argv[3], NULL, 0);
				}
				pic_dumpexec(addr, words);
			} else {			/* EEPROM */
				if (argc > 2)
					usage(execname, "Too many args [eeprom]");
				pic_dumpdata();
			}
			break;

	case 'f':	{
			uint32_t words = UINT32_MAX, addr = UINT32_MAX;
			if (argc > 4)
				usage(execname, "Too many args [program flash]");
			if (argc >= 3) {
				words = strtoul(argv[2], NULL, 0);
				if (words == 0)
					usage(execname, "Invalid arg [program flash]");
			}
			if (argc == 4) {
				addr = strtoul(argv[3], NULL, 0);
			}
			pic_dumpprogram(addr, words);
			}
			break;

	case 'i':	if (argc > 2)
				usage(execname, "too many args [id]");
			pic_dumpdeviceid();
			break;

	case 'o':	if (argc > 3)
				usage(execname, "Too many args [osccal]");
			if (argc == 2)
				pic_dumposccal();
			else
				pic_writeosccal(strtoul(argv[2], NULL, 0));
			break;

	case 'p':	{
			int blank = 1;
			if (argc > 4)
				usage(execname, "Too many args [program]");
			if (argc == 4) switch (argv[3][0]) {
				case 'n':
				case 'N':
				case '0': blank = 0;
					break;
				case 'y':
				case 'Y':
				case '1': blank = 1;
					break;
				default:usage(execname, "invalid arg [program]");
					break;
			}
			if (argc < 3)
				pic_program("-", 1);
			else
				pic_program(argv[2], blank);
			}
			break;

	case 'v':	if (argv11 == 'i') {		/* VIEW */
				int raw = 0;
				if (argc > 4)
					usage(execname, "Too many args [view]");
				if (argc == 4) switch (argv[3][0]) {
					case 'r':
					case 'R': raw = 1;
						break;
					default:usage(execname, "invalid arg [view]");
						break;
				}
				if (argc < 3)
					pic_view("-", 0);
				else
					pic_view(argv[2], raw);
			} else {			/* VERIFY */
				if (argc > 3)
					usage(execname, "Too many args [verify]");
				if (argc < 3)
					rc = 0 - pic_verify("-");
				else
					rc = 0 - pic_verify(argv[2]);
			}
			break;
#ifdef STK500
	case '/':	if (strstr(argv[1], p.device) != NULL) {
				usage(execname, "Device in use [TTY]");
			}
			stk500v2_listen(argv[1], 0);
			break;

	case '8':	stk500v2_listen("0.0.0.0", 8048);
			break;
#endif
	default:	usage(execname, "Unknown operation");
			break;
	}

	free(execdup);
	io_exit(rc);
}
