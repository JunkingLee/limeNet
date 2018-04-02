//
// Created by tony on 18-3-12.
//

#ifndef LIMENET_MSGFORMAT_HPP
#define LIMENET_MSGFORMAT_HPP
#include <memory.h>
enum exbuffer_endian{
    EXBUFFER_BIG_ENDIAN,
    EXBUFFER_LITTLE_ENDIAN
};

class CMsgFormat {
public:
    CMsgFormat(){
        memset(&m_Header,0,sizeof(m_Header));
        memset(&m_Tail,0,sizeof(m_Tail));
    }
    ~CMsgFormat(){}
    CMsgFormat& operator=(const CMsgFormat& _n){
        endian=_n.endian;
        fixLenBase=_n.fixLenBase;
        msgLen=_n.msgLen;
        varLen=_n.varLen;
        varLenOffset=_n.varLenOffset;
        bHeadUsed=_n.bHeadUsed;
        headType=_n.headType;
        m_Header=_n.m_Header;
        bTailUsed=_n.bTailUsed;
        tailType=_n.tailType;
        m_Tail=_n.m_Tail;

        return *this;
    }
public:
    enum exbuffer_endian endian;
    unsigned short fixLenBase;
    unsigned short msgLen;  //infoLen+fixLenBase
    unsigned short varLenOffset;    //帧长度位置与帧头偏差
    unsigned char varLen;   //len size
    bool bHeadUsed;
    unsigned char headType; //(1,2,4)
    union _Header{
        unsigned char uC;
        unsigned char uS;
        unsigned char uI;
    }m_Header;
    bool bTailUsed;
    unsigned char tailType; //(1,2,4)
    union _Tail{
        unsigned char uC;
        unsigned char uS;
        unsigned char uI;
    }m_Tail;

};


#endif //LIMENET_MSGFORMAT_HPP
