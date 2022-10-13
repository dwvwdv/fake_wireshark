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
            //時間戳整理
            local_time_sec = header->ts.tv_sec;
            localtime_s(&local_time, &local_time_sec);
            strftime(timeString,sizeof(timeString),"%H:%M:%S",&local_time);
//            qDebug() << timeString ;
            QString info;
            int type = ethernetPacketHandle(pkt_data,info);
            if(type){
                data_package data;
                int len = header->len;
                data.setInfo(info);
                data.setDataLength(len);
                data.setTimeStamp(timeString);
                emit send(data);
            }
        }
    }
}

int mult_thread::ethernetPacketHandle(const u_char *pkt_content,QString &info){
    eth_header *ethHeader;
    u_short content_type;
    ethHeader = (eth_header *)(pkt_content);
    //這裡還涉及到網路字節流和主機字節流，所以用ntohs(net to hosts)轉換
    content_type = ntohs(ethHeader->type);
    switch(content_type){
        case 0x0800: {      //ip
            info = "ip";
            return 1;
        }
        case 0x0806:{       //arp
            info = "arp";
            return 1;
        }
    }
    return 0;
}
