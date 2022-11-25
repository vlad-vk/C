// coding: cp866
//---------------------------------------------------------------------------
// определение установки графического режима
vd SetMGraph(vd){
    _i  g=1, w=0, h=0;   
    set_color_depth(DCOLOR);
    get_desktop_resolution(&w,&h);
    sprintf(SMSG[MMSG],"Current desktop resolution: %dx%d\n",w,h);
    DPrn(0,0,SMSG[MMSG]);
   //------------------------------------------------------------------------
    g=set_gfx_mode(GFX_AUTODETECT, w, h, 0, 0); GRMode[0]='A';
    if(g)
    g=set_gfx_mode(GFX_AUTODETECT, XMAX, YMAX, 0, 0); GRMode[0]='A';
   //------------------------------------------------------------------------
   if(g){
      sprintf(SMSG[MMSG],SMSG[89],XMAX,YMAX);           // "D: Impossible install graphic modes for permit: %dx%d."
      DPrn(0,0,"%s\n",SMSG[MMSG]);
      Mexit (0,1);
   }  else  {
      sprintf(SMSG[MMSG],SMSG[90],GRMode[0],XMAX,YMAX); // "D: Set graphics mode: %c:%dx%d."
      DPrn(0,0,"%s\n",SMSG[MMSG]);
   }
   //------------------------------------------------------------------------
   return;     
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// установка графического режима, глубины цвета и загрузка файла фонтов
// (бывают слеты на функции clear_to_color...)
_i Set_Graph(vd){
   _i  a=0, b=0;
   SetMGraph();
   DPrn(0,0,"Load font data file.\n");
   dFnt = load_datafile(FNDtf);
   if(dFnt==NULL){
      sprintf(SMSG[MMSG],SMSG[91],FNDtf);   // "D: Error loaded data file [%s]."
      DPrn(0,0,"%s\n",SMSG[MMSG]);
      Mexit(0,1);
    }
    DPrn(0,0,"Load cursors data file.\n");
    dSys = load_datafile(FNDts);
    if(dSys == NULL){
      sprintf(SMSG[MMSG],SMSG[91],FNDts);   // "D: Error loaded data file [%s]."
      DPrn(0,0,"%s\n",SMSG[MMSG]);
      Mexit(0,1);
    }
    // SWITCH_NONE, SWITCH_PAUSE, SWITCH_AMNESIA, 
    // SWITCH_BACKGROUND, SWITCH_BACKAMNESIA
    // set_display_switch_mode(SWITCH_PAUSE);
    //------------------------------------------------------------------------
    sfh=text_height(dFnt[SFNT].dat    );     // высота системного фонта
    sfl=text_length(dFnt[SFNT].dat,"0");     // ширина символа системного фонта
    //------------------------------------------------------------------------
    DPrn(0,0,"Rect fill screen.\n");
    rectfill(screen,0,0,XMAX,YMAX,0);
    //------------------------------------------------------------------------
    // загрузка логотипа
    sprintf(FNTmp,"%s/codus_lg.pcx",FDir);
    if((access(FNTmp,4))!=0){ sprintf(FNTmp,"%s",FNLgt); }
    if((access(FNTmp,4))==0){
       pcxbuf = load_bitmap(FNTmp,pal);
       if (!pcxbuf){
            sprintf(SMSG[MMSG],SMSG[92],FNTmp);
            DPrn(0,0,"%s\n",SMSG[MMSG]);
            Mexit(0,1);
       }
       set_pallete(pal);
       blit(pcxbuf,screen,0,0,0,0,XMAX,YMAX);
       destroy_bitmap(pcxbuf);
    }
    //------------------------------------------------------------------------
    // отображения номера версии
    // далее по программе после определённой инициализации делаем вывод на экран
    // с помощью функции smPRN какого-нибудь знака...
    if(VWver==1){
      sprintf(FNTmp,MDEC[11],codus_version,OSVER);
      smPRN(FNTmp,0,VWcol);
    }
    //------------------------------------------------------------------------
    pcxsav=create_bitmap_ex(DCOLOR,XMAX,YMAX); clear_to_color(pcxsav,0);
    pcxrun=create_bitmap_ex(DCOLOR,XMAX,YMAX); clear_to_color(pcxrun,0);
    pcxval=create_bitmap_ex(DCOLOR,XMAX,YMAX); clear_to_color(pcxval,0);
    pcxtmp=create_bitmap_ex(DCOLOR,XMAX,YMAX); clear_to_color(pcxtmp,0);
    pcxsss=create_bitmap_ex(DCOLOR,XMAX,YMAX); clear_to_color(pcxsss,0);
    pcxmnu=create_bitmap_ex(DCOLOR,XMAX,YMAX); clear_to_color(pcxmnu,0);
    scrsav=create_bitmap_ex(DCOLOR,XMAX,YMAX); clear_to_color(scrsav,0);
    scrout=create_bitmap_ex(DCOLOR,XMAX,YMAX); clear_to_color(scrout,0);
    scrmou=create_bitmap_ex(DCOLOR,XMAX,YMAX); clear_to_color(scrmou,0);
    winedt=create_bitmap_ex(DCOLOR,XMAX,EDT_H);clear_to_color(winedt,EDTBC);
    //------------------------------------------------------------------------
    rectfill(scrsav,0,0,XMAX,YMAX,0);
    //------------------------------------------------------------------------
    text_mode(-1);
    if(VWini>0){ 
      smPRN(SMSG[93],0,VWcol);          // " Инициализация"
    }
    //------------------------------------------------------------------------
    // загрузить файлы курсоров
    for(a=0;a<3;a++){
       curb[a]=create_bitmap(20,32); clear_to_color(curb[a],0);
       blit(dSys[curn[a]].dat,curb[a],0,0,0,0,20,32);
    }
   //------------------------------------------------------------------------
   // резервирование памяти под отдельные спрайты
   if(VWini>0){ smPRN(".",0,VWcol); }
   for(a=0;a<MSPR;a++){
       LSPR[a].p=create_bitmap(WSPR,WSPR);
       clear_to_color(LSPR[a].p,0);
   }
   //------------------------------------------------------------------------
   // резервирование памяти под строки выбора
   for(a=0;a<MCST;a++){
       LCST[a].pb=create_bitmap(MCSTPBX,MCSTPBY);
       clear_to_color(LCST[a].pb,0);
   }
   //------------------------------------------------------------------------
   // резервирование памяти под спрайты кнопок меню
   if(VWini>0){ smPRN(".",0,VWcol); }
   for(a=0;a<MKNP;a++){
      for(b=0;b<3;b++){
         LKNP[a].sp[b]=create_bitmap(WKNP,WKNP);
         if (LKNP[a].sp[b]==NULL){
             sprintf(SMSG[MMSG],SMSG[94],MKNP,WKNP,a,b);
             DPrn(0,0,"%s\n",SMSG[MMSG]);
             Mexit(0,0);
         }
         clear_to_color(LKNP[a].sp[b],0);
      }
   }
   //------------------------------------------------------------------------
   // резервирование памяти под спрайты анимации
   if(VWini>0){ smPRN(".",0,VWcol); }
   for(a=0;a<MANI;a++){
      LANI[a].ss=create_bitmap(WANI,WANI);
      if (LANI[a].ss==NULL){
          sprintf(SMSG[MMSG],SMSG[95]);
          DPrn(0,0,"%s\n",SMSG[MMSG]);
          Mexit(0,0);
      }
      clear_to_color(LANI[a].ss,0);
      for(b=0;b<MASP;b++){
         LANI[a].sp[b]=create_bitmap(WANI,WANI);
         if (LANI[a].sp[b]==NULL){
             sprintf(SMSG[MMSG],SMSG[96]);
             DPrn(0,0,"%s\n",SMSG[MMSG]);
             Mexit(0,0);
         }
         clear_to_color(LANI[a].sp[b],0);
      }
   }
   //------------------------------------------------------------------------
   // резервирование памяти под спрайты ползунков
   if(VWini>0){ smPRN(".",0,VWcol); }
   for(a=0;a<MPLZ;a++){
      LPLZ[a].ss=create_bitmap(WPLZ,WPLZ);
      if (LPLZ[a].ss==NULL){
          sprintf(SMSG[MMSG],SMSG[97],MPLZ,WPLZ,a);
          DPrn(0,0,"%s\n",SMSG[MMSG]);
          Mexit(0,0);
      }
      clear_to_color(LPLZ[a].ss,0);
      for(b=0;b<3;b++){
         LPLZ[a].sp[b]=create_bitmap(WPLZ,WPLZ);
         if (LPLZ[a].sp[b]==NULL){
             sprintf(SMSG[MMSG],SMSG[98],MPLZ,WPLZ,a,b);
             DPrn(0,0,"%s\n",SMSG[MMSG]);
             Mexit(0,0);
         }
         clear_to_color(LPLZ[a].sp[b],0);
      }
    }
    //------------------------------------------------------------------------
    // установить текущий курсор
    set_mouse_sprite(curb[curp]);
    set_mouse_sprite_focus(curx[curp],cury[curp]);
    //------------------------------------------------------------------------
    if(VWini>0){ smPRN(".",0,VWcol); }
    // ждать указанное количество миллисекунд
    rest(VWLgt*1000);
    return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
_i Ust_Graph(vd){
   _i a=0, b=0;
   sprintf(SMSG[MMSG],SMSG[99]);
   DPrn(20,0,"%s\n",SMSG[MMSG]);
   // ползунки
   for(a=0;a<MPLZ;a++){
      for(b=0;b<3;b++){ destroy_bitmap(LPLZ[a].sp[b]); }
      destroy_bitmap(LPLZ[a].ss);
   }
   sprintf(SMSG[MMSG],SMSG[100]);
   DPrn(20,0,"%s\n",SMSG[MMSG]);
   // анимация
   for(a=0;a<MANI;a++){
      for(b=0;b<MASP;b++){ destroy_bitmap(LANI[a].sp[b]); }
      destroy_bitmap(LANI[a].ss);
   }
   sprintf(SMSG[MMSG],SMSG[101]);
   DPrn(20,0,"%s\n",SMSG[MMSG]);
   // кнопки меню
   for(a=0;a<MKNP;a++){
      for(b=0;b<3;b++){ destroy_bitmap(LKNP[a].sp[b]); }
   }
   // спрайты
   sprintf(SMSG[MMSG],SMSG[102]);
   DPrn(20,0,"%s\n",SMSG[MMSG]);
   for(a=0;a<MSPR;a++){ destroy_bitmap(LSPR[a].p); }
   sprintf(SMSG[MMSG],SMSG[103]);
   DPrn(20,0,"%s\n",SMSG[MMSG]);
   // курсоры
   for(a=0;a<3;a++){ destroy_bitmap(curb[a]); }
   sprintf(SMSG[MMSG],SMSG[104]);
   DPrn(20,0,"%s\n",SMSG[MMSG]);
   // экраны
   destroy_bitmap(scrsav);
   destroy_bitmap(pcxtmp);
   destroy_bitmap(pcxval);
   destroy_bitmap(pcxsss);
   destroy_bitmap(pcxmnu);
   destroy_bitmap(pcxrun);
   destroy_bitmap(pcxsav);
   destroy_bitmap(scrout);
   destroy_bitmap(winedt);
   sprintf(SMSG[MMSG],SMSG[105]);
   DPrn(20,0,"%s\n",SMSG[MMSG]);
   return(0);
}
//---------------------------------------------------------------------------
