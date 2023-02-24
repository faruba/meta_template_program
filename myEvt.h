/*
 * =====================================================================================
 *
 *       Filename:  myEvt.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2021年01月29日 20时43分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef MYEVT_H
#define MYEVT_H
#include "event_handler.h"
#include "connection.h"
#include "addon.h"

class MyEvt :public socketor::EventHandler {
  public:
    MyEvt(Addon* addon)
      : socketor::EventHandler(std::string("MyEvt"))
      , mp_addon(addon)
    {
    };
 
    virtual int32_t onRead(socketor::Connection* connection);
    //virtual int32_t onWrite(socketor::Connection* connection);
  private:
    void toObject(uint8_t* buff,agip::PSAGIPHeader header);
    Addon* mp_addon;
};

#endif //MYEVT_H
