#if defined(__linux) || defined(__WIN32)
#include "x86-hal.h"

#include <QDebug>

namespace HAL
{

void print(char *buffer){
    qDebug() << buffer;
}

uint16_t calculateCRC16(char *buffer, int length){
    QByteArray arr(buffer, length);

    return qChecksum(arr, length, Qt::ChecksumIso3309);
}

} // namespace HAL

#endif
