#ifndef PROJET_H_INCLUDED
#define PROJET_H_INCLUDED
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
typedef struct {
    char parking_id[50];
    char parking_name[50];
    char municipality_address[100];
    int capacity;
    int hourly_tarif;
    int parking_type; 
    int security_option[2]; 
    char opening_time[10];
    char closing_time[10];
} Parking;

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char reservation_id[100];
    char parking_id[100];
    char number_place[100];
    char license_plate[100];
    Date start_date;
    Date end_date;
    int number_hours;
    int message[2];
} Reservation;
void afficher_toutes_les_reservations(GtkWidget *liste);
void afficher_reservation_par_id(GtkWidget *liste, const char *reservation_id_cherche);
int validate_reservation_id(const char *reservation_id, const char *filename);
int ajouter_reservation(const char *filename, Reservation res);
int modifier_reservation(const char *filename, const char *id, Reservation new_res);
int supprimer_reservation(const char *filename, const char *id);
Reservation chercher_reservation(const char *filename, const char *id);
int lire_parkings_et_trier(const char *filename, Parking *parkings, int *count);
#endif



