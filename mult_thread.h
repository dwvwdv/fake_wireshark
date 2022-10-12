#ifndef MULT_THREAD_H
#define MULT_THREAD_H

#include <QThread>
#include "pcap.h"

class mult_thread : public QThread
{
    Q_OBJECT
public:
    mult_thread();
    bool setPointer(pcap_t *);
    void setFlag();
    void resetFlag();
    void run() override;
private:
    pcap_t *pcap_t_pointer;
    struct pcap_pkthdr *header;
    const u_char *pkt_data;
    time_t local_time_sec;
    struct tm local_time;
    char timeString[16];
    bool isDone;
};

#endif // MULT_THREAD_H
