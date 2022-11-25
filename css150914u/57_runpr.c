// coding: cp866
//---------------------------------------------------------------------------
// Выполнение внешних программ
_i RunPRG(_i b){
   // запомнить экран
   get_pallete(palrun);
   blit(scrsav,pcxrun,0,0,0,0,XMAX,YMAX);
   // если установлен флаг режима - переход в текстовый режим
   if (LRUN[b].md>0){ set_gfx_mode(GFX_TEXT, 80, 25, 0, 0); }
   // разрешить доступ к клавиатуре во внешней программе
   remove_keyboard();
   sprintf(SMSG[MMSG],SMSG[152],LRUN[b].prg);
   DPrn(0,0,"%s\n",SMSG[MMSG]);
   // выполнить внешнюю программу
   if(access(LRUN[b].prg,0)==0){
// #define SW_HIDE 0
// #define SW_NORMAL 1
// #define SW_SHOWNORMAL 1
// #define SW_SHOWMINIMIZED 2
// #define SW_MAXIMIZE 3
// #define SW_SHOWMAXIMIZED 3
// #define SW_SHOWNOACTIVATE 4
// #define SW_SHOW 5
// #define SW_MINIMIZE 6
// #define SW_SHOWMINNOACTIVE 7
// #define SW_SHOWNA 8
// #define SW_RESTORE 9
// #define SW_SHOWDEFAULT 10
// #define SW_FORCEMINIMIZE 11
// #define SW_MAX  11
    WinExec(LRUN[b].prg,2);
   }
   // передать управление клавиатурой библиотеки Allegro
   install_keyboard();
   // восстановить графический режим
//!!! раскоментировать
// if (LRUN[b].md>0){ SetMGraph(); }
   // восстановить экран
   set_pallete(palrun);
   blit(pcxrun,scrsav,0,0,0,0,XMAX,YMAX);
   // восстановить каталог
   chdir(CDir);
   return(0);
}
//---------------------------------------------------------------------------
