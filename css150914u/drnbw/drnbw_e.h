// coding: cp866
//---------------------------------------------------------------------------
// файл заголовков и описания переменных
#define  MNCB                           8           // кол-во NCB
#define  LNCB                           504         // длинна буфера посылки-приема
#define  LNCBMAX                        512         // макс возможная длинна буфера NCB

ex struct  NBRQ  {
   _i   cq[MRCH+1];                   // номера каналов на удален машине
   _i   ca[MRCH+1];                   // номера каналов для ответа
}  NBRQ[MQR+1];
ex struct  NBST  {
   _i   cq[MRCH+1];                   // номера каналов для установки
   _i   ca[MRCH+1];                   // номера каналов для ответа
}  NBST[MQR+1];

ex ui   CurNCBQ;                      // номер текущего NCB запроса
ex ui   CurNCFQ;                      // флаг определения нового NCB запроса
ex ui   CurNCBS;                      // номер текущего NCB посылки
ex ui   CurNCFS;                      // флаг определения нового NCB посылки
ex uc   QRC;                          // номер  текущей очереди для запроса
ex uc   STC;                          // номер  текущей очереди для установок
ex uc   WS_RD[MNCB+1][LNCBMAX];

ex ui   WS_NB__Init(ui);              // инициализация NetBIOS
ex ui   WS_NB__AddName(uc*);          // добавление имени в список сети
ex ui   WS_NB__DeleteName();          // удаление  имени из списка сети
ex ui   WS_NB__ReqToNet  (ui);        // посылка запросов
ex ui   WS_NB__SetToNet  (ui);        // посылка установок
ex uc   WS_NB__Cancel    (uc);        // отмена последней команды Receive
ex _i   WS_NB__AvNCB     (vd);
ex ui   WS_NB__ReqToNet  (ui);
ex vd   WS_NB__GetFromNet(ui);
ex vd   WS_NB__GetReq    (ui);
ex vd   WS_NB__Answer    (ui,ui);
ex uc   WS_NB__GetRetCCode  (ui);
ex uc   WS_NB__GetFinalCCode(ui);
ex uc * WS_NB__GetCallName(ui);
ex vd   WS_NB__ZeroNCB(ui);
ex vd   WS_NB__Receive(ui);
ex vd   WS_NB__Send(ui,uc*);
ex vd   Quit_NB(vd);
//---------------------------------------------------------------------------
