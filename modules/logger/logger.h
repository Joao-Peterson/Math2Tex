#ifndef _LOGGER
#define _LOGGER

//extern int console_flag_out; //if declared every file will be able to acess globally

// enable console log
void console_log_enable();

// disable console log
void console_log_disable();

//self explanatory
int is_console_log_enable();

// enable a log file
void enable_log_file();

//função de log
void log_to_console(const char *type, const char *msg, int id, int *line);

#endif