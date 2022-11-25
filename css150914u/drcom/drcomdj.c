// coding: cp866
//---------------------------------------------------------------------------
// выбор строки по переменной
#include <allegro.h>
#include "../include.h"
#include "../define.h"
#include "drcom.h"


//---------------------------------------------------------------------------
// начальные определения
extern unsigned short __djgpp_ds_alias;
extern void VKSPProtISR(void);

#define Ctrl8259_0 0x020                      // 8259 port
#define Ctrl8259_1 0x021                      // 8259 port (Masks)
#define BufSize 32768                         // Buffer Size

// Globals to be set in VKSPInit()
static unsigned char VectorNum;               // Vector Number
static unsigned char EnableIRQ;               // Mask to enable 8259 IRQ
static unsigned char DisableIRQ;              // Mask to disable 8259 IRQ
static _go32_dpmi_seginfo ProtVector;         // Old Protmode Vector
static _go32_dpmi_seginfo info;               // New Protmode Vector

// Register Addresses for the UART
static unsigned short Port;                   // Port Base Address

unsigned short THR;                           // Transmitter Holding Register
unsigned short RDR;                           // Reciever Data Register
unsigned short BRDL;                          // Baud Rate Divisor, Low byte
unsigned short BRDH;                          // Baud Rate Divisor, High Byte
unsigned short IER;                           // Interupt Enable Register
unsigned short IIR;                           // Interupt Identification Register
unsigned short FCR;                           // FIFO Control Register
unsigned short LCR;                           // Line Control Register
unsigned short MCR;                           // Modem Control Register
unsigned short LSR;                           // Line Status Register
unsigned short MSR;                           // Modem Status Register
unsigned short SCR;                           // SCR Register

// Buffer Data
unsigned volatile char RecBuffer[BufSize] = { 0 };
unsigned volatile int RecHead, RecTail;

static unsigned char VKSPStatus=0;

static void lock_interrupt_memory(void);
static void unlock_interrupt_memory(void);


//***************************************************************************
// This will empty the recieve buffer
vd VKSPClear(vd){
   disable();
   RecHead = 0;
   RecTail = 0;
   enable();
   return;
}
//***************************************************************************



//***************************************************************************
// This initalizes the serial port and installs the ISR
// CommPort maps out to the following: 0-COM1, 1-COM2, 2-COM3, 3-COM4
_i VKSPInit(unsigned int CommPort){
   // Set various things according to com port number
   if(!CommPort){              // com 1
       Port=0x03F8; VectorNum=0x0C; EnableIRQ=0xEF; DisableIRQ=0x10;
   } else {
     if(CommPort==1){          // com 2
       Port=0x02F8; VectorNum=0x0B; EnableIRQ=0xF7; DisableIRQ=0x08;
   } else {
     if(CommPort==2){          // com 3
       Port=0x03E8; VectorNum=0x0C; EnableIRQ=0xEF; DisableIRQ=0x10;
   } else {                    // com 4
       Port=0x02E8; VectorNum=0x0B; EnableIRQ=0xF7; DisableIRQ=0x08;
   } } }
   // Compute Register locations
   THR   = Port;
   RDR   = Port;
   BRDL  = Port;
   BRDH  = 1 + Port;
   IER   = 1 + Port;
   IIR   = 2 + Port;
   FCR   = 2 + Port;
   LCR   = 3 + Port;
   MCR   = 4 + Port;
   LSR   = 5 + Port;
   MSR   = 6 + Port;
   SCR   = 7 + Port;
   // Initalize Buffer
   VKSPClear();
   lock_interrupt_memory();
   atexit(unlock_interrupt_memory);
   // Set bit 3 in MCR to 0
   outportb(MCR, (inportb(MCR) & 0xF7));
   // Save and reassign interrupt vectors
   _go32_dpmi_get_protected_mode_interrupt_vector(VectorNum, &ProtVector);
   info.pm_offset = (int) VKSPProtISR;
   info.pm_selector = _my_cs();
   _go32_dpmi_set_protected_mode_interrupt_vector(VectorNum, &info);
   atexit(VKSPStop);
   // Enable 8259 interrupt (IRQ) line for this async adapter
   outportb(Ctrl8259_1, (inportb(Ctrl8259_1) & EnableIRQ));
   // Enable 8250 Interrupt-on-data-ready
   outportb(LCR, (inportb(LCR) & 0x7F));
   outportb(IER, 0);
   if(inportb(IER)){ VKSPStatus=0; return 1; }
   outportb(IER, 0x01);
   // Clear 8250 Status and data registers
   {
      unsigned char temp;
      do {
          temp=inportb(RDR);
          temp=inportb(LSR);
          temp=inportb(MSR);
          temp=inportb(IIR);
      }  while(!(temp & 1));
   }
   // Set Bit 3 of MCR -- Enable interupts
   outportb(MCR, (inportb(MCR) | 0x08));
   VKSPStatus=1;
   // Clear Buffer Just in case
   VKSPClear();
   return 0;
}
//***************************************************************************



//***************************************************************************
// This uninstalls the ISR and resets the serial port.
vd VKSPStop(vd){
   if (!VKSPStatus){ return; }
   VKSPStatus = 0;
   // Mask (disable) 8259 IRQ Interrupt
   outportb(Ctrl8259_1, (inportb(Ctrl8259_1) | DisableIRQ));
   // Disable 8250 interrupt
   outportb(LCR, (inportb(LCR) & 0x7F));
   outportb(IER, 0);
   // Set bit 3 in MCR to 0
   outportb(MCR, (inportb(MCR) & 0xF7));
   // Interrupts are disabled.  Restore saved interrupt vector.
   _go32_dpmi_set_protected_mode_interrupt_vector(VectorNum, &ProtVector);
   return;
}
//***************************************************************************



//***************************************************************************
// Gets a byte from the input buffer
uc VKSPIn(void){
   unsigned char retvalue;
   if(RecTail == RecHead){ return 0; }
   disable();
   retvalue = RecBuffer[RecTail++];
   if(RecTail >= BufSize){ RecTail=0; }
   enable();
   return retvalue;
}
//***************************************************************************



//***************************************************************************
// Gets a binary from the input buffer
_i VKSPInBin(uc *BinIn){
   if(RecTail==RecHead){ return 0; }
   if(!BinIn){ return 0; }
   disable();
   *BinIn = RecBuffer[RecTail++];
   if(RecTail>=BufSize){ RecTail=0; }
   enable();
   return 1;
}
//***************************************************************************



//***************************************************************************
// This simply outputs a byte to the serial port.
vd VKSPOut(uc CharOut){
   while(~inportb(LSR) & 0x20);
   outportb(THR, CharOut);
   return;
}
//***************************************************************************



//***************************************************************************
// Sets communication parameters
// Baud = 150, 300, 600, 1200, 2400, 4800, 9600, 19200, 28800, 38400, 57600
// Control = The value to place in the LCR
_i VKSPSet(ui p, ui Baud, ui Control){
   _i  divisor;
   uc  divlow, divhigh;
   if(!Baud){ return(0); }
   divisor = divi(115200, Baud);
   disable();
   outportb(LCR, Control | 0x80);      // Set Port toggle BRDL/BRDH registers
   divlow = divisor & 0x000000ff;
   divhigh = (divisor >> 8) & 0x000000ff;
   outportb(BRDL, divlow);             // Set Baud Rate
   outportb(BRDH, divhigh);
   outportb(LCR, Control & 0x007F);    // Set LCR and Port Toggle
   enable(); p=1;
   return(p);                          // p- ненужный оператор для совмест
}
//***************************************************************************



//***************************************************************************
// Returns the # of characters in input buffer waiting to be read.
_i VKSPInStat(vd){
   _i retvalue;
   if(RecHead >= RecTail){ retvalue = RecHead - RecTail; }
   else                  { retvalue = (RecHead - RecTail) + BufSize; }
   return retvalue;
}
//***************************************************************************


//***************************************************************************
// Returns the # of characters in output buffer.
// This will only be useful once interrupt driven transmitting is implemented.
// Now, it returns 1 if it cannot instantly send a character and 0 otherwise.
_i VKSPOutStat(vd){
   if(~inportb(LSR) & 0x20){ return 1; }
   else                    { return 0; }
}
//***************************************************************************



//***************************************************************************
// Sets various handshaking lines  returns nothing.
vd VKSPHand(ui Hand){
   outportb(MCR, Hand | 0x08);  // Keep interrupt enable ON
   return;
}
//***************************************************************************



//***************************************************************************
ui VKSPStat(vd){
   return 0;
}
//***************************************************************************



//***************************************************************************
// Internal function
static void lock_interrupt_memory(void){
   int errval;
   __dpmi_meminfo info;
   unsigned long address;
   __dpmi_get_segment_base_address(_my_ds(), &address);
   //------------------------------------------------------------------------
   info.address = (int) address + (int) &RDR;
   info.size = sizeof(RDR);
   errval = __dpmi_lock_linear_region(&info);
   if(errval==-1){ DPrn(0,0,"Error in locking memory (1)\n!"); }
   //------------------------------------------------------------------------
   info.address = (int) address + (int) &LSR;
   info.size = sizeof(LSR);
   errval = __dpmi_lock_linear_region(&info);
   if(errval==-1){ DPrn(0,0,"Error in locking memory (2)\n!"); }
   //------------------------------------------------------------------------
   info.address = (int) address + (int) &RecHead;
   info.size = sizeof(RecHead);
   errval = __dpmi_lock_linear_region(&info);
   if(errval==-1){ DPrn(0,0,"Error in locking memory (3)\n!"); }
   //------------------------------------------------------------------------
   info.address = (int) address + (int) &RecBuffer;
   info.size = sizeof(RecBuffer);
   errval = __dpmi_lock_linear_region(&info);
   if(errval==-1){ DPrn(0,0,"Error in locking memory (4)\n!"); }
   //------------------------------------------------------------------------
   info.address = (int) address + (int) RecBuffer;
   info.size = BufSize;
   errval = __dpmi_lock_linear_region(&info);
   if(errval==-1){ DPrn(0,0,"Error in locking memory (5)\n!"); }
   //------------------------------------------------------------------------
   __dpmi_get_segment_base_address(_my_cs(), &address);
   info.address = (int) address + (int) VKSPProtISR;
   info.size = 4096;
   errval = __dpmi_lock_linear_region(&info);
   if(errval==-1){ DPrn(0,0,"Error in locking memory (6)\n!"); }
   //------------------------------------------------------------------------
   return;
}
//***************************************************************************



//***************************************************************************
// Internal function
static void unlock_interrupt_memory(void){
   __dpmi_meminfo info;
   unsigned long address;

   __dpmi_get_segment_base_address(_my_ds(), &address);

   info.address = (int) address + (int) &RDR;
   info.size = sizeof(RDR);
   __dpmi_unlock_linear_region(&info);

   info.address = (int) address + (int) &LSR;
   info.size = sizeof(LSR);
   __dpmi_unlock_linear_region(&info);

   info.address = (int) address + (int) &RecHead;
   info.size = sizeof(RecHead);
   __dpmi_unlock_linear_region(&info);

   info.address = (int) address + (int) &RecBuffer;
   info.size = sizeof(RecBuffer);
   __dpmi_unlock_linear_region(&info);

   info.address = (int) address + (int) RecBuffer;
   info.size = BufSize;
   __dpmi_unlock_linear_region(&info);

   __dpmi_get_segment_base_address(_my_cs(), &address);

   info.address = (int) address + (int) VKSPProtISR;
   info.size = 4096;
   __dpmi_unlock_linear_region(&info);

   return;
}
//***************************************************************************



//***************************************************************************
// Detects UART type, enables FIFO if it exists.
// Returns 0 if 16550 UART and FIFO enabled.
// Returns 1 if 16450 UART.
// Returns 2 if less than 16450 UART.
_i VKSPFifoInit(vd){
   outportb(SCR, 0x55);
   if(inportb(SCR) != 0x55){ return 2; }
   outportb(FCR, 0x0f);
   if((inportb(IIR)&0xC0) != 0xC0){ return 1; }
   outportb(FCR, 1 | 2 | 64);          // 8 byte trigger level on receive.
   return 0;
}
//***************************************************************************








//---------------------------------------------------------------------------
// Очистка буфера приема
vd ClearCB(ui p){
   _i  a=0;
   while(RecTail!=RecHead){
         VKSPIn(); a++;
         if(a==50){ DPrn(6,1,"Clear COM port recv buffer.\n"); }
         if(a> 50){ break; }
   }
   CSRB[p]=0; COMBusy[p]=0; TOInp[p]=0; RecTail=RecHead;
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// передача данных из буфера приема
vd inp_COM(ui p){
   _i  j=0,i=0;
   // если это не режим ожидания приема - выйти
   if (COMBusy[p]!=2){
       DPrn(6,1,"?CB%d\n",COMBusy[p]);
       return;
   }
   //  каждую миллисекунду увеличиваем счетчик таймаута приема
   if(MTimer(8,'m',1)==2){
      MTimer(8,'m',0); if(TOInp[p]<10000){ TOInp[p]++; }
   }
   // если в буфере приема нет принятых символов - выйти
   if (VKSPInStat()<1){
       DPrn(6,0,"-");
       return;
   };  TOInp[p]=0;
   DPrn(6,1,"06:RCV:");
   // передача данных из буфера COM порта в буфер приема,
   for(j=0;j<SPBSZ;j++){
       // если больше нет символов для передачи - выйти из цикла
       if(VKSPInStat()<1){
          // если в буфер ничего не было передано- выйти из функции
          if(CSRB[p]<1){ DPrn(6,0,"[Z]"); return; }
          // если какие-то данные были переданы в программный буфер-
          // уйти на их обработку
          else         { break;  }
       }
       // записать очередной принятый символ в программный буфер
       SPRBuf[p][CSRB[p]]= VKSPIn();
       DPrn(6,0,"%c",SPRBuf[p][CSRB[p]]);
       // если первый символ в буфере не равен заданному ограничителю-
       // не увеличивать счетчик приема
       if(SPRBuf[p][0]!='@'){ CSRB[p]=0; continue; }
       // увеличить счетчик текущего символа в программном буфере
       if(CSRB[p]<SPBSZ-1){ CSRB[p]++; }
       // если буфер полный - выйти из цикла
       else               { break; }
   }
   DPrn(6,0,"\n");
   // выполнить не более 100 циклов обработки программного буфера (100 комманд)
   for(j=0;j<100;j++){
       // если еще не приняли нужное кол-во символов- выйти для ожидания приема
       if(CSRB[p]<13){
          DPrn(6,1,"06:INP:WAIT: Little recv chars [%d].\n",CSRB[p]);
          return;
       }
       // если нет второго ограничительного символа строки комманды - выйти
       if(SPRBuf[p][13]!='*'){
          DPrn(6,1,"06:INP:NOTEND: not ended data char '*'. Clear recv buffer.\n");
          break;
       }
       // принятую комманду передать в буфер обработки
       memcpy(&COMANS[p],&SPRBuf[p],14);
       COMGetCH(p);
       // сдвинуть данные в буфере приема на длинну комманды (14символов) влево
       for(i=0;i<CSRB[p];i++){
           SPRBuf[p][i]=SPRBuf[p][i+14];
       };  CSRB[p]=CSRB[p]-14;
   }
   if(MTimer(8,'m',1)==2){
      MTimer(8,'m',0); if(TOInp[p]<10000){ TOInp[p]++; }
   }
   // сбросить счетчик таймаута приема
   ClearCB(p);
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// посылка данных в COM порт
vd out_COM(ui p){
   _i  a=0;
   if(COMBusy[p]!=1){ return; }
   for(a=0;a<10000;a++){
       ICTS[p]=inportb(SPIO[p]+5);
       if(((ICTS[p]>>5)&1)==1){
         outportb(SPIO[p],SPSBuf[p][CSSB[p]]); CSSB[p]++; a=0;
       }
       // если вся строка была передана
       if (CSSB[p]>=LSSB[p]){
           CSSB[p]=0; LSSB[p]=0; CSRB[p]=0; COMBusy[p]=2;
           DPrn(5,1,"05:SND:[%s]\n",SPSBuf[p]);
           TOInp[p]=0;
           break;
       }
   }
   return;
}
//---------------------------------------------------------------------------







//---------------------------------------------------------------------------
// формирование запроса на получение значения канала
vd COMReqCH(ui p,_i WS,_i CHNum){
   _i a=0;
   COMCRC[p]=0;
   COMREQ[p].Begin='@';
   if(WS>99){ WS=99; }
   if(WS< 0){ WS= 0; }
   COMTmp[0]=0; COMTmp[1]=0; COMTmp[2]=0; COMTmp[3]=0; COMTmp[4]=0;
   sprintf(COMTmp,"%d",WS);
   if(COMTmp[1]==0){ COMTmp[1]=COMTmp[0]; COMTmp[0]='0'; }
   COMREQ[p].Addr[0]=COMTmp[0];
   COMREQ[p].Addr[1]=COMTmp[1];
   COMREQ[p].Code='Q';
   if(CHNum>=MCH){ CHNum=0; }
   if(CHNum<   0){ CHNum=0; }
   COMTmp[0]=0; COMTmp[1]=0; COMTmp[2]=0; COMTmp[3]=0; COMTmp[4]=0;
   sprintf(COMTmp,"%d",CHNum);
   for(a=0;a<4;a++){
       if(COMTmp[3]==0){
          COMTmp[3]=COMTmp[2]; COMTmp[2]=COMTmp[1];
          COMTmp[1]=COMTmp[0]; COMTmp[0]='0';
       }
   }
   COMREQ[p].nCH1[0]=COMTmp[0];
   COMREQ[p].nCH1[1]=COMTmp[1];
   COMREQ[p].nCH1[2]=COMTmp[2];
   COMREQ[p].nCH1[3]=COMTmp[3];
   COMREQ[p].nCH2[0]=COMTmp[0];
   COMREQ[p].nCH2[1]=COMTmp[1];
   COMREQ[p].nCH2[2]=COMTmp[2];
   COMREQ[p].nCH2[3]=COMTmp[3];
   memcpy(&COMCRCTmp,&COMREQ[p],14);
   for(a=1;a<12;a++){ COMCRC[p]=COMCRC[p]^COMCRCTmp[a]; }
   COMREQ[p].CRC=COMCRC[p];
   COMREQ[p].End='*';
   LSSB[p]=14;
   memcpy(&SPSBuf[p],&COMREQ[p],LSSB[p]); CSSB[p]=0;
   COMBusy[p]=1;
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// формирование запроса на установку значения на канале
vd COMPutCH(ui p,_i WS,_i CHNum,_f Val){
   _i a=0;
   COMCRC[p]=0;
   if(WS>99){ WS=99; }
   if(WS< 0){ WS= 0; }
   for(a=0;a<5;a++){ COMTmp[a]=0; }
   sprintf(COMTmp,"%d",WS);
   if(COMTmp[1]==0){ COMTmp[1]=COMTmp[0]; COMTmp[0]='0'; }
   COMSET[p].Begin='@';
   COMSET[p].Addr[0]=COMTmp[0];
   COMSET[p].Addr[1]=COMTmp[1];
   COMSET[p].Code='A';
   if(CHNum>=MCH){ CHNum=0; }
   if(CHNum<   0){ CHNum=0; }
   sprintf(COMTmp,"%d",CHNum);
   for(a=0;a<4;a++){
       if(COMTmp[3]==0){
          COMTmp[3]=COMTmp[2]; COMTmp[2]=COMTmp[1];
          COMTmp[1]=COMTmp[0]; COMTmp[0]='0';
       }
   }
   COMSET[p].nCHS[0]=COMTmp[0];
   COMSET[p].nCHS[1]=COMTmp[1];
   COMSET[p].nCHS[2]=COMTmp[2];
   COMSET[p].nCHS[3]=COMTmp[3];
   COMSET[p].Val=Val;
   memcpy(&COMCRCTmp,&COMSET[p],14);
   for(a=1;a<12;a++){ COMCRC[p]=COMCRC[p]^COMCRCTmp[a]; }
   COMSET[p].CRC=COMCRC[p];
   COMSET[p].End='*';
   LSSB[p]=14;
   memcpy(&SPSBuf[p],&COMSET[p],LSSB[p]);
   COMBusy[p]=1;
   CSSB[p]=0;
   DPrn(7,0,"S=[%s]\n",SPSBuf[p]);
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// если получили ответ на запрос значения канала
_i COMGetCH(ui p){
   _i a=0;
   uc CCh[5]={0,0,0,0,0};
   COMCVal[p]=0;
   DPrn(6,1,"06:GET:");
   for(a=0;a<14;a++) DPrn(6,0,"%c",SPRBuf[p][a]);
   DPrn(6,0,"\n");
   if (COMANS[p].Begin!='@'&&COMANS[p].End!='*'){ return(0); }
   COMCRC[p]=0;
   memcpy(&COMCRCTmp,&COMANS[p],14);
   for(a=1;a<12;a++){ COMCRC[p]=COMCRC[p]^COMCRCTmp[a]; }
   if(COMANS[p].CRC!=COMCRC[p]){ return(0); }
   CCh[0]=COMANS[p].nCHA[0]; CCh[1]=COMANS[p].nCHA[1];
   CCh[2]=COMANS[p].nCHA[2]; CCh[3]=COMANS[p].nCHA[3];
   COMCCh[p]=atoi(CCh);
   if(COMCCh[p]>MCH-1||COMCCh[p]<1){
      DPrn(6,1,"06:GET:BAD:ChNUM\n"); return(0);
   }
   COMAnsVal(p,'z'); TOInp[p]=0;
   if(COMANS[p].Code=='q'){ COMAnsVal(p,'q'); return(1); }
   if(COMANS[p].Code=='a'){ COMAnsVal(p,'a'); return(2); }
   return(3);
}
//---------------------------------------------------------------------------
