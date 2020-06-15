#ifndef _LOGGER
#define _LOGGER

extern int console_flag_out;

// enable console log
void console_log_enable();

// disable console log
void console_log_disable();

//self explanatory
int is_console_log_enable();

//função de log
void log_to_console(const char *type, const char *msg, int id, int *line);

#endif