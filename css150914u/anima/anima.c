// coding: cp866
//---------------------------------------------------------------------------
// вывод картинок анимации
#include <allegro.h>
#include "../include.h"
#include "../define.h"
#include "anima.h"


//---------------------------------------------------------------------------
vd Animation(_i md){
   _i  a=0, b=0, w=0, sc=0, fl=0, aX=0, aY=0;

   //------------------------------------------------------------------------
   // загрузка спрайтов при чтении конфигурации
   // (бывают слеты на функции clear_to_color
   if (md==0){
       for(a=0;a<MANI;a++){
           if (LANI[a].nv==0){ continue; }
           pcxani=load_bitmap(LANI[a].fp,pal);
           if (pcxani){
               if (LANI[a].hp<=0){ LANI[a].hp=1; }
               if (LANI[a].hp>MASP){ LANI[a].hp=MASP; }
               for(b=0;b<LANI[a].hp;b++){
                   w=LANI[a].W*b+b;
                   clear_to_color(LANI[a].sp[b],0);
                   blit (pcxani,LANI[a].sp[b],0+w,0,0,0,LANI[a].W,LANI[a].H);
               }
           }   else   {
               DPrn(0,0,"Ошибка загрузки файла анимации [%s]\n",LANI[a].fp);
               LANI[a].vw=0;
           }
           destroy_bitmap(pcxani);
           LANI[a].x1=LANI[a].X;
           LANI[a].y1=LANI[a].Y;
           Val_to_Arr(LANI[a].na, LANI[a].nf,0, CBMODNO, 'a');
           LANI[a].vx=LANI[a].vw;
       }
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // отображение картинок анимации
   if (md==1){
       for(a=0;a<MANI;a++){
           if (LANI[a].nv==0){ continue; }
           fl=(_i)Arr_to_Val(LANI[a].na, LANI[a].nf);
           // выключение анимации
           if (fl>=400000){
               LANI[a].vw =LANI[a].vx;
               Val_to_Arr(LANI[a].na, LANI[a].nf, 0, CBMODNO, 'a');
           }
           if (LANI[a].vw>0){
               // получить текущие значения связанных переменных
               sc=(_i)Arr_to_Val(LANI[a].na, LANI[a].nv);    // номер спрайта
               if (sc <=0){ sc=0; }
               if (sc >=LANI[a].hp){ sc=LANI[a].hp-1; }
               if (LANI[a].nf>0){
                  aX=(_i)Arr_to_Val(LANI[a].na, LANI[a].nx); // изменен по X
                  aY=(_i)Arr_to_Val(LANI[a].na, LANI[a].ny); // изменен по Y
               }
               // запомнить экран под спрайтом
               blit( scrsav,    LANI[a].ss,
                     LANI[a].X, LANI[a].Y,             0,          0,
                     LANI[a].W, LANI[a].H
                   );
               // отобразить картинку спрайта
               draw_sprite( scrsav,
                            LANI[a].sp[sc],
                            LANI[a].X,
                            LANI[a].Y
                          );
               //------------------------------------------------------------
               // запомнить координаты вывода спрайта (для последующ.востан.)
               LANI[a].sX=LANI[a].X;
               LANI[a].sY=LANI[a].Y;
               //------------------------------------------------------------
               if (fl>0){
                   LANI[a].X=LANI[a].X+aX;
                   LANI[a].Y=LANI[a].Y+aY;
                   // выход за границу X
                   if (LANI[a].X > LANI[a].XM){
                       LANI[a].X = LANI[a].XM;
                       Val_to_Arr(LANI[a].na, LANI[a].nf, 200001, CBMODNO, 'a');
                   }
                   if (LANI[a].X < LANI[a].x1){
                       LANI[a].X = LANI[a].x1;
                       Val_to_Arr(LANI[a].na, LANI[a].nf, 200002, CBMODNO, 'a');
                   }
                   // выход за границу Y
                   if (LANI[a].Y > LANI[a].YM){
                       LANI[a].Y = LANI[a].YM;
                       Val_to_Arr(LANI[a].na, LANI[a].nf, 200003, CBMODNO, 'a');
                   }
                   if (LANI[a].Y < LANI[a].y1){
                       LANI[a].Y = LANI[a].y1;
                       Val_to_Arr(LANI[a].na, LANI[a].nf, 200004, CBMODNO, 'a');
                   }
               }
               //------------------------------------------------------------
           }
       }
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // восстановление экрана под спрайтом
   if (md==2){
       for(a=HANI;a>=0;a--){
           if (LANI[a].nv==0){ continue; }
           // восстановление запомненного экрана
           if (LANI[a].vw>0){
               blit( LANI[a].ss,     scrsav,
                              0,          0,  LANI[a].sX ,  LANI[a].sY,
                     LANI[a].W ,  LANI[a].H
                   );
           }
       }
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // остановка и закрытие анимации по нажатию кнопки
   if (md==3){
       for(a=0;a<MANI;a++){
           if (LANI[a].nv==0){ continue; }
           fl=(_i)Arr_to_Val(LANI[a].na, LANI[a].nf);
           if (LANI[a].vw==a+MANI*2 && fl<300000 ){
               if(LANI[a].nf>0){ Val_to_Arr(LANI[a].na, LANI[a].nf, 300000, CBMODNO, 'a'); }
               else            { Val_to_Arr(LANI[a].na, LANI[a].nf, 400000, CBMODNO, 'a'); }
           }
       }
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // остановка и закрытие анимации
   if (md==4){
       for(a=0;a<MANI;a++){
           if (LANI[a].nv==0){ continue; }
           fl=(_i)Arr_to_Val(LANI[a].na, LANI[a].nf);
           if (LANI[a].vw==a+MANI*3 && fl<300000 ){
               if(LANI[a].nf>0){ Val_to_Arr(LANI[a].na, LANI[a].nf, 300000, CBMODNO, 'a'); }
               else            { Val_to_Arr(LANI[a].na, LANI[a].nf, 400000, CBMODNO, 'a'); }
           }
       }
   }
   //------------------------------------------------------------------------
   return;
}
//---------------------------------------------------------------------------
