// coding: cp1251
//---------------------------------------------------------------------------
// определение разрешения отладки
vkt_i Debug(vkt_i md){
   vkt_i   a=0;
   for(a=0;a<MDB;a++){
      if (Deb[a]==md){ return(1); }
   }
   return(0);
}
//---------------------------------------------------------------------------
// запись в файл отладки служебных сообщений
vkt_v DPrn (_i lv, _i tm, vktuc *strfrm, ...){
   va_list  argptr;
   va_start(argptr,strfrm);
   if (lv==0||(Debug(lv))){
      if(DebOut!=NULL){
         if(tm>0){
            GetTime(0);
            fprintf(DebOut,"%04.0f.%02.0f.%02.0f %02.0f:%02.0f:%02.0f ",
            YEA_CMD,MON_CMD,DAY_CMD,HOU_CMD,MIN_CMD,SEC_CMD);
         }
         vfprintf(DebOut,strfrm,argptr);
//       if(GRInitF==0){ vprintf(strfrm,argptr); }
      }
   }
   va_end  (argptr);
   if(DebOut!=NULL){ fflush(DebOut); }
   return;
}
//---------------------------------------------------------------------------

