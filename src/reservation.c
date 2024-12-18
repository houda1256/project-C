#include "reservation.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
int lire_parkings_et_trier(const char *filename, Parking *parkings, int *count) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return -1;  // Erreur d'ouverture du fichier
    }

    *count = 0;
    while (fscanf(f, "%s %s %s %d %d %d %d %d %s %s",
                  parkings[*count].parking_id,
                  parkings[*count].parking_name,
                  parkings[*count].municipality_address,
                  &parkings[*count].capacity,
                  &parkings[*count].hourly_tarif,
                  &parkings[*count].parking_type,
                  &parkings[*count].security_option[0],
                  &parkings[*count].security_option[1],
                  parkings[*count].opening_time,
                  parkings[*count].closing_time) != EOF) {
        (*count)++;
    }
    fclose(f);

    // Trier les parkings par capacité (places disponibles)
    for (int i = 0; i < *count - 1; i++) {
        for (int j = i + 1; j < *count; j++) {
            if (parkings[i].capacity < parkings[j].capacity) {
                Parking temp = parkings[i];
                parkings[i] = parkings[j];
                parkings[j] = temp;
            }
        }
    }

    return 0;  // Succès
}

int validate_reservation_id(const char *reservation_id, const char *filename) {
    int len = strlen(reservation_id);

    // Vérifier si l'ID est vide
    if (len == 0) {
        printf("Erreur : L'ID de réservation est vide.\n");
        return 0;
    }

    // Vérifier la longueur (entre 3 et 20 caractères)
    if (len < 3 || len > 20) {
        printf("Erreur : L'ID de réservation doit contenir entre 3 et 20 caractères.\n");
        return 0;
    }

    // Vérifier les caractères valides (lettres, chiffres, tirets uniquement)
    for (int i = 0; i < len; i++) {
        if (!(isalnum(reservation_id[i]) || reservation_id[i] == '-')) {
            printf("Erreur : L'ID de réservation contient des caractères invalides.\n");
            return 0;
        }
    }

    // Vérifier l'unicité de l'ID dans le fichier
    FILE *f = fopen(filename, "r");
    if (f) {
        Reservation res;
        while (fscanf(f, "%s %s %s %s %d %d %d %d %d %d %d %d %d\n",
                      res.reservation_id, res.parking_id, res.number_place, res.license_plate,
                      &res.start_date.day, &res.start_date.month, &res.start_date.year,
                      &res.end_date.day, &res.end_date.month, &res.end_date.year,
                      &res.number_hours, &res.message[0], &res.message[1]) != EOF) {
            if (strcmp(res.reservation_id, reservation_id) == 0) {
                printf("Erreur : L'ID de réservation '%s' existe déjà.\n", reservation_id);
                fclose(f);
                return 0;
            }
        }
        fclose(f);
    }
    return 1;
}

// Ajouter une réservation avec contrôle de saisie
int ajouter_reservation(const char *filename, Reservation res) {
    if (!validate_reservation_id(res.reservation_id, filename)) {
        return 0; // Si l'ID est invalide ou non unique, abandonner
    }

    FILE *f = fopen(filename, "a");
    if (f != NULL) {
        fprintf(f, "%s %s %s %s %d %d %d %d %d %d %d %d %d\n",
                res.reservation_id, res.parking_id, res.number_place, res.license_plate,
                res.start_date.day, res.start_date.month, res.start_date.year,
                res.end_date.day, res.end_date.month, res.end_date.year,
                res.number_hours, res.message[0], res.message[1]);
        fclose(f);
        printf("Réservation ajoutée avec succès.\n");
        return 1;
    } else {
        printf("Erreur : Impossible d'ouvrir le fichier %s pour l'ajout.\n", filename);
        return 0;
    }
}

// Modifier une réservation existante
int modifier_reservation(const char *filename, const char *id, Reservation new_res) {
    int found = 0;
    Reservation res;
    FILE *f = fopen(filename, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (f != NULL && temp != NULL) {
        while (fscanf(f, "%s %s %s %s %d %d %d %d %d %d %d %d %d\n",
                      res.reservation_id, res.parking_id, res.number_place, res.license_plate,
                      &res.start_date.day, &res.start_date.month, &res.start_date.year,
                      &res.end_date.day, &res.end_date.month, &res.end_date.year,
                      &res.number_hours, &res.message[0], &res.message[1]) != EOF) {
            if (strcmp(res.reservation_id, id) == 0) {
                fprintf(temp, "%s %s %s %s %d %d %d %d %d %d %d %d %d\n",
                        new_res.reservation_id, new_res.parking_id, new_res.number_place, new_res.license_plate,
                        new_res.start_date.day, new_res.start_date.month, new_res.start_date.year,
                        new_res.end_date.day, new_res.end_date.month, new_res.end_date.year,
                        new_res.number_hours, new_res.message[0], new_res.message[1]);
                found = 1;
            } else {
                fprintf(temp, "%s %s %s %s %d %d %d %d %d %d %d %d %d\n",
                        res.reservation_id, res.parking_id, res.number_place, res.license_plate,
                        res.start_date.day, res.start_date.month, res.start_date.year,
                        res.end_date.day, res.end_date.month, res.end_date.year,
                        res.number_hours, res.message[0], res.message[1]);
            }
        }
        fclose(f);
        fclose(temp);
        remove(filename);
        rename("temp.txt", filename);
    }
    return found;
}

// Supprimer une réservation par ID
int supprimer_reservation(const char *filename, const char *id) {
    int found = 0;
    Reservation res;
    FILE *f = fopen(filename, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (f != NULL && temp != NULL) {
        while (fscanf(f, "%s %s %s %s %d %d %d %d %d %d %d %d %d\n",
                      res.reservation_id, res.parking_id, res.number_place, res.license_plate,
                      &res.start_date.day, &res.start_date.month, &res.start_date.year,
                      &res.end_date.day, &res.end_date.month, &res.end_date.year,
                      &res.number_hours, &res.message[0], &res.message[1]) != EOF) {
            if (strcmp(res.reservation_id, id) != 0) {
                fprintf(temp, "%s %s %s %s %d %d %d %d %d %d %d %d %d\n",
                        res.reservation_id, res.parking_id, res.number_place, res.license_plate,
                        res.start_date.day, res.start_date.month, res.start_date.year,
                        res.end_date.day, res.end_date.month, res.end_date.year,
                        res.number_hours, res.message[0], res.message[1]);
            } else {
                found = 1;
            }
        }
        fclose(f);
        fclose(temp);
        remove(filename);
        rename("temp.txt", filename);
    }
    return found;
}

// Chercher une réservation par ID
Reservation chercher_reservation(const char *filename, const char *id) {
    Reservation res;
    int found = 0;
    FILE *f = fopen(filename, "r");

    if (f == NULL) {
        perror("Erreur : Impossible d'ouvrir le fichier.");
        strcpy(res.reservation_id, "NOT_FOUND");
        return res;
    }

    while (fscanf(f, "%s %s %s %s %d %d %d %d %d %d %d %d %d\n",
                  res.reservation_id, res.parking_id, res.number_place, res.license_plate,
                  &res.start_date.day, &res.start_date.month, &res.start_date.year,
                  &res.end_date.day, &res.end_date.month, &res.end_date.year,
                  &res.number_hours, &res.message[0], &res.message[1]) != EOF) {
        if (strcmp(res.reservation_id, id) == 0) {
            found = 1;
            break;
        }
    }
    fclose(f);

    if (!found) {
        strcpy(res.reservation_id, "NOT_FOUND");
    }
    return res;
}
// Fonction pour afficher toutes les réservations
void afficher_toutes_les_reservations(GtkWidget *liste) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkListStore *store;
    Reservation r;
    FILE *f;

    // Récupérer le modèle actuel de la TreeView
    store = (GtkListStore *)gtk_tree_view_get_model(GTK_TREE_VIEW(liste));

    if (store == NULL) {
        g_message("Le modèle de la TreeView est NULL, création d'un nouveau GtkListStore.");
        store = gtk_list_store_new(9,
            G_TYPE_STRING,  // Reservation ID
            G_TYPE_STRING,  // Parking ID
            G_TYPE_STRING,  // Number Place
            G_TYPE_STRING,  // License Plate
            G_TYPE_STRING,  // Start Date
            G_TYPE_STRING,  // End Date
            G_TYPE_INT,     // Number of Hours
            G_TYPE_STRING,  // Message [0]
            G_TYPE_STRING   // Message [1]
        );

        gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
        g_object_unref(store);

        // Configuration des colonnes
        renderer = gtk_cell_renderer_text_new();

        column = gtk_tree_view_column_new_with_attributes("Reservation ID", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Parking ID", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Number Place", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("License Plate", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Start Date", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("End Date", renderer, "text", 5, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Number of Hours", renderer, "text", 6, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Message par email", renderer, "text", 7, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Message par SMS", renderer, "text", 8, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
    } else {
        g_message("Le modèle de la TreeView existe déjà, on le vide.");
        gtk_list_store_clear(store);
    }

    // Lecture du fichier des réservations
    f = fopen("reservation.txt", "r");
    if (f == NULL) {
        g_warning("Erreur : Impossible d'ouvrir le fichier reservation.txt.");
        return;
    }

    // Lire les données et remplir le modèle
    while (fscanf(f, "%s %s %s %s %d %d %d %d %d %d %d %d %d\n",
                  r.reservation_id, r.parking_id, r.number_place, r.license_plate,
                  &r.start_date.day, &r.start_date.month, &r.start_date.year,
                  &r.end_date.day, &r.end_date.month, &r.end_date.year,
                  &r.number_hours, &r.message[0], &r.message[1]) != EOF) {
        char start_date[11], end_date[11];
        snprintf(start_date, sizeof(start_date), "%02d/%02d/%04d",
                 r.start_date.day, r.start_date.month, r.start_date.year);
        snprintf(end_date, sizeof(end_date), "%02d/%02d/%04d",
                 r.end_date.day, r.end_date.month, r.end_date.year);

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, r.reservation_id,
            1, r.parking_id,
            2, r.number_place,
            3, r.license_plate,
            4, start_date,
            5, end_date,
            6, r.number_hours,
            7, r.message[0] ? "email" : "No",
            8, r.message[1] ? "SMS" : "No",
            -1);
    }

    fclose(f);
}

void afficher_reservation_par_id(GtkWidget *liste, const char *reservation_id_cherche) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkListStore *store;

    // Récupérer le modèle actuel de la TreeView
    store = (GtkListStore *)gtk_tree_view_get_model(GTK_TREE_VIEW(liste));

    // Si le modèle est NULL, créer un nouveau GtkListStore
    if (store == NULL) {
        g_message("Le modèle de la TreeView est NULL, création d'un nouveau GtkListStore.");
        store = gtk_list_store_new(9,
            G_TYPE_STRING,  // Reservation ID
            G_TYPE_STRING,  // Parking ID
            G_TYPE_STRING,  // Number Place
            G_TYPE_STRING,  // License Plate
            G_TYPE_STRING,  // Start Date
            G_TYPE_STRING,  // End Date
            G_TYPE_INT,     // Number of Hours
            G_TYPE_STRING,  // Message [0]
            G_TYPE_STRING   // Message [1]
        );

        gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
        g_object_unref(store);

        // Ajouter les colonnes si elles n'existent pas
        renderer = gtk_cell_renderer_text_new();

        column = gtk_tree_view_column_new_with_attributes("Reservation ID", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Parking ID", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Number Place", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("License Plate", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Start Date", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("End Date", renderer, "text", 5, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Number of Hours", renderer, "text", 6, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Message par email", renderer, "text", 7, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Message par SMS", renderer, "text", 8, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
    } else {
        g_message("Le modèle de la TreeView existe déjà, on le vide.");
        gtk_list_store_clear(store);
    }

    // Utiliser la fonction existante pour chercher la réservation
    Reservation r = chercher_reservation("reservation.txt", reservation_id_cherche);

    // Vérifier si une réservation a été trouvée
    if (strlen(r.reservation_id) == 0) {
        g_warning("Aucune réservation trouvée avec l'ID : %s", reservation_id_cherche);
        return;
    }

    // Ajouter la réservation trouvée au GtkListStore
    char start_date[11], end_date[11];
    snprintf(start_date, sizeof(start_date), "%02d/%02d/%04d", r.start_date.day, r.start_date.month, r.start_date.year);
    snprintf(end_date, sizeof(end_date), "%02d/%02d/%04d", r.end_date.day, r.end_date.month, r.end_date.year);

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
        0, r.reservation_id,
        1, r.parking_id,
        2, r.number_place,
        3, r.license_plate,
        4, start_date,
        5, end_date,
        6, r.number_hours,
        7, r.message[0] ? "email" : "No",
        8, r.message[1] ? "SMS" : "No",
        -1);

    g_message("Affichage terminé pour reservation_id=%s", reservation_id_cherche);
}

