
#include<gtk/gtk.h>
#include<string.h>
 
void destroy(GtkWidget *widget,gpointer data)
{gtk_main_quit();
}
void hello( GtkWidget *button,
                   gpointer   data )

{	pid_t x;
	char user[50],path[100];
	strcpy(path,"/home/");
	getlogin_r(user,50);
	strcat(path,user);
	x = fork();
	if(x == 0)
	{
		strcat(path,"/snp");
		execlp(path,"snp",NULL);
		_exit(0);
	}
	wait(NULL);
	gtk_main_quit ();

}
void mnu(GtkWidget *menu_item,gpointer data)
{
if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)),"New")==0)
{g_print("you create new\n");
}
if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)),"Exit")==0)
{
gtk_main_quit();}
}
void click(GtkWidget *widget,gpointer data)
{
g_print("%s\n",gtk_entry_get_text(GTK_ENTRY(data)));
//gtk_entry_set_text(GTK_ENTRY(data),"ENTER TEXT");

}
int main(int argc, char *argv[])
{
 GtkWidget *window ;
GtkWidget *button,*menu_bar,*menu_item,*file_menu,*run_menu,*help_menu;
GtkWidget *container, *entry;
gtk_init(&argc,&argv);
  window=gtk_window_new (GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(window),"Myapp");
gtk_container_set_border_width(GTK_CONTAINER(window),20);

g_signal_connect (window,"destroy",G_CALLBACK (destroy),NULL);
menu_bar=gtk_menu_bar_new();
file_menu=gtk_menu_new();
run_menu=gtk_menu_new();
help_menu=gtk_menu_new();
menu_item=gtk_menu_item_new_with_label("File");
gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item),file_menu);
gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar),menu_item);

menu_item=gtk_menu_item_new_with_label("Run");
gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item),run_menu);
gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar),menu_item);

menu_item=gtk_menu_item_new_with_label("Help");
gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item),help_menu);
gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar),menu_item);

menu_item=gtk_menu_item_new_with_label("New");
gtk_menu_shell_append(GTK_MENU_SHELL(file_menu),menu_item);
g_signal_connect (menu_item,"activate",G_CALLBACK (mnu),NULL);


menu_item=gtk_menu_item_new_with_label("Exit");
gtk_menu_shell_append(GTK_MENU_SHELL(file_menu),menu_item);
g_signal_connect (menu_item,"activate",G_CALLBACK (mnu),NULL);


menu_item=gtk_menu_item_new_with_label("about myapp");
gtk_menu_shell_append(GTK_MENU_SHELL(help_menu),menu_item);

menu_item=gtk_menu_item_new_with_label("run program");
gtk_menu_shell_append(GTK_MENU_SHELL(run_menu),menu_item);
g_signal_connect (menu_item,"activate",G_CALLBACK (hello),NULL);

menu_item=gtk_menu_item_new_with_label("myapp help");
gtk_menu_shell_append(GTK_MENU_SHELL(help_menu),menu_item);


entry=gtk_entry_new();

button=gtk_button_new_with_mnemonic("Click Me");

g_signal_connect (G_OBJECT(button),"clicked",G_CALLBACK (click),entry);
//g_signal_connect (G_OBJECT(button),"clicked",G_CALLBACK (open_dialog),window);
g_signal_connect (entry,"activate",G_CALLBACK (click),entry);
container=gtk_hbox_new(0,0);
gtk_box_pack_start(GTK_BOX(container),menu_bar,0,0,0);
gtk_box_pack_start(GTK_BOX(container),entry,0,0,0);
gtk_box_pack_start(GTK_BOX(container),button,0,0,0);
gtk_container_add(GTK_CONTAINER(window),container);
gtk_widget_show(button);
gtk_widget_show(button);
gtk_widget_show(container);
gtk_widget_show(window);
gtk_widget_show_all(window);
gtk_main();
return 0;
}
