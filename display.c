

#include "includes.h"

#ifdef USE_DISPLAY

#include "displayConfig.c"

//#define DISPLAY_WRITE_POS
void DisplayPinInit(void)
{
   LPC_GPIO2->DIR |=0xff ; //data
   LPC_GPIO3->DIR |=0x0f ; //rd;rw;rs;cs
   LPC_GPIO0->DIR |=1<<3 ; //reset
   LPC_IOCON->R_PIO0_11 = 0x01;
   LPC_GPIO0->DIR |=1<<11; //b_light
   LPC_GPIO0->DATA &=~(1<<11);
}
 
void DisplayBacklight(unsigned char nyala)
{
   if(nyala==1)LPC_GPIO0->DATA |=1<<11;
   else	 LPC_GPIO0->DATA &=~(1<<11);
   return;
}

void DisplaySendCommand(WORD comm,WORD data) 
{
 	DisplayTxCmd(comm);
	DisplayTxData(data);  
}

void DisplayGoToXY(WORD x, WORD y)
{  
	#ifdef DISPLAY_WRITE_POS
  DisplaySendCommand(ILI9325_COMMANDS_VERTICALGRAMADDRESSSET,319-x);
  DisplaySendCommand(ILI9325_COMMANDS_HORIZONTALGRAMADDRESSSET,y);  
	#else
  
	DisplayTxCmd(0x0020);				/* GRAM Vertical/Horizontal ADDR Set(AD0~AD7) */
	DisplayTxData(OFS_RAW + x);
	DisplayTxCmd(0x0021);				/* GRAM Vertical/Horizontal ADDR Set(AD8~AD16) */
	DisplayTxData(OFS_COL + y);
	#endif
}

void DisplaySetWindow(WORD x0, WORD y0, WORD x1, WORD y1)
{
	#ifdef DISPLAY_WRITE_POS
	DisplaySendCommand(ILI9325_COMMANDS_HORIZONTALADDRESSSTARTPOSITION,y0);
	DisplaySendCommand(ILI9325_COMMANDS_HORIZONTALADDRESSENDPOSITION,y1);
	DisplaySendCommand(ILI9325_COMMANDS_VERTICALADDRESSSTARTPOSITION, 319-x1);
	DisplaySendCommand(ILI9325_COMMANDS_VERTICALADDRESSENDPOSITION, 319-x0);
	DisplayGoToXY(x0,y0);	
	DisplayTxCmd(ILI9325_COMMANDS_WRITEDATATOGRAM);
	#else
	DisplayTxCmd(0x0050);				/* Horizontal RAM Start ADDR */
	DisplayTxData(OFS_COL + x0);
	DisplayTxCmd(0x0051);				/* Horizontal RAM End ADDR */
	DisplayTxData(OFS_COL + x1);
	DisplayTxCmd(0x0052);				/* Vertical RAM Start ADDR */
	DisplayTxData(OFS_RAW + y0);
	DisplayTxCmd(0x0053);				/* Vertical End ADDR */
	DisplayTxData(OFS_RAW + y1);
	DisplayGoToXY(x0,y0);	
	DisplayTxCmd(0x0022);			/* Write Data to GRAM */
	#endif
}


 



ScrCol_t ScrCol = { FrontG, BackG };
ScrCol_t* pScrCol = &ScrCol;


//static inline
INT root_i(INT x){
    INT s=1, s2=1;
    do {
        s=(x/s+s)/2;
        s2=s+1;
        if (s*s<=x && x<s2*s2) break;
    } while(1);
    return s;
}
//static inline 
DOUBLE root(DOUBLE x){
    DOUBLE s=1, s2=1;
    if (x<=0) return 1;
    do { s2=s; s=(x/s+s)/2; } while(s2!=s);
    return s;
}


 //inline 
void Display_ChrCol(WORD fg, WORD bg)
{
	pScrCol->Fg = fg;
	pScrCol->Bg = bg;
}
#if 1
#define SetPixel(col, row)	DisplaySetWindow(col,row,col,row)
#else
void SetPixel(WORD col, WORD row)
{
	//DisplaySetRect(col,col,row,row);
	DisplaySetWindow(col,col,row,row);
}
#endif

void  PutPixel(WORD col, WORD row,WORD colour)
{
	SetPixel(col,row);
	DisplayTxData(colour);
}
void DisplayFillRect(DWORD x, DWORD width, DWORD y, DWORD height, WORD colour)
{
	volatile DWORD n;

	if((width < x) || (height < y) ) return;
	
	DisplaySetWindow(x,y,width,height);
	n = (width-x+1) * (height-y+1);
	
	do {
		DisplayTxData(colour);
	} while (--n);

}

void DisplayDrawLine(int xs,int ys,int xe,int ye, int width,WORD colour)
{
	/* Bresenham Algorithum */
	int  x,y,n,sx,sy;
	int  e,x_ct,y_ct;
	int w,ww;
	//int temp;

	if(width<=0) return;

	w=width-1;
	ww=w;
	x_ct = xe - xs;
	y_ct = ye - ys;

	if (x_ct>0x80000000) sx = -1; else	 sx =1;
	if (y_ct>0x80000000) sy = -1; else	 sy =1;

	x_ct  = ABS(x_ct);
	y_ct  = ABS(y_ct);
	x = xs;
	y = ys;

	if ( x_ct >= y_ct )
	{
		e=x_ct;
		for(n=0; n<=x_ct;++n ){
			while(w)
			{
			PutPixel(x,y+w,colour);
			PutPixel(x,y-w,colour);
			w--;
			}
			PutPixel(x,y,colour);
			x +=sx;
			e += 2*y_ct;
				if (e>=2*x_ct){
					e -= 2*x_ct;
					y +=sy;
				}
			w=ww;
		}
	}
	else
	{
		e=y_ct;
		for(n=0; n<=y_ct;++n ){
			while(w)
				{
				PutPixel(x+w,y,colour);
				PutPixel(x-w,y,colour);
				w--;
				}
			PutPixel(x,y,colour);
			y +=sy;
			e += 2*x_ct;
				if (e>=2*y_ct){
					e -= 2*y_ct;
					x +=sx;
				}
			w=ww;
		}
	}

	
}

#if 1
void DisplayDrawCircle(INT16 x_ct,INT16 y_ct,LONG diameter, WORD colour)
{
	/* Bresenham Midpoint Algorithum */
   LONG cx, cy, d, dH, dD;
	LONG radius=diameter/2;
    d   = 1 - radius;
    dH  = 3;
    dD  = 5 - 2 * radius;
    cy  = radius;

    for (cx = 0; cx <= cy; cx++) {
        if (d < 0) {
            d   += dH;
            dH  += 2;
            dD  += 2;
        }
        else{
            d   += dD;
            dH  += 2;
            dD  += 4;
            --cy;
        }

        PutPixel( cy + x_ct,  cx + y_ct, colour);	/* Between   0- 45 */
        PutPixel( cx + x_ct,  cy + y_ct, colour);	/* Between  45- 90 */
        PutPixel(-cx + x_ct,  cy + y_ct, colour);	/* Between  90-135 */
        PutPixel(-cy + x_ct,  cx + y_ct, colour);	/* Between 135-180 */
        PutPixel(-cy + x_ct, -cx + y_ct, colour);	/* Between 180-225 */
        PutPixel(-cx + x_ct, -cy + y_ct, colour);	/* Between 225-270 */
        PutPixel( cx + x_ct, -cy + y_ct, colour);	/* Between 270-315 */
        PutPixel( cy + x_ct, -cx + y_ct, colour);	/* Between 315-360 */
    }
}
#else
void DisplayDrawCircle(INT16 x_ct,INT16 y_ct,LONG diameter, WORD colour)
{
	LONG cx = 0; 
	LONG cy=diameter/2;
	LONG *px, *py;
	LONG tmp;
    LONG dx, dy, x_sign, num_eigth, r_root2;
	LONG y_sign =0;
    DOUBLE d;
	INT16 li;
	PLOT_XY center;
	PLOT_XY mirror_center = center, now_center, start_po[8], end_po[8];
    
    center.x=x_ct;
    center.y=y_ct;

	

    r_root2 = (diameter>3)? root(diameter*diameter/8) :1;
    tmp = r_root2*r_root2*8-diameter*diameter;
    if (ABS(tmp)>ABS(tmp+8*(2*r_root2+1))) r_root2++;	/* near by (r*Å„2) */

    d = -diameter*diameter + 4*cy*cy -4*cy +2;
    dx = 4;
    dy = -8*cy+8;
	
    
    if ((diameter&1) ==0){
        mirror_center.x++;
        mirror_center.y++;
    }

    /* Clipper */
    for(num_eigth=0; num_eigth <8; num_eigth++){
        start_po[num_eigth].y=diameter/2;
        start_po[num_eigth].x=0;
        end_po[num_eigth].x=end_po[num_eigth].y=r_root2;
    }

    for(li=0;li<4;li++){
        if (li==0) { cy = center.y-(MAX_Y-1); y_sign=-1;}
        if (li==1) { cy = mirror_center.x;    y_sign= 1;}
        if (li==2) { cy = mirror_center.y;    y_sign= 1;}
        if (li==3) { cy = center.x-(MAX_X-1); y_sign=-1;}

        if (ABS(cy)>=(diameter/2)) {
            if (((li==0 || li==3) && cy<0) || ((li==1 || li==2) && cy>0)) continue;	/* Circle is Inside of Region */
            else return ;	/* Circle is Outside of Region */
        }

        tmp = diameter*diameter -4*cy*cy;
        cx = root_i(tmp/4); /* n=tmp/4; if (tmp%4) n++; */
        tmp -= 4*cx*cx;
        if (ABS(tmp)>=ABS(tmp -8*cx-4)) cx++;

		/* Clipper Divided Octal Regions */
        if (cy*y_sign>r_root2){
            /* 1,2 -> 3,4 -> 5,6 -> 7,0 */
            if (start_po[li*2+1].x<ABS(cx)) {
                start_po[li*2+1].y = ABS(cy);
                start_po[li*2+1].x = ABS(cx);
            }
            if (start_po[(li*2+2)%8].x<ABS(cx)) {
                start_po[(li*2+2)%8].y = ABS(cy);
                start_po[(li*2+2)%8].x = ABS(cx);
            }
        }
        else {
            start_po[li*2+1].y = start_po[(li*2+2)%8].y = 0;        /* Set Out of Region */
            start_po[li*2+1].x = start_po[(li*2+2)%8].x = diameter;	/* Set Out of Region */
            if (cy*y_sign<=r_root2 && cy*y_sign>0){
                /* Set Out of Region... 1,2 -> 3,4 -> 5,6 -> 7,0 */
                /* 0,3 -> 2,5 -> 4,7 -> 6,1 */
                if (end_po[li*2].x > ABS(cy)) {
                    end_po[li*2].y = ABS(cx);
                    end_po[li*2].x = ABS(cy);
                }
                if (end_po[(li*2+3)%8].x > ABS(cy)) {
                    end_po[(li*2+3)%8].y = ABS(cx);
                    end_po[(li*2+3)%8].x = ABS(cy);
                }
            }
            else {
                start_po[li*2].y = start_po[(li*2+3)%8].y = 0;
                start_po[li*2].x = start_po[(li*2+3)%8].x = diameter;
                if (cy*y_sign<=0 && cy*y_sign>-r_root2){
				    /* Set Out of Region... 0,3 -> 2,5 -> 4,7 -> 6,1 */
					/* 4,7 -> 6,1 -> 0,3 -> 2,5 */
                    if (start_po[(li*2+4)%8].x < ABS(cy)) {
                        start_po[(li*2+4)%8].y = ABS(cx);
                        start_po[(li*2+4)%8].x = ABS(cy);
                    }
                    if (start_po[(li*2+7)%8].x < ABS(cy)) {
                        start_po[(li*2+7)%8].y = ABS(cx);
                        start_po[(li*2+7)%8].x = ABS(cy);
                    }
                }
                else {
                    start_po[(li*2+4)%8].y = start_po[(li*2+7)%8].y = 0;
                    start_po[(li*2+4)%8].x = start_po[(li*2+7)%8].x = diameter;
					/* Set Out of Region...  4,7 -> 6,1 -> 0,3 -> 2,5 */
					/* 5,6 -> 7,0 -> 1,2 -> 3,4 */
                    if (end_po[(li*2+5)%8].x>ABS(cx)) {
                        end_po[(li*2+5)%8].y = ABS(cy);
                        end_po[(li*2+5)%8].x = ABS(cx);
                    }
                    if (end_po[(li*2+6)%8].x>ABS(cx)) {
                        end_po[(li*2+6)%8].y = ABS(cy);
                        end_po[(li*2+6)%8].x = ABS(cx);
                    }
                }
            }
        }
    }

    /* Draw Circle main loop */
    for(num_eigth=0; num_eigth<8 ; num_eigth++){
		/* Circle Divided Octal Regions */
        if (num_eigth<4){
                now_center.y = y_ct; y_sign=1; }            /* 0,1,2,3 */
        else{   now_center.y = mirror_center.y; y_sign=-1; }    /* 4,5,6,7 */
        if ((num_eigth%6)<=1){
                now_center.x = x_ct;  x_sign=1; }           /* 0,1,6,7 */
        else {  now_center.x = mirror_center.x; x_sign=-1; }    /* 2,3,4,5 */
        if ((num_eigth%4)%3){
                px = &cx; py = &cy; }   /* 0,3,4,7 */
        else {  px = &cy; py = &cx; }   /* 1,2,5,6 */

        /* initial value  */
        cy=start_po[num_eigth].y;
        cx=start_po[num_eigth].x;

        /* diameter */
        d = 4*cx*cx+4*cy*cy -4*cy+2 - diameter * diameter;
        dx = 8*cx+4;
        dy = -8*cy+8;

        /* Set Point */
        for (;cx<=end_po[num_eigth].x;cx++){
            if (d>0) {
                d += dy;
                dy+=8;
                cy--;
            }
            PutPixel((*px)*x_sign + now_center.x, (*py)*y_sign + now_center.y, colour);
            d += dx;
            dx+=8;
        }
    }
}
#endif
void DisplayFillCircle(INT16 x_ct,INT16 y_ct,LONG diameter, WORD colour)
{
	/* Bresenham Midpoint Algorithum */
	LONG cx, cy, d, dH, dD, n;
	LONG radius= diameter/2;
	
    d   = 1 - radius;
    dH  = 3;
    dD  = 5 - 2 * radius;
    cy  = radius;

    for (cx = 0; cx <= cy; cx++) {
        if (d < 0) {
            d   += dH;
            dH  += 2;
            dD  += 2;
        }
        else{
            d   += dD;
            dH  += 2;
            dD  += 4;
            --cy;
        }

		/* Between 0-45deg */
		n = 2*cy;
		do{
			PutPixel((cy-n)+ x_ct,cx + y_ct,colour);
		} while (n--);

		/* Between 45-90deg */
		n = 2*cx;
		do{
			PutPixel((cx-n)+ x_ct,cy + y_ct,colour);
		} while (n--);

		/* Between 270-315deg */
		n = 2*cx;
		do{
			PutPixel((cx-n)+ x_ct,-cy + y_ct,colour);
		} while (n--);

		/* Between 315-360deg */
		n = 2*cy;
		do{
			PutPixel((cy-n)+ x_ct,-cx + y_ct,colour);
		} while (n--);

    }
}

void DisplayDrawRect(DWORD xs, DWORD xe, DWORD ys, DWORD ye, WORD colour)
{
	volatile DWORD n;
	
	n = xe - xs;
	//n=xs;
	do{
		PutPixel((xe-n),ys,colour);
	} while (n--);

	n = xe - xs;
	//n=xs;
	do{
		PutPixel((xe-n),ye,colour);
	} while (n--);

	n = ye - ys;
	//n=ys;
	do{
		PutPixel(xs,(ye-n),colour);
	} while (n--);

	n = ye - ys;
	//n=ys;
	do{
		PutPixel(xe,(ye-n),colour);
	} while (n--);
	
}

void DisplayTilt(INT x,INT y,INT dia,INT ang,WORD color)
{
	INT xsin,ycos;
	INT r;
	FLOAT angrad;
	

	angrad=((float)ang/(float)180);	
	angrad=angrad*22/7;
	r=dia*100/2;
	
	//clear old circle
	//DisplayFillCircle(x,y, dia, PANEL_CLR);
	
	xsin=(INT)((float)r*sin(angrad));
	ycos=(INT)((float)r*cos(angrad));
	xsin/=100;
	ycos/=100;
			DisplayFillCircle(x,y, dia, PANEL_CLR);
			DisplayDrawLine(x-xsin,y-ycos,x+xsin,y+ycos,1, color);
		
}

void DisplayInit()
 {
		BYTE a;
		DWORD cnt;
		DisplayPinInit();
		LPC_GPIO2->DATA=0;
	
		TFT_LCD_RD_HIGH;
		TFT_LCD_WR_HIGH;
		TFT_LCD_CS_HIGH;
		TFT_LCD_RS_HIGH;
		//Reset display  	
		TFT_LCD_RST_LOW;
		WaitMs(100);
		TFT_LCD_RST_HIGH;
		WaitMs(100);


		for(a=0;a<48;a++)
		{
			if(DisplayConfAdd[a])
					DisplaySendCommand(DisplayConfAdd[a],DisplayConfData[a]);
			else 
					WaitMs(DisplayConfData[a]);
		}
		//set background color
		DisplaySetWindow(0,0,239,319);	 
		for(cnt=0;cnt<(320*240);cnt++)DisplayTxData(BG_CLR);
 }
  
 
 
 void DisplayTest()
 {
	 DWORD cnt;
	 //DisplayInit();	 
	 #ifdef DISPLAY_WRITE_POS 
	 DisplayBacklight(ON);
	 WaitMs(1000);
	 DisplayBacklight(OFF);
	 WaitMs(1000);
	 DisplayBacklight(ON);
	 WaitMs(1000);
	 //printf("LCD FILL..");	 	 	
	 DisplaySetWindow(0,0,319,239);	 
	 for(cnt=0;cnt<(320*240);cnt++){DisplayTxData(BLUE);	} 	
	 WaitMs(1000);
	 DisplaySetWindow(0,0,319,239);	 
	 for(cnt=0;cnt<(320*240);cnt++){DisplayTxData(RED);}	 
	 WaitMs(1000);
	 DisplaySetWindow(0,0,319,239);	 
	 for(cnt=0;cnt<(320*240);cnt++){DisplayTxData(GREEN);}	 
	 WaitMs(1000);
	#else
	 DisplaySetWindow(0,0,239,319);	 
	 for(cnt=0;cnt<(320*240);cnt++)DisplayTxData(BLUE);	

	 
	 WaitMs(1000);
	 
	//DisplayDrawLine(100, 120, 150, 120, WHITE);
	//DisplayDrawLine(100, 120, 150, 150, BLUE);
	//DisplayDrawLine(100, 110, 150, 120, RED);
	//DisplayDrawLine(100, 90,70, 30, BLACK);
	//DisplayDrawCircle(70,160, 100, BLUE);
	//DisplayFillCircle(0,0, 100, BLUE);
	//DisplayDrawRect(50, 100, 80, 120, RED);
	//DisplayFillRect(100,140, 30, 70,BLUE);
	//DisplayFillCircle(120,150, 200, YELLOW);
		#if 0
		DisplayTilt(120,150,200,10,MAGENTA)	;
		DisplayTilt(120,150,200,20,RED)	;
		DisplayTilt(120,150,200,30,BLUE)	;
		DisplayTilt(120,150,200,92,CYAN)	;
		DisplayTilt(120,150,200,181,WHITE)	;
		DisplayTilt(120,150,200,-10,BLACK)	;
		#endif
	 #endif
	 //printf("OK\r");	 	 	
 }
#if 0

void DisplayInit()
{
	//setGPIO
	DISPLAY_DATAPORT_AS_OUT;
	SetBitDir(DISP_RES_PORT,DISP_RES_PIN)
	SetBitDir(DISP_DC_PORT,DISP_DC_PIN)
	SetBitDir(DISP_CS_PORT,DISP_CS_PIN)
	SetBitDir(DISP_WR_PORT,DISP_WR_PIN)
      	SetBitDir(DISP_RD_PORT,DISP_RD_PIN)

	//set DMA if any
	
}


/**************************************************************************/
/*! 
	Put Pixel Position(Basis Function).
*/
/**************************************************************************/
//static inline 


/**************************************************************************/
/*! 
	Put Pixel.
*/
/**************************************************************************/
//inline 



/**************************************************************************/
/*! 
	Fill Rectangle.
*/
/**************************************************************************/
//inline 


/**************************************************************************/
/*! 
	DRAW Line Function(There is no cripper yet).
*/
/**************************************************************************/
//inline 
void 


/**************************************************************************/
/*! 
	DRAW Circle Function With no Filler.
*/
/**************************************************************************/
#if 1 /* More Accuracy method by http://dencha.ojaru.jp thanks! */
//inline 


#else
//inline void 
Display_DrawCircle_If(int x_ct,int y_ct,int radius, WORD colour)
{
	/* Bresenham Midpoint Algorithum */
   LONG cx, cy, d, dH, dD;

    d   = 1 - radius;
    dH  = 3;
    dD  = 5 - 2 * radius;
    cy  = radius;

    for (cx = 0; cx <= cy; cx++) {
        if (d < 0) {
            d   += dH;
            dH  += 2;
            dD  += 2;
        }
        else{
            d   += dD;
            dH  += 2;
            dD  += 4;
            --cy;
        }

        PutPixel( cy + x_ct,  cx + y_ct, colour);	/* Between   0- 45 */
        PutPixel( cx + x_ct,  cy + y_ct, colour);	/* Between  45- 90 */
        PutPixel(-cx + x_ct,  cy + y_ct, colour);	/* Between  90-135 */
        PutPixel(-cy + x_ct,  cx + y_ct, colour);	/* Between 135-180 */
        PutPixel(-cy + x_ct, -cx + y_ct, colour);	/* Between 180-225 */
        PutPixel(-cx + x_ct, -cy + y_ct, colour);	/* Between 225-270 */
        PutPixel( cx + x_ct, -cy + y_ct, colour);	/* Between 270-315 */
        PutPixel( cy + x_ct, -cx + y_ct, colour);	/* Between 315-360 */
    }
}
#endif

/**************************************************************************/
/*! 
	DRAW Circle with Filler.
*/
/**************************************************************************/
//inline 


/**************************************************************************/
/*! 
	DRAW Rectangle Line with no Filler.
*/
/**************************************************************************/
//inline 





/**************************************************************************/
/*! 
	Put SJIS Kanji Font With Transparency.
*/
/**************************************************************************/
//static 
void 
putkanji_t(WORD col, WORD row, BYTE* pkanji, WORD colour)
{

	BYTE*	ptemp;
	int 		i,j,l;
	int 		k,m;

	l = CurrentKanjiDat->X_Size / 8;
	m = CurrentKanjiDat->X_Size % 8;

	ptemp = GetPtr_Kanji(((*pkanji) << 8) | *(pkanji+1));

	for(i=0; i<CurrentKanjiDat->Y_Size;i++)
	{
		for(k=0; k < l ;k++){
			
			for(j=0; j< 8;j++){

				if((*(ptemp)<<j)&0x80){
						SetPixel(col +j +(k*8),row +i);
						Display_wr_dat_if(colour);
					}
			}
			ptemp++;
		}

		for(j=0; j< m ;j++){

			if((*(ptemp)<<j)&0x80){
					SetPixel(col +j +(k*8),row +i);
					Display_wr_dat_if(colour);
				}
				
		}
		
		if (m != 0) ptemp++;	
	
	}

}


/**************************************************************************/
/*! 
	Put SJIS ANK Font With Transparency.
*/
/**************************************************************************/
//static 
void 
putank_t(WORD col, WORD row, BYTE* pank, WORD colour)
{

	BYTE*	ptemp;
	int 		i,j,l;
	int 		k,m;

	l = CurrentAnkDat->X_Size / 8;
	m = CurrentAnkDat->X_Size % 8;

	ptemp = GetPtr_Ank(*pank);

	for(i=0; i<CurrentAnkDat->Y_Size;i++)
	{
		for(k=0; k < l ;k++){
			
			for(j=0; j< 8;j++){

				if((*(ptemp)<<j)&0x80){
						SetPixel(col +j +(k*8),row +i);
						Display_wr_dat_if(colour);
					}
			}
			ptemp++;
		}

		for(j=0; j< m ;j++){

			if((*(ptemp)<<j)&0x80){
					SetPixel(col +j +(k*8),row +i);
					Display_wr_dat_if(colour);
				}
		}
		if (m != 0) ptemp++;	

	}
}


/**************************************************************************/
/*! 
	Put SJIS Kanji Font With Occupy.
*/
/**************************************************************************/
void putkanji(WORD col, WORD row, BYTE* pkanji, WORD fg,WORD bg)
{

	BYTE*	ptemp;
	int 		i,j,l;
	int 		k,m;
	WORD 	wd;

	l = CurrentKanjiDat->X_Size / 8;
	m = CurrentKanjiDat->X_Size % 8;

	ptemp = GetPtr_Kanji(((*pkanji) << 8) | *(pkanji+1));
	
	Display_rect_if(col, col + CurrentKanjiDat->X_Size - 1,
					row, row + CurrentKanjiDat->Y_Size - 1);


	for(i=0; i<CurrentKanjiDat->Y_Size;i++)
	{
		for(k=0; k < l ;k++){
			
			for(j=0; j< 8;j++){
				wd = (((*(ptemp))<<j)&0x80) ?  fg : bg;
			#if defined(DISABLE_FASTWRITE)
				SetPixel(col +j +(k*8),row +i);
			#endif
				Display_wr_dat_if(wd);
			}
			ptemp++;
		}

		for(j=0; j< m ;j++){
				wd = (((*(ptemp))<<j)&0x80) ?  fg : bg;
			#if defined(DISABLE_FASTWRITE)
				SetPixel(col +j +(k*8),row +i);
			#endif
				Display_wr_dat_if(wd);
		}

		if (m != 0) ptemp++;	
	}

}


/**************************************************************************/
/*! 
	Put SJIS ANK Font With Occupy.
*/
/**************************************************************************/
void putank(WORD col, WORD row, BYTE* pank, WORD fg,WORD bg)
{

	BYTE*	ptemp;
	int 		i,j,l;
	int 		k,m;
	WORD 	wd;

	l = CurrentAnkDat->X_Size / 8;
	m = CurrentAnkDat->X_Size % 8;

	ptemp = GetPtr_Ank(*pank);

	Display_rect_if(col, col + CurrentAnkDat->X_Size - 1,
					row, row + CurrentAnkDat->Y_Size - 1);


	for(i=0; i<CurrentAnkDat->Y_Size;i++)
	{
		for(k=0; k < l ;k++){
			
			for(j=0; j< 8;j++){
				wd = (((*(ptemp))<<j)&0x80) ?  fg : bg;
			#if defined(DISABLE_FASTWRITE)
				SetPixel(col +j +(k*8),row +i);
			#endif
				Display_wr_dat_if(wd);
			}
			ptemp++;
		}

		for(j=0; j< m ;j++){
				wd = (((*(ptemp))<<j)&0x80) ?  fg : bg;
			#if defined(DISABLE_FASTWRITE)
				SetPixel(col +j +(k*8),row +i);
			#endif
				Display_wr_dat_if(wd);
		}

		if (m != 0) ptemp++;	
	}

}


/**************************************************************************/
/*! 
	Put SJIS Fonts Basic.
*/
/**************************************************************************/
int Display_Puts_If(WORD col, WORD row, BYTE* pch, BYTE trans)
{

	while(*pch){

		if (col >= MAX_X || row >= MAX_Y) return 0;

		if(((*pch >= 0x80)&&(*pch < 0xA0))||((*pch >= 0xE0)&&(*pch <= 0xFF)))
	  		{
				if(trans == TRANSPARENT) putkanji_t(col,row,pch,pScrCol->Fg);
				else					 putkanji  (col,row,pch,pScrCol->Fg,pScrCol->Bg);
				
				pch +=2;
				col += CurrentKanjiDat->X_Size;
	  		}
		else
			{
				if(trans == TRANSPARENT) putank_t(col,row,pch,pScrCol->Fg);
				else					 putank  (col,row,pch,pScrCol->Fg,pScrCol->Bg);

				pch +=1;
				col += CurrentAnkDat->X_Size;
			}
	}

	return 1;
}

/**************************************************************************/
/*! 
	Put SJIS Fonts Advanced.
*/
/**************************************************************************/
int Display_Puts_If_Ex(WORD col, WORD row, BYTE* pch, BYTE trans, WORD fg,WORD bg)
{

	while(*pch){

		if (col >= MAX_X || row >= MAX_Y) return 0;

		if(((*pch >= 0x80)&&(*pch < 0xA0))||((*pch >= 0xE0)&&(*pch <= 0xFF)))
	  		{
				if(trans == TRANSPARENT) putkanji_t(col,row,pch,fg);
				else					 putkanji  (col,row,pch,fg,bg);
				
				pch +=2;
				col += CurrentKanjiDat->X_Size;
	  		}
		else
			{
				if(trans == TRANSPARENT) putank_t(col,row,pch,fg);
				else					 putank  (col,row,pch,fg,bg);

				pch +=1;
				col += CurrentAnkDat->X_Size;
			}
	}

	return 1;
}


#endif
/* End Of File ---------------------------------------------------------------*/


#endif	//USE_DISPLAY

