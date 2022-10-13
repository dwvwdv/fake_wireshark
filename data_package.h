#ifndef DATA_PACKAGE_H
#define DATA_PACKAGE_H

#include <QString>

#include "format.h"


class data_package
{
private:
    u_int data_length;
    QString timeStamp;
    QString info;
    int package_type;
protected:
    static QString byteToString(char *str,int size);
public:
    data_package();
    const u_char *pkt_content;

    void setDataLength(u_int dt_len);
    void setTimeStamp(QString time_stamp);
    void setPackageType(int pt);
    void setPointer(const u_char *pkt_content,int size);
    void setInfo(QString info);

    QString getDataLength();
    QString getTimeStamp();
    QString getPackageType();
    QString getPointer();
    QString getInfo();
};

#endif // DATA_PACKAGE_H
