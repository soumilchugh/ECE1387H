#include <stdio.h>
#include <iostream>
#include "graphics.h"
using namespace std;

/*void drawscreen() {
	int index[5][5]; 
    
    int x,y = 0;
    int x1,y1 = 0;
    int x2,y2;

    x = originalx;
    y = originaly;
    set_draw_mode (DRAW_NORMAL); 
    clearscreen(); 
    setfontsize (10);
    setlinestyle (SOLID);
    setlinewidth (1);
    setcolor (YELLOW);
    for (int i = 4; i >= 0 ; i --) {
        for (int j = 0; j < 5; j++) {
            index[j][i] = x+ 10*y; 
            t_point polypts2[5] = {{x,y}, {x+75,y}, {x+45,y+30},{x+30,y+40}, {x,y+40}};
            t_point polypts[5] = {{x+75,y+55}, {x+110,y+25},{x+175,y+25}, {x+175,y+125},{x+75,y+125}};
            fillpoly(polypts,5);
            fillpoly(polypts2,5);
            x = x + 300;
        }
        x = originalx;
        y = y + 200;
    }
    x = originalx;
    y = originaly;
    x1 = x + 300;
    y1 = y;
    y2 = y + 200;
    for (int i = 4; i >= 0; i --) {
        for (int j = 0; j < 5; j++) {
            setcolor (BLACK);
            if (j != 4) {
                drawline(x+175,y+125,x1+75,y1+125);
                drawline(x+175,y+105,x1+75,y1+105);
                drawline(x+175,y+85,x1+75,y1+85);
                drawline(x+175,y+65,x1+75,y1+65);
            }
            if (i != 0) {
            // Vertical
            drawline(x+155,y+125,x+155,y2+25);
            drawline(x+175,y+125,x+175,y2+25);
            drawline(x+135,y+125,x+135,y2+25);
            drawline(x+115,y+125,x+115,y2+25); 
        }

            x1 = x1 + 300;
            x = x + 300;
        }
        x = originalx;
        x1 = x + 300;
        y = y + 200;
        y1 = y1 + 200;
        y2 = y2 + 200;
    }

}*/



//void button_press (float x, float y) {
    /* Called whenever event_loop gets a button press in the graphics *
     * area.  Allows the user to do whatever he/she wants with button *
     * clicks.                                                        */

  //   printf("User clicked at coordinates (%f, %f)\n", x, y);
//}

/*int main() {

   int i;

   
   printf ("About to start graphics.\n");
   init_graphics("Some Example Graphics", WHITE);

   init_world (0.,0.,1000.,1000.);
   update_message("Interactive graphics example.");
    drawscreen();
    event_loop(button_press,NULL, NULL,drawscreen);
   close_graphics ();
   printf ("Graphics closed down.\n");

   return 0;
}*/