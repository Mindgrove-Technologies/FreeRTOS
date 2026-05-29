#ifndef LOG_H  
#define LOG_H  

#ifdef __cplusplus
extern "C" {
#endif

// Log Level Macros  
typedef enum {  
  FATAL = 0,  // Ordered by severity  
  ERROR = 1,  
  WARN  = 2,  
  INFO  = 3,  
  TRACE = 4,  
  DEBUG = 5  
} LogLevel;  

// Conditional Compilation Guard  
#if !defined(LOG_LEVEL)  
  #define LOG_LEVEL WARN  // Default level  
#endif  

extern volatile LogLevel log_level;  
void log_emit_function(LogLevel level, const char* fmt, ...);
#define log_emit(level, fmt, ...) \
    do { \
        if ((level) <= log_level) { \
            log_emit_function(level, fmt, ##__VA_ARGS__); \
        } \
    } while (0)
#ifdef __cplusplus
}
#endif

#endif // LOG_H 