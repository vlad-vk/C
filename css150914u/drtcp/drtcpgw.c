// coding: cp866
//---------------------------------------------------------------------------
// выбор строки по переменной
#include <allegro.h>
#include <winalleg.h>
#include <winsock.h>
#include <nb30.h>
#include "../include.h"
#include "../define.h"
#include "../drnbw/drnbw_e.h"
#include "drtcpgw.h"
#include "udpw.h"

_i TCP_ExitSRV=0;

//---------------------------------------------------------------------------
// Инициализация сокетов
_i TCP_init(void){
   _i z=0;
   // Инициализация сокета для запросов по UDP (клиента)
   WSAStartup(WS_VERSION_REQD, &stWSAData);
   // Привязать сокет к адресу (к интерфейсу) для UDP клиента
   for(z=0;z<HSRV;z++){ hSockS[z]=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); }
   // Обнулить начальные счётчики
   for(z=0;z<MWS;z++){ UDRC[z]=0; }
   DPrn(0,0,"00 WinSOCKET initialization.\n");
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Закрытие сокетов
_i TCP_close(void){
   _i z=0;
   for(z=0;z<HSRV;z++) if(hSockS[z]){ closesocket(hSockS[z]); WSACleanup(); }
   DPrn(0,0,"00 WinSOCKET closed.\n");
   return(0);
}
//---------------------------------------------------------------------------
