#include "mult_thread.h"
#include <QDebug>

mult_thread::mult_thread()
{
    this->isDone = true;

}


bool mult_thread::setPointer(pcap_t *pcap_t_ptr){
    if(pcap_t_ptr == nullptr)
        return false;
    this->pcap_t_pointer = pcap_t_ptr;
    return true;
}

void mult_thread::setFlag(){
    this->isDone = false;
}

void mult_thread::resetFlag(){
    this->isDone = true;
}

void mult_thread::run(){
    while(true){
        if(isDone)
            return;
        else{
            int res = pcap_next_ex(pcap_t_pointer,&header,&pkt_data); //封包處理函數
            if(res == 0)    //如果是0，這個封包是無效的
                continue;
            local_time_sec = header->ts.tv_sec;
            localtime_s(&local_time, &local_time_sec);
            strftime(timeString,sizeof(timeString),"%H:%M:%S",&local_time);
            qDebug() << timeString ;
        }
    }
}
