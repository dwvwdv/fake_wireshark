#include "data_package.h"
#include <QMetaType>

data_package::data_package()
{
    qRegisterMetaType<data_package>("data_package");
    this->timeStamp = "";
    this->data_length = 0;
    this->package_type = 0;
}


void data_package::setInfo(QString info){
    this->info = info;
}

void data_package::setPointer(const u_char *pkt_content,int size){
    this->pkt_content = pkt_content;
    //保存這塊記憶體，因為之後還要動態解析數據
    memcpy((char *)(this->pkt_content),pkt_content,size);
}

void data_package::setDataLength(u_int dt_len){
    this->data_length = dt_len;
}

void data_package::setPackageType(int pt){
    this->package_type = pt;
}

void data_package::setTimeStamp(QString time_stamp){
    this->timeStamp = time_stamp;
}

QString data_package::getDataLength(){
    return QString::number(this->data_length);
}

QString data_package::getInfo(){
    return this->info;
}

QString data_package::getPackageType(){
    switch(this->package_type){
        case 1: return "TCP";
        case 2: return "UDP";
        case 3: return "SSL";
        case 4: return "TLS";
        case 5: return "ARP";
        case 6: return "ICMP";
        case 7: return "DNS";
    }

    return QString::number(this->package_type);
}

QString data_package::getTimeStamp(){
    return this->timeStamp;
}

QString data_package::byteToString(char *str,int size){
    QString res = "";
    for(int i=0;i<size;++i){
        char one = str[i] >> 4;
        if(one >= 0x0A)
            one += 0x41 - 0x0A;
        else
            one += 0x30;
        char two = str[i] & 0xF;
        if(two >= 0x0A)
            two += 0x41 - 0x0A;
        else
            two += 0x30;
        res.append(one);
        res.append(two);
    }
    return res;
}
