#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "reservation.h"


void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
   // Widgets for input
    GtkWidget *reservation_id = lookup_widget(GTK_WIDGET(button), "ent10");
     GtkWidget *parking_id = lookup_widget(GTK_WIDGET(button), "entry1");
    GtkWidget *license_plate = lookup_widget(GTK_WIDGET(button),  "ent3");
    GtkWidget *Jour_start = lookup_widget(GTK_WIDGET(button),  "spinbutton4");
    GtkWidget *Mois_start = lookup_widget(GTK_WIDGET(button),  "spinbutton5");
    GtkWidget *Annee_start = lookup_widget(GTK_WIDGET(button),  "spinbutton6");
    GtkWidget *Jour_end = lookup_widget(GTK_WIDGET(button),  "spinbutton7");
    GtkWidget *Mois_end = lookup_widget(GTK_WIDGET(button),  "spinbutton8");
    GtkWidget *Annee_end = lookup_widget(GTK_WIDGET(button),  "spinbutton9");
    GtkWidget *NB_H =lookup_widget(GTK_WIDGET(button),  "spinbutton17");
    GtkWidget *label = lookup_widget(GTK_WIDGET(button),  "labeladdres");
    GtkWidget *number_combo = lookup_widget(GTK_WIDGET(button),  "comboboxentry1");
    GtkWidget *check_email = lookup_widget(GTK_WIDGET(button),  "checb9");
    GtkWidget *check_SMS  = lookup_widget(GTK_WIDGET(button), "checb7");
	Reservation R;

   // Get input values from the interface
    strcpy(R.reservation_id, gtk_entry_get_text(GTK_ENTRY(reservation_id)));
    strcpy(R.parking_id, gtk_entry_get_text(GTK_ENTRY(parking_id)));
    strcpy(R.license_plate, gtk_entry_get_text(GTK_ENTRY(license_plate)));

    R.start_date.day = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Jour_start));
    R.start_date.month = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Mois_start));
    R.start_date.year = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Annee_start));

    R.end_date.day = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Jour_end));
    R.end_date.month = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Mois_end));
    R.end_date.year = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Annee_end));
    
    R.number_hours = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(NB_H));
    
   

    // Handle ComboBox for number_place
    strcpy(R.number_place, gtk_combo_box_get_active_text(GTK_COMBO_BOX(number_combo)));

    // Handle CheckButtons for parking_type
    R.message[0] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_email)) ? 1 : 0;
    R.message[1] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_SMS)) ? 1 : 0;
    // Add reservation to file    
    int result = ajouter_reservation("reservation.txt", R);

    // Display message based on result
    if (result) {
        gtk_label_set_text(GTK_LABEL(label), "Reservation added successfully.");
    } else {
        gtk_label_set_text(GTK_LABEL(label), "Error adding reservation.");
    }
}
void
on_but19_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
      GtkWidget *reservation_id = lookup_widget(GTK_WIDGET(button), "ent11");
      GtkWidget *parking_id = lookup_widget(GTK_WIDGET(button), "entry2");
     GtkWidget *license_plate = lookup_widget(GTK_WIDGET(button),  "ent13");
     GtkWidget *Jour_start = lookup_widget(GTK_WIDGET(button),  "spinbutton10");
     GtkWidget *Mois_start = lookup_widget(GTK_WIDGET(button),  "spinbutton11");
    GtkWidget *Annee_start = lookup_widget(GTK_WIDGET(button),  "spinbutton12");
    GtkWidget *Jour_end = lookup_widget(GTK_WIDGET(button),  "spinbutton13");
    GtkWidget *Mois_end = lookup_widget(GTK_WIDGET(button),  "spinbutton14");
    GtkWidget *Annee_end = lookup_widget(GTK_WIDGET(button),  "spinbutton15");
    GtkWidget *NB_H =lookup_widget(GTK_WIDGET(button),  "spinbutton16");
    GtkWidget *label = lookup_widget(GTK_WIDGET(button),  "label55");
    GtkWidget *number_combo = lookup_widget(GTK_WIDGET(button),  "comboboxentry2");
    GtkWidget *check_email = lookup_widget(GTK_WIDGET(button),  "checkb1");
    GtkWidget *check_SMS  = lookup_widget(GTK_WIDGET(button), "checkb2");
	
     // Get thereservation ID to search for
     const char *search_id = gtk_entry_get_text(GTK_ENTRY(reservation_id ));
   // Initialize Reservation structure
    Reservation R = chercher_reservation("reservation.txt",search_id);
  // Search for the reservation details by ID
   if (strcmp(R.reservation_id, "NOT_FOUND") == 0) {
    gtk_label_set_text(GTK_LABEL(label), "Reservation ID not found.");
    return;
}

  // If the reservation is found, populate the fields with the current values
  gtk_entry_set_text(GTK_ENTRY(parking_id), R.parking_id);
  gtk_combo_box_set_active(GTK_COMBO_BOX(number_combo), get_message_index(R.number_place));
  gtk_entry_set_text(GTK_ENTRY(license_plate), R.license_plate);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(Jour_start),R.start_date.day);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(Mois_start),R.start_date.month);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(Annee_start), R.start_date.year);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(Jour_end),R.end_date.day);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(Mois_end),R.end_date.month);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(Annee_end), R.end_date.year);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(NB_H),R.number_hours);
  // Set security options
 
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_email),R.message[0]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_SMS), R.message[1]);
  
    gtk_label_set_text(GTK_LABEL(label), "Reservation found. Modify as needed."); 
    
}
// Helper function to get municipality index (if needed)
int get_message_index(const char *message) {
    // This function can map a municipality address to its index in the combo box.
    // You can implement this function based on your list of municipalities.
    return 0; // Placeholder for now
}
void
on_but14_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
      GtkWidget *reservation_id = lookup_widget(GTK_WIDGET(button), "ent11");
      GtkWidget *parking_id = lookup_widget(GTK_WIDGET(button), "entry2");
     GtkWidget *license_plate = lookup_widget(GTK_WIDGET(button),  "ent13");
     GtkWidget *Jour_start = lookup_widget(GTK_WIDGET(button),  "spinbutton10");
     GtkWidget *Mois_start = lookup_widget(GTK_WIDGET(button),  "spinbutton11");
    GtkWidget *Annee_start = lookup_widget(GTK_WIDGET(button),  "spinbutton12");
    GtkWidget *Jour_end = lookup_widget(GTK_WIDGET(button),  "spinbutton13");
    GtkWidget *Mois_end = lookup_widget(GTK_WIDGET(button),  "spinbutton14");
    GtkWidget *Annee_end = lookup_widget(GTK_WIDGET(button),  "spinbutton15");
    GtkWidget *NB_H =lookup_widget(GTK_WIDGET(button),  "spinbutton16");
    GtkWidget *label = lookup_widget(GTK_WIDGET(button),  "label55");
    GtkWidget *number_combo = lookup_widget(GTK_WIDGET(button),  "comboboxentry2");
    GtkWidget *check_email = lookup_widget(GTK_WIDGET(button),  "checkb1");
    GtkWidget *check_SMS  = lookup_widget(GTK_WIDGET(button), "checkb2"); 

    // Get thereservation ID to search for
    const char *search_id = gtk_entry_get_text(GTK_ENTRY(reservation_id ));
   // Initialize reservation structure
   Reservation R = chercher_reservation("reservation.txt",search_id);
  // Search for the reservation details by ID
   if (strcmp(R.reservation_id, "NOT_FOUND") == 0){
        gtk_label_set_text(GTK_LABEL(label), "Reservation ID not found. Modify failed.");
        return;
    }
    snprintf(R.parking_id, sizeof(R.parking_id), "%s", gtk_entry_get_text(GTK_ENTRY(parking_id)));
    snprintf(R.number_place, sizeof(R.number_place), "%s", gtk_combo_box_get_active_text(GTK_COMBO_BOX(number_combo)));
    snprintf(R.license_plate, sizeof(R.license_plate), "%s", gtk_entry_get_text(GTK_ENTRY(license_plate)));
    
    R.start_date.day= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Jour_start));
    R.start_date.month= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Mois_start));
    R.start_date.year = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Annee_start));

    R.end_date.day = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Jour_end));
    R.end_date.month = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Mois_end));
    R.end_date.year = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Annee_end));
  
    R.number_hours = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(NB_H));
  
    R.message[0] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_email)) ? 1 : 0;
    R.message[1] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_SMS)) ? 1 : 0;
    int result = modifier_reservation("reservation.txt", search_id, R);
    
   if (result) {
        gtk_label_set_text(GTK_LABEL(label), "Reservation modified successfully!");
    } else {
        gtk_label_set_text(GTK_LABEL(label), "Failed to modify Reservation.");
    }

}
void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

    GtkWidget *tree = lookup_widget(GTK_WIDGET(button), "tree1");

    if (!GTK_IS_TREE_VIEW(tree)) {
        g_warning("Erreur : Le widget tree1 n'est pas une TreeView valide.");
        return;
    }
  afficher_toutes_les_reservations(tree);
}
void
on_bres1_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
   GtkWidget *tree = lookup_widget(GTK_WIDGET(button), "tree1");
    GtkWidget *entry = lookup_widget(GTK_WIDGET(button), "entry4");

    const char *search_id = gtk_entry_get_text(GTK_ENTRY(entry));
    afficher_reservation_par_id(tree, search_id);
}




void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
    Parking parkings[100];  // Taille maximale du tableau, ajustez si nécessaire
    int nombre_parkings = 0;

    // Charger et trier les parkings depuis le fichier
    if (lire_parkings_et_trier("parking.txt", parkings, &nombre_parkings) != 0) {
        g_warning("Erreur : Impossible de charger et trier les parkings depuis le fichier.");
        return;
    }

    /*// Vérification des données des parkings
    for (int i = 0; i < nombre_parkings; i++) {
        printf("Parking: %s, Places disponibles: %d\n", parkings[i].parking_name, parkings[i].capacity);
    }*/

    // Récupérer le TreeView à partir de l'interface utilisateur
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "treeview2");
    if (!treeview) {
        g_warning("Erreur : TreeView non trouvé.");
        return;
    }

    // Créer un modèle GtkListStore pour stocker les données
    GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
    
    // Remplir le modèle avec les données triées des parkings
    for (int i = 0; i < nombre_parkings; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 
            0, parkings[i].parking_name, 
            1, parkings[i].capacity, 
            -1);
    }

    // Lier le modèle au TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

    // Libérer le modèle une fois qu'il est lié au TreeView
    g_object_unref(store);

    /*// Supprimer les colonnes existantes avant d'ajouter de nouvelles
    while (gtk_tree_view_get_n_columns(GTK_TREE_VIEW(treeview)) > 0) {
        GtkTreeViewColumn *column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0);
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), column);
    }*/

    // Créer et configurer les colonnes du TreeView
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Colonne 1: Nom du parking
    renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Nom du parking", renderer , "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Colonne 2: Nombre de places disponibles
    renderer = gtk_cell_renderer_text_new();
column = gtk_tree_view_column_new_with_attributes("Places disponibles", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
}




void
on_tree1_row_activated                 (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

    GtkTreeIter iter;
    gchar *id;
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);

    if (gtk_tree_model_get_iter(model, &iter, path)) {
        // Récupérer l'ID depuis la première colonne
        gtk_tree_model_get(GTK_LIST_STORE(model), &iter, 0, &id, -1);

        // Appeler la logique de suppression avec votre fonction existante
        if (supprimer_reservation("reservation.txt", id)) {
            g_print("Réservation avec ID %s supprimée avec succès.\n", id);
        } else {
            g_print("Échec de la suppression de la réservation avec ID %s.\n", id);
        }

        // Mettre à jour l'affichage après suppression
        afficher_toutes_les_reservations(GTK_WIDGET(treeview));
        g_free(id);
    }
}


int confirmation_result = 0; // 0 = No, 1 = Yes
void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

    // Récupérer les widgets des boutons radio
    GtkWidget *yes = lookup_widget(GTK_WIDGET(button), "radiobutton6");
    GtkWidget *no = lookup_widget(GTK_WIDGET(button), "radb5");

    // Vérifier quel bouton radio est activé
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(yes))) { 
        confirmation_result = 1; // Yes sélectionné
    } else {
        confirmation_result = 0; // No sélectionné
    }
}


void
on_b17_clicked                         (GtkButton       *button,
                                        gpointer         user_data)
{

    // Récupérer les widgets nécessaires
    GtkWidget *entry_delete_id = lookup_widget(GTK_WIDGET(button), "entry4");
    GtkWidget *label = lookup_widget(GTK_WIDGET(button), "label58");

    const char *filename = "reservation.txt"; // Nom de ton fichier
    const char *reservation_id = gtk_entry_get_text(GTK_ENTRY(entry_delete_id));

    // Vérifier si la confirmation a été donnée
    if (confirmation_result == 1) {
        // Appel de ta fonction pour supprimer la réservation
        int result = supprimer_reservation(filename, reservation_id);

        if (result) {
            gtk_label_set_text(GTK_LABEL(label), "Réservation supprimée avec succès.");
        } else {
            gtk_label_set_text(GTK_LABEL(label), "Réservation introuvable ou échec de suppression.");
        }
    } else {
        // L'utilisateur a annulé l'action
        gtk_label_set_text(GTK_LABEL(label), "Suppression annulée par l'utilisateur.");
    }
}


// Déclaration de la fenêtre 2 globale
extern GtkWidget *window2;

	void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
    gtk_widget_show_all(window2);
}





