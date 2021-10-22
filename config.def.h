static const int cycletime      = 2;  /* time between updates in second */ 
static const char interface[]   = "enp4s0"; /* The name of the inteface to display information for */
static const char * icons[]       = { /* Array of icons to be used for the status bar segments */
    "",    // Icon for date
    "",    // Icon for battery
    "",    // Icon for CPU usage
    "",    // Icon for RAM usage
    "",    // Icon for disk
    "龍",   // Icon for network speed
    "",    // Icon for upload speed 
    "",    // Icon for download speed 
    "",    // Icon for network information
    ""     // Icon for music infos 
};

static const unsigned int icon_color    = 1; /* 1 enables colors for icons (needs DWM status2d patch */
static const char icon_colorfg[] = "#eeeeee"; /* The foreground color for icons */
static const char icon_colorbg[] = "#408977"; /* The background color for icons */ 
