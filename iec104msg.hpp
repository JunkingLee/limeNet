//
// Created by tony on 18-3-13.
//

#ifndef LIMENET_IEC104MSG_HPP
#define LIMENET_IEC104MSG_HPP

#include "exmsg.hpp"

#define MAX_IEC104_ASDU_BUF_LEN 256 //多保留了几位
class CIEC104Msg: public CExMsg{
public:
    CIEC104Msg(){
        m_msgFormat.endian=EXBUFFER_LITTLE_ENDIAN;
        m_msgFormat.fixLenBase=9;
        m_msgFormat.bHeadUsed=true;
        m_msgFormat.headType=1;
        m_msgFormat.m_Header.uC=0x7e;
        m_msgFormat.bTailUsed=false;
        m_msgFormat.varLenOffset=1;
    }
    virtual ~CIEC104Msg(){

    }
    CIEC104Msg(CIEC104Msg* _pMsg){
        m_msgFormat=_pMsg->m_msgFormat;
        m_BSOI=_pMsg->m_BSOI;
        m_BAPDULen=_pMsg->m_BAPDULen;
        m_BAPCICtr1=_pMsg->m_BAPCICtr1;
        m_BAPCICtr2=_pMsg->m_BAPCICtr2;
        m_BAPCICtr3=_pMsg->m_BAPCICtr3;
        m_BAPCICtr4=_pMsg->m_BAPCICtr4;
        m_BASDUCmdType=_pMsg->m_BASDUCmdType;
        m_BASDUVarQua=_pMsg->m_BASDUVarQua;
        m_nASDUCOT=_pMsg->m_nASDUCOT;
        m_nASDUPubAddr=_pMsg->m_nASDUPubAddr;
        memcpy(m_pASDUChData,_pMsg->m_pASDUChData,MAX_IEC104_ASDU_BUF_LEN);
    }
    CIEC104Msg& operator=(CIEC104Msg& _n){
        m_msgFormat=_n.m_msgFormat;
        m_BSOI=_n.m_BSOI;
        m_BAPDULen=_n.m_BAPDULen;
        m_BAPCICtr1=_n.m_BAPCICtr1;
        m_BAPCICtr2=_n.m_BAPCICtr2;
        m_BAPCICtr3=_n.m_BAPCICtr3;
        m_BAPCICtr4=_n.m_BAPCICtr4;
        m_BASDUCmdType=_n.m_BASDUCmdType;
        m_BASDUVarQua=_n.m_BASDUVarQua;
        m_nASDUCOT=_n.m_nASDUCOT;
        m_nASDUPubAddr=_n.m_nASDUPubAddr;
        memcpy(m_pASDUChData,_n.m_pASDUChData,MAX_IEC104_ASDU_BUF_LEN);
        return *this;
    }

public:
    unsigned char   m_BSOI;         //帧启动标记
    unsigned char	m_BAPDULen;     //APDU长度
    unsigned char	m_BAPCICtr1;    //控制域1
    unsigned char	m_BAPCICtr2;    //控制域2
    unsigned char	m_BAPCICtr3;    //控制域3
    unsigned char	m_BAPCICtr4;    //控制域4
    unsigned char	m_BASDUCmdType; //类型标识
    unsigned char	m_BASDUVarQua;  //可变结构限定词
    unsigned int	m_nASDUCOT;     //传送原因
    unsigned int	m_nASDUPubAddr; //公共地址
    unsigned char	m_pASDUChData[MAX_IEC104_ASDU_BUF_LEN];//信息体(地址:数据)
public:
    bool pack(evbuffer *_pWrite){
        return true;
    }
    bool unpack(evbuffer *_pRead){
        return false;
    }
};
#endif //LIMENET_IEC104MSG_HPP
