/* cStatBar
 * 
 * A simple statusbar to be used with DWM
 * Gathers different kinds of system info and displays it as the windows title
 * 
 * Requires dwm to have the status2d patch applied and a font that supports icons e.g. Nerdfonts
 *
 * @author: _N0x
 * @version: 0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>

#include "config.h"

/* enums */ 
enum Icon { IconDateTime, IconBattery, IconCPU, IconRAM, IconDisk,
    IconNetSpeed, IconNetSpeedUp, IconNetSpeedDown, IconNetwork, IconMusic };

/* function declaration */
char *concat_string (int count, ...);
char *get_networkspeed ();
char *get_networkinfo ();
char *make_stat_item (enum Icon icon, char *val);
char *read_in_file (char *filename);
void setup ();
void set_xroot (char *title);

/* variables */
static const char *g_if;
static const char KBS[] = "kB/s";
static unsigned int g_ic;
static const char *g_icb;
static const char *g_icf;


/* a variadic function to take in count number of strings and concatenate them into one single string */
char 
*concat_string (int count, ...)
{
    va_list ap;
    va_start(ap, count);
    
    int len = 1;
    for(int i=0; i<count; i++) { len += strlen(va_arg(ap, char*)); }
    va_end(ap);

    char * result = malloc(len * sizeof(char));
    int pos = 0;

    va_start(ap, count);
    for(int i=0; i<count; i++){
        char * tmp = va_arg(ap, char *);
        strcpy(result + pos, tmp);
        pos += strlen(tmp);
    }
    return result;
}


/* Read in the IP from /proc/net/fib_trie for g_if */
char 
*get_networkinfo ()
{
    return NULL;
}


/* get the rx and tx network speed from /sys/ */
char 
*get_networkspeed ()
{
    char *tmp;
    char *r1;
    char *r2;
    char *t1;
    char *t2;
    char tt[12];
    char rr[12];
    
    tmp = concat_string(3, "/sys/class/net/", g_if, "/statistics/rx_bytes");
    r1 = read_in_file(tmp);
    free(tmp);
    
    tmp = concat_string(3, "/sys/class/net/", g_if, "/statistics/tx_bytes");
    t1 = read_in_file(tmp);
    free(tmp);
    
    sleep(1);

    tmp = concat_string(3, "/sys/class/net/", g_if, "/statistics/rx_bytes");
    r2 = read_in_file(tmp);
    free(tmp);
    
    tmp = concat_string(3, "/sys/class/net/", g_if, "/statistics/tx_bytes");
    t2 = read_in_file(tmp);
    free(tmp);

    sprintf(rr, "%d", (atoi(r2) - atoi(r1)) / 1024);
    sprintf(tt, "%d", (atoi(t2) - atoi(t1)) / 1024);

    return concat_string(9, 
            icons[IconNetSpeedUp], " ", tt, KBS, " ", 
            icons[IconNetSpeedDown], " ", rr, KBS);
}


/* creates a formatet string for a status item. Adds color if enabled */
char 
*make_stat_item (enum Icon icon, char *val)
{
    return concat_string(4, " ", 
            ((g_ic) 
                ? concat_string(7 ,"^b", g_icb, "^^c", g_icf, "^ ", icons[icon], " ^d^") 
                : icons[icon]
            ), "  ", val);
}


/* Takes in a filename and returns the content of the file */
char 
*read_in_file (char *fileName)
{
    FILE * fd;
    char * c;
    int len;

    fd = fopen(fileName, "r");
    if (fd==NULL) 
    {
        perror("Could not read in File.\n");
        exit(EXIT_FAILURE);
    }
   
    fseek(fd, 0, SEEK_END);
    len = ftell(fd);
    fseek(fd, 0, SEEK_SET); 
    c = malloc(len);
    fread(c, 1, len, fd);
    
    return c;
}


/* basic setup method for internal variable etc */
void setup ()
{
    g_if = interface;
    g_ic = icon_color;
    g_icb = icon_colorbg;
    g_icf = icon_colorfg;
}


/* Uses functionality from Xlib to set the window root name to a specified string */
void 
set_xroot (char *title)
{
    Display * dpy = XOpenDisplay(NULL);

    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);

    XStoreName(dpy, root, title); 
    XFlush(dpy);

    XCloseDisplay(dpy);
}


int 
main (int argc, char *argv[])
{
    setup();

    while(1) {
        char * ns = make_stat_item(IconNetSpeed, get_networkspeed());

        set_xroot(ns);

        sleep(cycletime);
    }
}
