// coding: cp866
//---------------------------------------------------------------------------
// Функции и обмен данными по NetBIOS (IPX/SPX)

#define  LNAM                           16          // длинна имени WS
#define  MNCB                           8           // кол-во NCB
#define  LNCB                           504         // длинна буфера посылки-приема
#define  LNCBMAX                        512         // макс возможная длинна буфера NCB

uc       ResRes=0;                      // флаги сброса адаптеров
uc       ResAdd=0;                      // флаги добавления имен в список сети
uc       ResDel=0;                      // флаги удаления имен из списока сети

uc       ucMaxSess=50;                  // Максимальное количество сеансов NetBIOS
uc       ucMaxName=50;                  // Максимальное количество имен в таблице имен
uc       uFirstName=0;                  // Номер первого устанавливаемого имени

uc       LAN_NUM[MAX_LANA+1];           // номера адаптеров, соответствующие ном серверов
uc       NumNAM [MAX_LANA+1];           // Номера имен в сети для каждого адаптера

struct   LENUM {
 UCHAR   howa;                          // количество доступных адаптеров
 UCHAR   lana  [MAX_LANA+1];            // номера доступных для работы адаптеров (служб)
}        lenum [MAX_LANA+1];            // 

//---------------------------------------------------------------------------
// Комманды для работы с именами
#define WS_NB_WAddName                  0x30
#define WS_NB_AddName                   0xb0
#define WS_NB_WAddGroupName             0x36
#define WS_NB_AddGroupName              0xb6
#define WS_NB_WDeleteName               0x31
#define WS_NB_DeleteName                0xb1

//---------------------------------------------------------------------------
// Комманды для передачи|приема датаграмм
#define WS_NB_WSendDatagram             0x20
#define WS_NB_SendDatagram              0xa0
#define WS_NB_WSendBroadcastDatagram    0x22
#define WS_NB_SendBroadcastDatagram     0xa2
#define WS_NB_WReceiveDatagram          0x21
#define WS_NB_ReceiveDatagram           0xa1
#define WS_NB_WReceiveBroadcastDatagram 0x23
#define WS_NB_ReceiveBroadcastDatagram  0xa3

//---------------------------------------------------------------------------
// Комманды для работы с каналами
#define WS_NB_WCall                     0x10
#define WS_NB_Call                      0x90
#define WS_NB_WListen                   0x11
#define WS_NB_Listen                    0x91
#define WS_NB_WHangUp                   0x12
#define WS_NB_HangUp                    0x92

//---------------------------------------------------------------------------
// Комманды для передачи|приема по каналам
#define WS_NB_WSend                     0x14
#define WS_NB_Send                      0x94
#define WS_NB_WSendNoAck                0x71
#define WS_NB_SendNoAck                 0xf1
#define WS_NB_WChainSend                0x17
#define WS_NB_ChainSend                 0x97
#define WS_NB_WChainSendNoAck           0x72
#define WS_NB_ChainSendNoAck            0xf2
#define WS_NB_WReceive                  0x15
#define WS_NB_Receive                   0x95
#define WS_NB_WReceiveAny               0x16
#define WS_NB_ReceiveAny                0x96

//---------------------------------------------------------------------------
// Дополнительныые комманды
#define WS_NB_WResetAdapter             0x32
#define WS_NB_WCancel                   0x35
#define WS_NB_WSessionStatus            0x34
#define WS_NB_SessionStatus             0xb4

//---------------------------------------------------------------------------
// Смещение полей NCB
#define O_CMD       0x00 /* 1    BYTE: NetBios command code                */
#define O_RET_CODE  0x01 /* 1    BYTE: NetBios return code                 */
#define O_LSN       0x02 /* 1    BYTE: Local session number                */
#define O_NUM       0x03 /* 1    BYTE: Name number                         */
#define O_IO_OFF    0x04 /* 2    WORD: Offset of IO buffer in conv mem     */
#define O_IO_SEG    0x06 /* 2    WORD: Segment of IO buffer in conv mem    */
#define O_LEN       0x08 /* 2    WORD: Length of data in IO buffer         */
#define O_RMT_NAME  0x0A /* 1*16 BYTE[16]: Remote name                     */
#define O_LCL_NAME  0x1A /* 1*16 BYTE[16]: Local name                      */
#define O_REC_TO    0x2A /* 1    BYTE: Receive time out in 0.5 secs        */
#define O_SND_TO    0x2B /* 1    BYTE: Send time out in 0.5 secs           */
#define O_POST_OFF  0x2C /* 2    WORD: Offset of POST routine in conv mem  */
#define O_POST_SEG  0x2E /* 2    WORD: Segment of POST routine in conv mem */
#define O_LANA      0x30 /* 1    BYTE: LAN adapter number                  */
#define O_CMP_CODE  0x31 /* 1    BYTE: Command completion code             */
#define O_RSRV      0x32 /* 1*14 BYTE[14]: Reserved area                   */
#define O_USER0     0x40 /* 4    DWORD: User variable                      */
#define O_USER1     0x44 /* 4    DWORD: User variable                      */
#define O_USER2     0x48 /* 4    DWORD: User variable                      */
#define O_USER3     0x4C /* 4    DWORD: User variable                      */

uc      WS_RD[MNCB+1][LNCBMAX];
uc      WS_SD[MNCB+1][LNCBMAX];

#define       LNCBSTRUCT        64
struct {
        UCHAR ncb_command;                            //  1
        UCHAR ncb_retcode;                            // +1
        UCHAR ncb_lsn;                                // +1
        UCHAR ncb_num;                                // +1
        PUCHAR ncb_buffer;                            // +8
        WORD ncb_length;                              // +2
        UCHAR ncb_callname[NCBNAMSZ];                 // +16
        UCHAR ncb_name[NCBNAMSZ];                     // +16
        UCHAR ncb_rto;                                // +1
        UCHAR ncb_sto;                                // +1
        void (CALLBACK *ncb_post)(struct _NCB*);      // +4
        UCHAR ncb_lana_num;                           // +1
        UCHAR ncb_cmd_cplt;                           // +1
        UCHAR ncb_reserve[10];                        // +10 = 64
        HANDLE ncb_event;                             //
} WS_ncb[MNCB+1];

//***************************************************************************
ex  uc  NMWS [16];                          // 
ex  vd  DPrn(vkt_i, vktuc *, ...);          // 
ex  _i  Debug(_i);                          // 
ex  _i  NSTAT;                              // статус сети NetBIOS
ex  _i  NBWLANA;
//***************************************************************************
