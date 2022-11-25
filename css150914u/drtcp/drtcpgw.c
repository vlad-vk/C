// coding: cp866
//---------------------------------------------------------------------------
// �롮� ��ப� �� ��६�����
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
// ���樠������ ᮪�⮢
_i TCP_init(void){
   _i z=0;
   // ���樠������ ᮪�� ��� ����ᮢ �� UDP (������)
   WSAStartup(WS_VERSION_REQD, &stWSAData);
   // �ਢ易�� ᮪�� � ����� (� ����䥩��) ��� UDP ������
   for(z=0;z<HSRV;z++){ hSockS[z]=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); }
   // ���㫨�� ��砫�� ����稪�
   for(z=0;z<MWS;z++){ UDRC[z]=0; }
   DPrn(0,0,"00 WinSOCKET initialization.\n");
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �����⨥ ᮪�⮢
_i TCP_close(void){
   _i z=0;
   for(z=0;z<HSRV;z++) if(hSockS[z]){ closesocket(hSockS[z]); WSACleanup(); }
   DPrn(0,0,"00 WinSOCKET closed.\n");
   return(0);
}
//---------------------------------------------------------------------------
