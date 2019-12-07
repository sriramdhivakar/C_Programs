#ifndef _SRI_CLOCK_H_
#define _SRI_CLOCK_H_


#define FILE_SIZE	6
#define DAY_CMD         7
#define MONTH_CMD       5
#define YEAR_CMD        9
#define HOURS_CMD       4
#define MINUTES_CMD     2
#define SECONDS_CMD     0
#define READ_FROM_CLOCK(cmd,data) outb(cmd,0x70);data=inb(0x71);
#define WRITE_TO_CLOCK(cmd,data)  outb(cmd,0x70);outb(data,0x71);data = inb(0x71);

 #endif
