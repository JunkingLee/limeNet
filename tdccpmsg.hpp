//
// Created by tony on 18-3-13.
//

#ifndef LIMENET_TDCCPMSG_HPP
#define LIMENET_TDCCPMSG_HPP

#include "exmsg.hpp"
#define MAX_BUF_LEN 256
class CTdccpMsg:public CExMsg{
public:
    CTdccpMsg(){
        m_msgFormat.endian=EXBUFFER_LITTLE_ENDIAN;
        m_msgFormat.fixLenBase=9;
        m_msgFormat.bHeadUsed=true;
        m_msgFormat.headType=1;
        m_msgFormat.m_Header.uC=0x7e;
        m_msgFormat.bTailUsed=false;
        m_msgFormat.varLenOffset=7;
    }
    virtual ~CTdccpMsg(){

    }

    CTdccpMsg(CTdccpMsg* _pMsg){
        m_msgFormat=_pMsg->m_msgFormat;
        BSOI=_pMsg->BSOI;
        BFID=_pMsg->BFID;
        BCID=_pMsg->BCID;
        SSEQ=_pMsg->SSEQ;
        RSEQ=_pMsg->RSEQ;
        INFORLEN=_pMsg->INFORLEN;
        memcpy(m_pData,_pMsg->m_pData,MAX_BUF_LEN);
    }
    CTdccpMsg& operator=(const CTdccpMsg& _n){
        m_msgFormat=_n.m_msgFormat;
        BSOI=_n.BSOI;
        BFID=_n.BFID;
        BCID=_n.BCID;
        SSEQ=_n.SSEQ;
        RSEQ=_n.RSEQ;
        INFORLEN=_n.INFORLEN;
        memcpy(m_pData,_n.m_pData,MAX_BUF_LEN);

        return *this;
    }

public:
    unsigned char BSOI;
    unsigned char BFID;
    unsigned char BCID;
    unsigned short SSEQ;
    unsigned short RSEQ;
    unsigned short INFORLEN;
    unsigned char m_pData[MAX_BUF_LEN];
public:
    bool pack(evbuffer *_pWrite){
        return true;
    }
    bool unpack(evbuffer *_pRead){
        return false;
    }
};
#endif //LIMENET_TDCCPMSG_HPP
