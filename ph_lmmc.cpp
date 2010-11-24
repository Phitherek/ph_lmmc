// Phitherek_' s Linux Music Management Center v. 0.3 "Mic-To-Output" (c) 2010 by Phitherek_
// Dołącz potrzebne pliki
#include <cstdlib>
#include <gtk/gtk.h>
#include <string>
#include <fstream>

std::string cmd;
GtkWidget* mto_icon; // Dla ikony MTO
GtkTooltips *tip; // Dla tipów

// Callbacki:

// Callback dla MPC
void mpc_command(GtkWidget *widget, gpointer data) {
	cmd = "mpc ";
	cmd += (char *)data;
	system(cmd.c_str());
}

// Callbacki do MPC Fading
void mpd_fade_mute(GtkWidget *widget, gpointer data) {
system("bash/mpd_fade_mute");	
}

void mpd_fade_volmax(GtkWidget *widget, gpointer data) {
system("bash/mpd_fade_volmax");	
}

void mpd_fade_halfvol(GtkWidget *widget, gpointer data) {
system("bash/mpd_fade_halfvol");	
}

void mpd_fade_halfmax(GtkWidget *widget, gpointer data) {
system("bash/mpd_fade_halfmax");	
}

// Callback dla SMPlayera
void smplayer_command(GtkWidget *widget, gpointer data) {
	cmd = "smplayer -send-action ";
	cmd += (char *)data;
	system(cmd.c_str());	
}

// Callbacki do SMPlayer Fading
void smplayer_fade_mute(GtkWidget *widget, gpointer data) {
system("bash/smplayer_fade_mute");	
}

void smplayer_fade_volmax(GtkWidget *widget, gpointer data) {
system("bash/smplayer_fade_volmax");	
}

void smplayer_fade_halfvol(GtkWidget *widget, gpointer data) {
system("bash/smplayer_fade_halfvol");	
}

void smplayer_fade_halfmax(GtkWidget *widget, gpointer data) {
system("bash/smplayer_fade_halfmax");	
}

// Callbacki do Dual Control
void mpd_pause_smplayer_play(GtkWidget *widget, gpointer data) {
system("mpc pause");
system("smplayer -send-action play");
}

void smplayer_pause_mpd_play(GtkWidget *widget, gpointer data) {
system("smplayer -send-action pause");
system("mpc play");
}

void play_all(GtkWidget *widget, gpointer data) {
system("mpc play");
system("smplayer -send-action play");
}

void pause_all(GtkWidget *widget, gpointer data) {
system("mpc pause");
system("smplayer -send-action pause");
}

void stop_all(GtkWidget *widget, gpointer data) {
system("mpc stop");
system("smplayer -send-action stop");
}

// Callbacki do Dual Fading
void mpd_smplayer_fade(GtkWidget *widget, gpointer data) {
system("bash/mpd_smplayer_fade");	
}

void smplayer_mpd_fade(GtkWidget *widget, gpointer data) {
system("bash/smplayer_mpd_fade");	
}

// Callback uruchamiający gncmpcpp
void gncmpcpp_launch(GtkWidget *widget, gpointer data) {
system("gncmpcpp&");	
}

// Callback uruchamiający SMPlayera
void smplayer_launch(GtkWidget *widget, gpointer data) {
system("smplayer&");
}

// Callback do projectM
void projectm_launch(GtkWidget *widget, gpointer data) {
system("projectM-pulseaudio&");	
}

// Funkcja do sprawdzania statusu ph_mto
int mtostatus() {
std::string houm;
std::string mtostatusfile;
system("echo $HOME > /tmp/houm");
std::ifstream home("/tmp/houm");
home >> houm;
home.close();
remove("/tmp/houm");
mtostatusfile = houm + "/.ph_mto/status";
std::ifstream mtostat(mtostatusfile.c_str());
if(mtostat) {
int st;
mtostat >> st;
mtostat.close();
return st;
} else {
return -1;
}
}

// Callback do Mic-To-Output
void ph_mto_launch(GtkWidget *widget, gpointer data) {
system("ph_mto");
int mtostat;
mtostat = mtostatus();
if(mtostat == -1) {
gtk_image_set_from_file(GTK_IMAGE(mto_icon), "mto_unknown.xpm");
} else if(mtostat == 0) {
gtk_image_set_from_file(GTK_IMAGE(mto_icon), "mto_on.xpm");	
} else if(mtostat == 1) {
gtk_image_set_from_file(GTK_IMAGE(mto_icon), "mto_off.xpm");	
}
while( gtk_events_pending() )
    gtk_main_iteration();
if(mtostat == -1) {
gtk_tooltips_set_tip(tip, widget, "Unknown status of Mic-To-Output: Click to launch ph_mto", NULL);
} else if(mtostat == 0) {
gtk_tooltips_set_tip(tip, widget, "Enable Mic-To-Output with ph_mto", NULL);	
} else if(mtostat == 1) {
gtk_tooltips_set_tip(tip, widget, "Disable Mic-To-Output with ph_mto", NULL);	
}
while( gtk_events_pending() )
    gtk_main_iteration();
}

// Callback do Stop, Close & Exit
void stop_close(GtkWidget *widget, gpointer data) {
system("mpc stop");
system("smplayer -send-action stop");
system("killall ncmpcpp");
system("killall smplayer");
system("killall projectM-pulseaudio");
int mtostat;
mtostat = mtostatus();
if(mtostat == 1) {
system("ph_mto");	
}
}


// Główna funkcja - zgodnie z GTK
int main(int argc, char *argv[]) {
// Tworzymy potrzebne widgety
GtkWidget *window; // Dla okna
GtkWidget *vbox; // Dla głównego vboxu
GtkWidget *hbox; // Dla hboxów
GtkWidget *button; // Dla przycisków
GtkWidget *logo; // Dla loga
GtkWidget *image; // Dla obrazków na przyciskach
GtkWidget *frame; // Dla ramek

// Zmienna do statusu MTO
int mtostat;

// Inicjuj GTK
gtk_init(&argc, &argv);

// Stwórz nowe okno
window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
// Ustaw tytuł okna
gtk_window_set_title(GTK_WINDOW(window), "Phitherek_' s Linux Music Management Center v. 0.3 'Mic-To-Output' (c) 2010 by Phitherek_");
// Wyjdź z GTK, gdy okno zostanie zamknięte
g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
// Ustaw odległość między ramką okna, a obiektem
gtk_container_set_border_width(GTK_CONTAINER(window), 5);
// Zainicjuj podpowiedzi
tip = gtk_tooltips_new();
// Utwórz VBox-a z odstępem 5
vbox = gtk_vbox_new(FALSE, 5);
// Ramka na logo
frame = gtk_frame_new(NULL);
// Stwórz obraz z logo programu
logo = gtk_image_new_from_file("ph_lmmc_logo.xpm");
// Pokaż logo
gtk_widget_show(logo);
// Pakuj logo do frame-a
gtk_container_add(GTK_CONTAINER(frame), logo);
// Frame do vboxa
gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, FALSE, 5);
// Pokaż frame
gtk_widget_show(frame);
// Ramka MPD Controls
frame = gtk_frame_new("MPD Controls");
// Utwórz HBox-a z odstępem 5
hbox = gtk_hbox_new(FALSE, 5);
// Przycisk Previous
button = gtk_button_new();
// Ikona Previous
image = gtk_image_new_from_file("gtk-media-previous-ltr.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Previous track in MPD", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(mpc_command), (gpointer) "prev");
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk Play
button = gtk_button_new();
// Ikona Play
image = gtk_image_new_from_file("gtk-media-play-ltr.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Play from MPD", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(mpc_command), (gpointer) "play");
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk Pause
button = gtk_button_new();
// Ikona Pause
image = gtk_image_new_from_file("gtk-media-pause.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Pause in MPD", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(mpc_command), (gpointer) "pause");
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk Stop
button = gtk_button_new();
// Ikona Stop
image = gtk_image_new_from_file("gtk-media-stop.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Stop in MPD", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(mpc_command), (gpointer) "stop");
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk Next
button = gtk_button_new();
// Ikona Next
image = gtk_image_new_from_file("gtk-media-next-ltr.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Next track in MPD", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(mpc_command), (gpointer) "next");
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk Launch NCMPC++
button = gtk_button_new();
// Ikona Launch NCMPC++
image = gtk_image_new_from_file("ncmpcpp_launch.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Launch NCMPC++ in gnome-terminal", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gncmpcpp_launch), NULL);
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Pokaż hboxa
gtk_widget_show(hbox);
// Przyczep hboxa do frame-a
gtk_container_add(GTK_CONTAINER(frame), hbox);
// Pakuj frame-a do vboxa
gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, FALSE, 5);
// Pokaż frame-a
gtk_widget_show(frame);
// Ramka MPD Fading
frame = gtk_frame_new("MPD Fading");
// Utwórz HBox-a z odstępem 5
hbox = gtk_hbox_new(FALSE, 5);
// Przycisk fade_mute
button = gtk_button_new();
// Ikona fade_mute
image = gtk_image_new_from_file("fade_mute.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Fade to mute in MPD", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(mpd_fade_mute), NULL);
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk fade_volmax
button = gtk_button_new();
// Ikona fade_volmax
image = gtk_image_new_from_file("fade_volmax.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Fade to max volume in MPD", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(mpd_fade_volmax), NULL);
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk fade_halfvol
button = gtk_button_new();
// Ikona fade_halfvol
image = gtk_image_new_from_file("fade_halfvol.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Fade to approximately 50% of volume in MPD", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(mpd_fade_halfvol), NULL);
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk fade_halfmax
button = gtk_button_new();
// Ikona fade_halfmax
image = gtk_image_new_from_file("fade_halfmax.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Fade from approximately 50% of volume to max volume in MPD", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(mpd_fade_halfmax), NULL);
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Pokaż hboxa
gtk_widget_show(hbox);
// Przyczep hboxa do frame-a
gtk_container_add(GTK_CONTAINER(frame), hbox);
// Pakuj frame-a do vboxa
gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, FALSE, 5);
// Pokaż frame-a
gtk_widget_show(frame);
// Ramka SMPlayer Controls
frame = gtk_frame_new("SMPlayer Controls");
// HBox 5
hbox = gtk_hbox_new(FALSE, 5);
// Przycisk Previous
button = gtk_button_new();
// Ikona Previous
image = gtk_image_new_from_file("gtk-media-previous-ltr.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Previous track in SMPlayer", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_command), (gpointer) "pl_prev");
// Pakuj w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk Play
button = gtk_button_new();
// Ikona Play
image = gtk_image_new_from_file("gtk-media-play-ltr.xpm");
// Wyświetl ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Play from SMPlayer", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_command), (gpointer) "play");
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Przycisk Pause
button = gtk_button_new();
// Ikona Pause
image = gtk_image_new_from_file("gtk-media-pause.xpm");
// Wyświetl ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Pause in SMPlayer", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_command), (gpointer) "pause");
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Przycisk Stop
button = gtk_button_new();
// Ikona Stop
image = gtk_image_new_from_file("gtk-media-stop.xpm");
// Wyświetl ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Stop in SMPlayer", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_command), (gpointer) "stop");
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Przycisk Next
button = gtk_button_new();
// Ikona Next
image = gtk_image_new_from_file("gtk-media-next-ltr.xpm");
// Wyświetl ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Next track in SMPlayer", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_command), (gpointer) "pl_next");
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Przycisk Launch SMPlayer
button = gtk_button_new();
// Ikona Launch SMPlayer
image = gtk_image_new_from_file("smplayer_launch.xpm");
// Wyświetl ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Launch SMPlayer", NULL);
// Wywołaj callbacka
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_launch), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Pokaż hboxa
gtk_widget_show(hbox);
// Do frame-a
gtk_container_add(GTK_CONTAINER(frame), hbox);
// Frame do vboxa
gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, FALSE, 5);
// Pokaż frame
gtk_widget_show(frame);
// Ramka SMPlayer Fading
frame = gtk_frame_new("SMPlayer Fading");
// Utwórz HBox-a z odstępem 5
hbox = gtk_hbox_new(FALSE, 5);
// Przycisk fade_mute
button = gtk_button_new();
// Ikona fade_mute
image = gtk_image_new_from_file("fade_mute.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Fade to mute in SMPlayer", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_fade_mute), NULL);
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk fade_volmax
button = gtk_button_new();
// Ikona fade_volmax
image = gtk_image_new_from_file("fade_volmax.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Fade to max volume in SMPlayer", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_fade_volmax), NULL);
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk fade_halfvol
button = gtk_button_new();
// Ikona fade_halfvol
image = gtk_image_new_from_file("fade_halfvol.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Fade to approximately 50% of volume in SMPlayer", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_fade_halfvol), NULL);
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Przycisk fade_halfmax
button = gtk_button_new();
// Ikona fade_halfmax
image = gtk_image_new_from_file("fade_halfmax.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Przyczep ikonę do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Dodaj podpowiedź do przycisku
gtk_tooltips_set_tip(tip, button, "Fade from approximately 50% of volume to max volume in SMPlayer", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_fade_halfmax), NULL);
// Pakuj przycisk w hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Pokaż przycisk
gtk_widget_show(button);
// Pokaż hboxa
gtk_widget_show(hbox);
// Przyczep hboxa do frame-a
gtk_container_add(GTK_CONTAINER(frame), hbox);
// Pakuj frame-a do vboxa
gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, FALSE, 5);
// Pokaż frame-a
gtk_widget_show(frame);
// Ramka Dual Control
frame = gtk_frame_new("Dual Control");
// HBox 5
hbox = gtk_hbox_new(FALSE, 5);
// Przycisk MPD: pause SMPlayer: play
button = gtk_button_new();
// Ikona
image = gtk_image_new_from_file("mpd_pause_smplayer_play.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Pause in MPD, play from SMPlayer", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(mpd_pause_smplayer_play), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Przycisk SMPlayer: pause MPD: play
button = gtk_button_new();
// Ikona
image = gtk_image_new_from_file("smplayer_pause_mpd_play.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Pause in SMPlayer, play from MPD", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_pause_mpd_play), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Przycisk Play All
button = gtk_button_new();
// Ikona
image = gtk_image_new_from_file("gtk-media-play-ltr_all.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Play everything", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(play_all), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Przycisk Pause All
button = gtk_button_new();
// Ikona
image = gtk_image_new_from_file("gtk-media-pause_all.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Pause everything", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(pause_all), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Przycisk Stop All
button = gtk_button_new();
// Ikona
image = gtk_image_new_from_file("gtk-media-stop_all.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Stop everything", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(stop_all), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Wyświetl hboxa
gtk_widget_show(hbox);
// Do frame-a
gtk_container_add(GTK_CONTAINER(frame), hbox);
// Frame do vboxa
gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, FALSE, 5);
// Pokaż frame-a
gtk_widget_show(frame);
// Ramka Dual Fading
frame = gtk_frame_new("Dual Fading");
// HBox 5
hbox = gtk_hbox_new(FALSE, 5);
// Przycisk MPD -> SMPlayer Fade
button = gtk_button_new();
// Ikona
image = gtk_image_new_from_file("mpd_smplayer_fade.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Pause in MPD, play from SMPlayer - crossfade", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(mpd_smplayer_fade), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Przycisk SMPlayer -> MPD Fade
button = gtk_button_new();
// Ikona
image = gtk_image_new_from_file("smplayer_mpd_fade.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Pause in SMPlayer, play from MPD - crossfade", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(smplayer_mpd_fade), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);

// Wyświetl hboxa
gtk_widget_show(hbox);
// Do frame-a
gtk_container_add(GTK_CONTAINER(frame), hbox);
// Frame do vboxa
gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, FALSE, 5);
// Pokaż frame-a
gtk_widget_show(frame);
// Ramka Visualisation
frame = gtk_frame_new("Visualisation");
// HBox 5
hbox = gtk_hbox_new(FALSE, 5);
// Przycisk Launch projectM-pulseaudio
button = gtk_button_new();
// Ikona
image = gtk_image_new_from_file("projectm_launch.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Launch projectM for PulseAudio", NULL);
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(projectm_launch), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Wyświetl hboxa
gtk_widget_show(hbox);
// Do frame-a
gtk_container_add(GTK_CONTAINER(frame), hbox);
// Frame do vboxa
gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, FALSE, 5);
// Pokaż frame-a
gtk_widget_show(frame);
// Ramka Mic-To-Output (PulseAudio only)
frame = gtk_frame_new("Mic-To-Output (PulseAudio only)");
// HBox 5
hbox = gtk_hbox_new(FALSE, 5);
// Przycisk Mic-To-Output On/Off/Unknown status
button = gtk_button_new();
// Sprawdzenie statusu
mtostat = mtostatus();
// Ikona:
if(mtostat == -1) {
mto_icon = gtk_image_new_from_file("mto_unknown.xpm");
} else if(mtostat == 0) {
mto_icon = gtk_image_new_from_file("mto_on.xpm");	
} else if(mtostat == 1) {
mto_icon = gtk_image_new_from_file("mto_off.xpm");	
}
// Pokaż ikonę
gtk_widget_show(mto_icon);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), mto_icon);
// Podpowiedź:
if(mtostat == -1) {
gtk_tooltips_set_tip(tip, button, "Unknown status of Mic-To-Output: Click to launch ph_mto", NULL);
} else if(mtostat == 0) {
gtk_tooltips_set_tip(tip, button, "Enable Mic-To-Output with ph_mto", NULL);	
} else if(mtostat == 1) {
gtk_tooltips_set_tip(tip, button, "Disable Mic-To-Output with ph_mto", NULL);	
}
// Wywołaj callback
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(ph_mto_launch), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Wyświetl hboxa
gtk_widget_show(hbox);
// Do frame-a
gtk_container_add(GTK_CONTAINER(frame), hbox);
// Frame do vboxa
gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, FALSE, 5);
// Pokaż frame-a
gtk_widget_show(frame);
// Ramka Close
frame = gtk_frame_new("LMMC");
// HBox 5
hbox = gtk_hbox_new(FALSE, 5);
// Przycisk Exit
button = gtk_button_new();
// Ikona
image = gtk_image_new_from_file("gtk-quit.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Exit from LMMC", NULL);
// Wywołaj callback zamykający
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Przycisk Stop, Close and Exit
button = gtk_button_new();
// Ikona
image = gtk_image_new_from_file("gtk-close.xpm");
// Pokaż ikonę
gtk_widget_show(image);
// Do przycisku
gtk_container_add(GTK_CONTAINER(button), image);
// Podpowiedź
gtk_tooltips_set_tip(tip, button, "Stop everything, close (kill) everything and exit from LMMC. WARNING! This will kill all instances of NCMPC++, SMPlayer and projectM-pulseaudio!", NULL);
// Wywołaj callbacki
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(stop_close), NULL);
g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
// Do hboxa
gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 5);
// Wyświetl przycisk
gtk_widget_show(button);
// Wyświetl hboxa
gtk_widget_show(hbox);
// Do frame-a
gtk_container_add(GTK_CONTAINER(frame), hbox);
// Frame do vboxa
gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, FALSE, 5);
// Pokaż frame-a
gtk_widget_show(frame);
// Pokaż vboxa
gtk_widget_show(vbox);
// Przyczep vboxa do okna
gtk_container_add(GTK_CONTAINER(window), vbox);
// Pokaż okno
gtk_widget_show(window);
// Uruchom główną pętlę GTK
gtk_main();
// Wszystko OK, przyjmij wartość sukcesu
return EXIT_SUCCESS;
}
