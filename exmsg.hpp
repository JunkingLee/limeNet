//
// Created by tony on 18-3-12.
//

#ifndef LIMENET_EXMSG_HPP
#define LIMENET_EXMSG_HPP

#include <event2/buffer.h>
#include "msgformat.hpp"
class CExMsg{
public:
    CExMsg(){}
    virtual ~CExMsg(){}
public:
    CMsgFormat m_msgFormat;
public:
    static bool NorGetMsgLen(evbuffer* const _pBuf,CMsgFormat const _MsgFormat,unsigned int& _nLen){
        char buf[128];
        evbuffer_copyout(_pBuf,buf,_MsgFormat.fixLenBase);
        if(evbuffer_get_length(_pBuf)<_MsgFormat.fixLenBase)
            return false;
        else{
            _nLen=*((unsigned short *)(buf+_MsgFormat.varLenOffset));
            return true;
        }
    }
};
#endif //LIMENET_EXMSG_HPP
