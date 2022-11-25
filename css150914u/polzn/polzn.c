// coding: cp866
//---------------------------------------------------------------------------
// выбор строки по переменной
#include <allegro.h>
#include "../include.h"
#include "../define.h"
#include "polzn.h"

//---------------------------------------------------------------------------
// проверка доступа к ползунку
_i isPLZ_En(_i a){
   // нулевой номер переменной не определяется
   if (LPLZ[a].nv< 1){ return(0); }
   // если не определены границы вывода ползунка
   if((LPLZ[a].W < 1)&&(LPLZ[a].H < 1)){ return(0); }
   // если не определена длинна ползунка
   if (LPLZ[a].L < 1){ return(0); }
   return(1);     
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
vd Polzun(_i md, _i kb){
   _i  a =0, fa=0,  b=0,  c=0;
   _f  vl=0, vx=0, vo=0,  r=0, r1=0, r2=0;
   _f   p=0, p1=0, p2=0, p3=0, p4=0, p5=0, p6=0, p7=0, p8=0;
   _f  z1=0, z2=0, z3=0, z4=0;
   _i  msy=0, msx=0;

   //------------------------------------------------------------------------
   // инициализация ползунков
   if (md==0){
       // для всех ползунков экрана 
       for(a=0,c=777;a<HPLZ;a++){
           // проверка разрешенности ползунка
           if((isPLZ_En(a))<1){ continue; }
           LPLZ[a].a=0;
           if (c==777){ c=a; LPLZ[a].a=1; }
           // если определен файл спрайтов для кнопки ползунка
           if (LPLZ[a].fb!=NULL){
               // загрузить файл кнопок ползунка
               pcxplz = load_pcx(LPLZ[a].fb,pal);
               if (pcxplz){
                   for(b=0;b<3;b++){
                       blit(pcxplz,LPLZ[a].sp[b],
                                   LPLZ[a].W*b+b,0, 0,0, LPLZ[a].W,LPLZ[a].H);
                   }
                   LPLZ[a].sf=1;
               }   else   {
                   DPrn(0,1,"Ошибка активизации спрайтов кнопок ползунка %d.\n",a);
                   LPLZ[a].sf=0;
               }
               destroy_bitmap(pcxplz);
               clear_bitmap(LPLZ[a].ss);
               //----------------------------------------------------------------
               // определить координаты для отображения кнопки:
               //----------------------------------------------------------------
               // направление снизу-вверх
               if (LPLZ[a].G==0){
                   LPLZ[a].x1= LPLZ[a].X;
                   LPLZ[a].y1= LPLZ[a].Y+LPLZ[a].L-vx-divi(LPLZ[a].H, 2);
                   LPLZ[a].r1=-VPLZ;
                   LPLZ[a].r2=-2;
                   LPLZ[a].r3= VPLZ*2;
                   LPLZ[a].r4= 4;
               //----------------------------------------------------------------
               // направление слева-направо
               }   else   {
                   LPLZ[a].x1= LPLZ[a].X+vx-divi(LPLZ[a].W, 2);
                   LPLZ[a].y1= LPLZ[a].Y;
                   LPLZ[a].r1= 0;
                   LPLZ[a].r2= 0;
                   LPLZ[a].r3= 0;
                   LPLZ[a].r4= 0;
               }
               //----------------------------------------------------------------
               // запомнить начальный экран под спрайтом ползунка
               blit(scrsav,LPLZ[a].ss,
                           LPLZ[a].x1+LPLZ[a].r1,
                           LPLZ[a].y1+LPLZ[a].r2,0,0,
                           LPLZ[a].W +LPLZ[a].r3,
                           LPLZ[a].H +LPLZ[a].r4 );
           }
       };  NaPL=c;
   }
   //------------------------------------------------------------------------


   //------------------------------------------------------------------------
   // отображение ползунков в цикле
   if (md==1){
       // для всех ползунков экрана
       for(a=0;a<HPLZ;a++){
           // проверка разрешенности ползунка
           if (isPLZ_En(a)<1){ continue; }
           // получить текущее значение переменной
           vl=Arr_to_Val(LPLZ[a].na,LPLZ[a].nv);
           // проверить границы переменной
           if (vl<LPLZ[a].min){ vl=LPLZ[a].min; }
           if (vl>LPLZ[a].max){ vl=LPLZ[a].max; }
           // если ползунок нажат и находится в движении
           if (MSK_PL==1){ vl=LPLZ[a].vl; }
           // определить текущее положение ползунка
           vx=divf((LPLZ[a].L*vl), (LPLZ[a].max-LPLZ[a].min));
           if (LPLZ[a].a==1){ fa=  VPLZ;    }
           else             { fa=-(VPLZ-1); }
           //----------------------------------------------------------------
           // восстановить экран под спрайтом ползунка
           blit(LPLZ[a].ss,scrsav,0,0,
                LPLZ[a].x1+LPLZ[a].r1, LPLZ[a].y1+LPLZ[a].r2,
                LPLZ[a].W +LPLZ[a].r3, LPLZ[a].H +LPLZ[a].r4);
           //----------------------------------------------------------------
           // переопределить координаты отображения ползунка
           //----------------------------------------------------------------
           // направление снизу-вверх
           if (LPLZ[a].G==0){
               LPLZ[a].x1= LPLZ[a].X;
               LPLZ[a].y1= LPLZ[a].Y+LPLZ[a].L-vx-divi(LPLZ[a].H, 2);
               LPLZ[a].r1=-VPLZ;
               LPLZ[a].r2=-2;
               LPLZ[a].r3= VPLZ*2;
               LPLZ[a].r4= 4;
               p1=LPLZ[a].X; p2=LPLZ[a].X+ LPLZ[a].W;
               p3=LPLZ[a].Y- LPLZ[a].H;
               p4=LPLZ[a].Y+ LPLZ[a].L+LPLZ[a].H;
               p5=p2; p6=LPLZ[a].Y+LPLZ[a].L;
               p7=LPLZ[a].X+LPLZ[a].W;
               p8=LPLZ[a].Y+LPLZ[a].L-vx+divi(LPLZ[a].H, 2);
               z1=LPLZ[a].X-VPLZ; z2=LPLZ[a].Y+LPLZ[a].L-vx;
               z3=p2+VPLZ-1; z4=z2;
               msy=(_f)mouse_y;
               r=(msy-MsY_PL)*(-1); r1=msy; r2=MsY_PL;
           //----------------------------------------------------------------
           // направление слева-направо
           }   else   {
               LPLZ[a].x1= LPLZ[a].X+vx-divi(LPLZ[a].W, 2);
               LPLZ[a].y1= LPLZ[a].Y;
               LPLZ[a].r1=-2;
               LPLZ[a].r2=-VPLZ;
               LPLZ[a].r3= 4;
               LPLZ[a].r4= VPLZ*2;
               p1=LPLZ[a].X- LPLZ[a].W;
               p2=LPLZ[a].X+ LPLZ[a].L+LPLZ[a].W;
               p3=LPLZ[a].Y; p4=LPLZ[a].Y+LPLZ[a].H;
               p5=LPLZ[a].X+LPLZ[a].L; p6=p4;
               p7=LPLZ[a].X+vx+divi(LPLZ[a].W, 2);
               p8=LPLZ[a].Y+LPLZ[a].H;
               z1=LPLZ[a].X+vx; z2=LPLZ[a].Y-VPLZ;
               z3=z1; z4=p4+VPLZ-1;
               msx=mouse_x;
               r=msx-MsX_PL; r1=msx; r2=MsX_PL;
           }
           //----------------------------------------------------------------


           //------------------------------------------------------------
           // если мышка находится на ползунке
           msx=mouse_x; msy=mouse_y;
           if ( msx > p1 && msx < p2 && msy > p3 && msy < p4 ){
                // вывести сообщение в строку статуса, если мышка на ползунке
                sprintf(STAT_str,"%s",LPLZ[a].STR); STAT_cnt=2;
                MCurPL=1; c=1;
                // если левая кнопка мышки была нажата
                if(MSKey==1){
                   LPLZ[NaPL].a=0; LPLZ[a].a=1; NaPL=a; KAL=1; MSK_PL=1;
                   // если предыдущая координата мышки не совпадает с новой:
                   // если курсор мышки передвигается- пересчитать координаты
                   // нового положения курсора мышки
                   if(r1 != r2){
                      p =divf(r, LPLZ[a].L);
                      // пересчет...
                      vl=vl+(LPLZ[a].max-LPLZ[a].min)*p;
                      // проверка диапазона
                      if (vl>LPLZ[a].max){ vl=LPLZ[a].max; }
                      if (vl<LPLZ[a].min){ vl=LPLZ[a].min; }
                      LPLZ[a].vl=vl;
                   }
                } else {
                // если левая кнопка мышки была отпущена
                   if(MSK_PL==1){
                      // передать новое значение в базу канлов
                      Val_to_Arr(LPLZ[a].na, LPLZ[a].nv,LPLZ[a].vl,CBMODYES,'p');
                      vl=LPLZ[a].vl;
                   }; MSK_PL=0;
                }
           }   else   {
               if(c==0){ MCurPL=0; }
           }
           //----------------------------------------------------------------


           //----------------------------------------------------------------
           // запомнить экран под спрайтом ползунка
           blit(scrsav,LPLZ[a].ss,
                       LPLZ[a].x1+LPLZ[a].r1,
                       LPLZ[a].y1+LPLZ[a].r2,0,0,
                       LPLZ[a].W +LPLZ[a].r3,
                       LPLZ[a].H +LPLZ[a].r4 );
            //---------------------------------------------------------------
            // нарисовать ползунок:
            // рисуем линию диапазона
            if(divi(LPLZ[a].tb, 10) >0 ){
               rect    (scrsav,LPLZ[a].X,LPLZ[a].Y,p5,p6,LPLZ[a].cb);
            }
            // пустая планка текущего значения
            if(LPLZ[a].tb%10==1 && LPLZ[a].a==1){
               rect    (scrsav,LPLZ[a].x1,LPLZ[a].y1,p7,p8,LPLZ[a].cb);
            }
            // закрашенная планка текущего значения
            if(LPLZ[a].tb%10 >1 && LPLZ[a].a==1){
               rectfill(scrsav,LPLZ[a].x1,LPLZ[a].y1,p7,p8,LPLZ[a].cb);
            }

           // вывод спрайта ползунка или центральной линии
           if (LPLZ[a].sf==1){
               if (KAL>0 && LPLZ[a].a>0){
                   draw_sprite(scrsav,LPLZ[a].sp[2],LPLZ[a].x1,LPLZ[a].y1);
                }  else  {
                   draw_sprite(scrsav,LPLZ[a].sp[LPLZ[a].a],LPLZ[a].x1,LPLZ[a].y1);
                }
           }    else    {
                rectfill(scrsav,z1,z2,z3,z4,LPLZ[a].cb);
           }
       };  msx=mouse_x; msy=mouse_y;  MsX_PL=msx; MsY_PL=msy;
   }
   //------------------------------------------------------------------------


   //------------------------------------------------------------------------
   // изменение значений и активности при нажатии соответствующих клавиш
   if (md==3){
       for(a=0;a<HPLZ;a++){
           // проверка разрешенности ползунка
           if (isPLZ_En(a)<1){ continue; }
           if (LPLZ[a].a==1){
               // активизировать следующий ползунок
               if (kb==1){
                   LPLZ[a].a=0;
                   for(b=0;b<MPLZ;b++){
                       if (a+1>=HPLZ){ LPLZ[  0].a=1; fa=  0; }
                       else          { LPLZ[a+1].a=1; fa=a+1; }; a=fa;
                       if (isPLZ_En(a)>0){ break; };  
                   }
                   break;
               }
               // активизировать предыдущий ползунок
               if (kb==2){
                   LPLZ[a].a=0;
                   for(b=0;b<MPLZ;b++){
                       if (a-1<    0){ LPLZ[HPLZ-1].a=1; fa=HPLZ-1; }
                       else          { LPLZ[   a-1].a=1; fa=   a-1; }; a=fa;
                       if (isPLZ_En(a)>0){ break; };     
                   }
                   break;
               }
               // получить текущее значение ползунка
               vl=Arr_to_Val(LPLZ[a].na,LPLZ[a].nv);
               // увеличить значение ползунка на  1%
               if (kb==3){ vo=vl+(LPLZ[a].max-LPLZ[a].min)*0.01; }
               // уменьшить значение ползунка на  1%
               if (kb==4){ vo=vl-(LPLZ[a].max-LPLZ[a].min)*0.01; }
               // увеличить значение ползунка на 10%
               if (kb==5){ vo=vl+(LPLZ[a].max-LPLZ[a].min)*0.10; }
               // уменьшить значение ползунка на 10%
               if (kb==6){ vo=vl-(LPLZ[a].max-LPLZ[a].min)*0.10; }
               if (vo > LPLZ[a].max){ vo=LPLZ[a].max; }
               if (vo < LPLZ[a].min){ vo=LPLZ[a].min; }
               Val_to_Arr(LPLZ[a].na, LPLZ[a].nv, vo, CBMODYES,'p');
               fa=a; break;
           }
       }
       // сообщение статуса ползунка
       if (LPLZ[fa].a==1){ 
           sprintf(STAT_str,"%s",LPLZ[fa].STR); STAT_cnt=10;
       }
       NaPL=fa;
   }
   //------------------------------------------------------------------------
   return;
}
//---------------------------------------------------------------------------
