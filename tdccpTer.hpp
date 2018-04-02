//
// Created by tony on 18-3-13.
//

#ifndef LIMENET_TDCCPTER_H
#define LIMENET_TDCCPTER_H

#include "conn.hpp"
#include "msgformat.hpp"
#include "tdccpmsg.hpp"

typedef bool (*TPGetMsgLen)(evbuffer* const _pBuf, CMsgFormat const _MsgFormat, unsigned int& _nLen);

class CTdccpTer: public Conn{
public:
    CTdccpTer(int fd):Conn(fd){
        CTdccpMsg  tdccpMsg;
        m_MsgFormat=tdccpMsg.m_msgFormat;
        m_pGetMsgLen=CTdccpMsg::NorGetMsgLen;
    }
    void ReadMsg(){
        unsigned int len;
        bool result=(*m_pGetMsgLen)(m_ReadBuf,m_MsgFormat,len);
        if(result){
            unsigned int readlen=GetReadBufferLen();
            m_MsgFormat.msgLen=len+m_MsgFormat.fixLenBase;
            if(readlen>=m_MsgFormat.msgLen){
                OnMsg();
            }
        }else{
            printf("get msg len error\n");
        }
    }
    void OnMsg(){
        char buf[1024*100];
        GetReadBuffer(buf,m_MsgFormat.msgLen);
        char *pBuf=buf+9;
        std::string result(pBuf,m_MsgFormat.msgLen-9);
        printf("result=%s",result.c_str());
        SendMsg();
    }
    void SendMsg(){
        char temp[33]="hello world";
        AddToWriteBuffer(temp,strlen(temp));
    }
public:
    CMsgFormat m_MsgFormat;
    TPGetMsgLen m_pGetMsgLen;
};
#endif //LIMENET_TDCCPTER_H
