// coding: cp866
//---------------------------------------------------------------------------
ex _i VKSPInit(ui);
ex _i VKSPFifoInit(vd);
ex _i VKSPSet(ui,ui,ui);
ex vd VKSPHand(ui);
ex vd VKSPStop(vd);
us  VKSPOut    (ui,us);
_i  VKSPOutStat   (ui);
ex _i COMGetCH(ui);
ex vd COMPutCH(ui,_i,_i,_f);
ex vd COMReqCH(ui,_i,_i);
ex vd inp_COM(ui);
ex vd out_COM(ui);
ex vd ClearCB(ui);
//---------------------------------------------------------------------------
ex uc SPSBuf[SPMAX][SPBSZ];             // буфер посылки
ex uc SPRBuf[SPMAX][SPBSZ];             // буфер приема
ex uc SPTBuf[SPMAX][14];                // буфер передачи принятых данных
ex uc ICTS[SPMAX];                      // байт статуса при посылке
ex uc ICTR[SPMAX];                      // байт статуса при приеме
ex uc ICTQ[SPMAX];                      // байт статуса прерывания
ex uc ICTM[SPMAX];                      // байт статуса модема
ex uc TMPB[SPMAX];                      // байт инициализации
ex _i SPIO[SPMAX];                      // адреса COM портов
ex _i LSSB[SPMAX];                      // длинна строки посылки
ex _i CSSB[SPMAX];                      // N текущего символа буфера посылки
ex _i CSRB[SPMAX];                      // N текущего символа буфера приема
ex uc COMTmp[5];                        // строка преобразования
ex uc COMCRC[SPMAX];                    // контрольная сумма
ex uc COMCRCTmp[14];                    // временный буфер подсчета CRC
ex _i COMBusy[SPMAX];                   // флаг работы с COM портом
ex _f COMCVal [SPMAX];                  // принятые значения текущих каналов
ex _f COMSCVal[SPMAX];                  // значение для установки  на канале
ex _i COMCCh  [SPMAX];                  // номер текущего принятого канала
ex _i TOInp[SPMAX];                     // таймаут ожидания приема
ex _i SPRESULT[SPMAX];                  // код ошибки при опросе портов
//---------------------------------------------------------------------------
// структура запроса на передачу значений каналов
ex struct {
   uc   Begin;                          // 1
   uc   Addr[2];                        // 2
   uc   Code;                           // 1
   uc   nCH1[4];                        // 4
   uc   nCH2[4];                        // 4
   uc   CRC;                            // 1
   uc   End;                            // 1
}  COMREQ[SPMAX];                       // = 14 байт
//---------------------------------------------------------------------------
// структура запроса на установку значений каналов
ex struct {
   uc   Begin;                          // 1
   uc   Addr[2];                        // 2
   uc   Code;                           // 1
   uc   nCHS[4];                        // 4
   _f   Val __attribute__((packed));    // 4
   uc   CRC;                            // 1
   uc   End;                            // 1
}  COMSET[SPMAX];                       // = 14 байт
//---------------------------------------------------------------------------
// структура ответа на запрос
ex struct {
   uc   Begin;                          // 1
   uc   Addr[2];                        // 2
   uc   Code;                           // 1
   uc   nCHA[4];                        // 4
   _f   Val   __attribute__((packed));  // 4
   uc   CRC;                            // 1
   uc   End;                            // 1
}  COMANS[SPMAX];                       // = 14 байт
//---------------------------------------------------------------------------
