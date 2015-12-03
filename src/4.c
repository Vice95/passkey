#include <curses.h>
#include <stdlib.h>

#define ENTER 	10
#define ESCAPE 	27
#define LEN	4
void init_curses(){
        initscr();
        start_color();
        init_pair(1,COLOR_GREEN,COLOR_BLUE);
        init_pair(2,COLOR_BLUE,COLOR_WHITE);
        init_pair(3,COLOR_WHITE,COLOR_BLUE);
        curs_set(0);
        noecho();
        keypad(stdscr,TRUE);
}

void draw_menubar(WINDOW *menubar){
        wbkgd(menubar,COLOR_PAIR(1));
        waddstr(menubar,"File");
        wattron(menubar,COLOR_PAIR(3));
        waddstr(menubar,"(F1)");
        wattroff(menubar,COLOR_PAIR(3));
        wmove(menubar,0,20);
        waddstr(menubar,"Info");
        wattron(menubar,COLOR_PAIR(3));
        waddstr(menubar,"(F2)");
        wattroff(menubar,COLOR_PAIR(3));
}
WINDOW **draw_menu(int start_col){

char *fname[] = { "Nuovo ",
                  "Salva",
		  "Info",
		  "Esci"
		};
//char app[];
        int i;
        WINDOW **items;
        items=(WINDOW **)malloc(LEN*sizeof(WINDOW *));
        items[0]=newwin(10,19,1,start_col);
        wbkgd(items[0],COLOR_PAIR(2));
        box(items[0],ACS_VLINE,ACS_HLINE);
        for (i=1;i<LEN+1;i++){
		items[i]=subwin(items[0],1,17,i+1,start_col+1);     
                wprintw(items[i],"%d %s",i,fname[i-1]);
        }
	wbkgd(items[1],COLOR_PAIR(1));
        wrefresh(items[0]);
        return items;
}
void delete_menu(WINDOW **items,int count){
        int i;
        for (i=0;i<count;i++)
                delwin(items[i]);
        free(items);
}

int scroll_menu(WINDOW **items,int count,int menu_start_col){
        int key;
        int selected=0;
        while (1) {
                key=getch();
                if (key==KEY_DOWN || key==KEY_UP) {
                        wbkgd(items[selected+1],COLOR_PAIR(2));
                        wnoutrefresh(items[selected+1]);
                        if (key==KEY_DOWN) {
                                selected=(selected+1) % count;
                        } else {
                                selected=(selected+count-1) % count;
                        }
                        wbkgd(items[selected+1],COLOR_PAIR(1));
                        wnoutrefresh(items[selected+1]);
                        doupdate();
                } else if (key==KEY_LEFT || key==KEY_RIGHT) {
                        delete_menu(items,count+1);
                        touchwin(stdscr);
                        refresh();
                        items=draw_menu(20-menu_start_col);
                        return scroll_menu(items,8,20-menu_start_col);
                } else if (key==ESCAPE) {
                        return -1;
                } else if (key==ENTER) {
                        return selected;
                }
        }
}
int main(){
    int key;
    WINDOW *menubar,*messagebar;
    
    init_curses();
    
    bkgd(COLOR_PAIR(3));
    menubar=subwin(stdscr,1,80,0,0);
    messagebar=subwin(stdscr,1,79,23,1);
    draw_menubar(menubar);
    move(2,1);
    printw("PassKey 0.01 ");
    printw("ESC quits.");
    refresh();

    do {
        int selected_item;
        WINDOW **menu_items;
        key=getch();
        werase(messagebar);
        wrefresh(messagebar);
        if (key==KEY_F(1)) {
            menu_items=draw_menu(0);
            selected_item=scroll_menu(menu_items,LEN,0);
            delete_menu(menu_items,9);
            if (selected_item<0)
                wprintw(messagebar,"You haven't selected any item.");
            else if(selected_item==4){
		 delwin(menubar);
		 delwin(messagebar);
    		 endwin();
    		return 0;
		}
	     else
                wprintw(messagebar,
                  "You have selected menu item %d.",selected_item+1);
            touchwin(stdscr);
            refresh();
        } else if (key==KEY_F(2)) {
            menu_items=draw_menu(20);
            selected_item=scroll_menu(menu_items,LEN,20);
            delete_menu(menu_items,9);
            if (selected_item<0)
                wprintw(messagebar,"You haven't selected any item.");
	    else if(selected_item==1){
                 delwin(menubar);
                 delwin(messagebar);
                 endwin();
                return 0;
                }
            else
                wprintw(messagebar,
                  "You have selected menu item %d.",selected_item+1);
            touchwin(stdscr);
            refresh();
        }
    } while (key!=ESCAPE);
    
    delwin(menubar);
    delwin(messagebar);
    endwin();
    return 0;
}
