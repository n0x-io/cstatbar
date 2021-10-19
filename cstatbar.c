/* cStatBar
 * 
 * A simple statusbar to be used with DWM
 * Gathers different kinds of system info and displays it as the windows title
 * 
 * Requires dwm to have the status2d patch applied and a font that supports icons e.g. Nerdfonts
 *
 * author: _N0x
 * version: 0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>

#include "config.h"

/* enums */ 
enum { IconDateTime, IconBattery, IconCPU, IconRAM, IconDisk,
       IconNetSpeed, IconNetSpeedUp, IconNetSpeedDown, IconNetwork, IconMusic };

/* function declaration */
void setup();
void setxroot(char *title);
char *readinfile(char *filename);
char *getnetworkspeed();
char *cs(int count, ...);

/* variables */
static const char *iface;
static const char kbs[] = "kb/s";


/* Uses functionality from Xlib to set the window root name to a specified string */
void setxroot(char *title)
{
    Display * dpy = XOpenDisplay(NULL);

    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);

    XStoreName(dpy, root, title); 
    XFlush(dpy);

    XCloseDisplay(dpy);
}


/* Takes in a filename and returns the content of the file */
char * readinfile(char *fileName)
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


char * getnetworkspeed()
{
    char *tmp,*result,*R1,*R2,*T1,*T2;
    char TT[12], RR[12];
    
    tmp = cs(3, "/sys/class/net/", iface, "/statistics/rx_bytes");
    R1 = readinfile(tmp);
    free(tmp);
    
    tmp = cs(3, "/sys/class/net/", iface, "/statistics/tx_bytes");
    T1 = readinfile(tmp);
    free(tmp);
    
    sleep(1);

    tmp = cs(3, "/sys/class/net/", iface, "/statistics/rx_bytes");
    R2 = readinfile(tmp);
    free(tmp);
    
    tmp = cs(3, "/sys/class/net/", iface, "/statistics/tx_bytes");
    T2 = readinfile(tmp);
    free(tmp);

    sprintf(RR, "%d", (atoi(R2) - atoi(R1)) / 1024);
    sprintf(TT, "%d", (atoi(T2) - atoi(T1)) / 1024);

    result = cs(11, icons[IconNetSpeed], " ", 
            icons[IconNetSpeedUp], " ", TT, kbs, " ", 
            icons[IconNetSpeedDown], " ", RR, kbs);

    return result;
}


/* a variadic function to take in count number of strings and concatenate them into one single string */
char * cs(int count, ...)
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


void setup(){
    iface = interface;
}


int main(int argc, char *argv[])
{
    setup();

    while(1) {
        //setxroot(name); 
        printf("%s\n", getnetworkspeed());

        sleep(cycletime);
    }
}
