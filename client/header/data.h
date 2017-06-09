#ifndef DATA_H
#define DATA_H
struct LogRec
{
    char log_name[32];
    pid_t pid;
    short type;
    long logTime;
    char logIp[32];
};

struct MatchedLogRec
{
    char logName[32];
    long logInTime;
    long logOutTime;
    long durations;
    char logIp[32];
};

#endif // DATA_H
