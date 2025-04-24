#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//FONCTION DIVERS###############################

float FlottantPourAtt(){ //flottant entre 0.8 et 1.2
    float n = (float)rand() / (float)RAND_MAX;
    return 0.8 + n * (1.2 - 0.8);
}
int ChoixDeClasse() { //entier entre 1 et 4
    return 1 + rand() % 4;
}
int ChoixAttaque() { //entier entre 1 et 2
    return 1 + rand() % 2;
}

//////////////////////////////////////////////////////
//DEBUT PARTIE CLASSE ################################################################
typedef struct{
    char *nom;
    int att;
    int def;
    int HPmax;
    int rest;
}Classe;

Classe* CreerClasse(const char* nom,int att,int def,int HPmax,int rest){
    Classe* C;
    C = (Classe*) malloc(sizeof(Classe));
    
    if(!C){
        return NULL;    
    }
    C->nom = (char*)malloc(sizeof(char)*(strlen(nom)+1));
    if(!C->nom){
        free(C);
        return NULL;
    }
    strcpy(C->nom,nom);
    C->att = att;
    C->def = def;
    C->HPmax = HPmax;
    C->rest = rest;
    return C;
}
void AfficheClasse(Classe* C) {
    if (C) {
        printf("| %-20s | %8d | %8d | %8d | %8d |\n",
               C->nom, C->att, C->def, C->HPmax, C->rest);
    } else {
        printf("| %-20s | %8s | %8s | %8s | %8s |\n",
               "NULL", "-", "-", "-", "-");
    }
}

typedef struct noeudClasse{
    Classe C;
    struct noeudClasse* suivant;
}noeudClasse;
typedef noeudClasse* ListeClasse;

ListeClasse AlloueCelluleClasse(Classe C){
    ListeClasse tmp = (ListeClasse) malloc(sizeof(noeudClasse));
    if(!tmp){
        return NULL;
    }
    tmp->C = C;
    tmp->suivant = NULL;
    return tmp;

}

ListeClasse AjouteClasseListe(ListeClasse lst,Classe C){
    ListeClasse tmp = lst;
    if(!tmp){
        lst = AlloueCelluleClasse(C);
        return lst;
    }
    for(;tmp->suivant;tmp = tmp->suivant){
        ;
    }
    tmp->suivant = AlloueCelluleClasse(C);
    return tmp;
}
void AfficheClasseListe(ListeClasse lst) {
    printf("+----------------------+----------+----------+----------+----------+\n");
    printf("| Nom                 | att      | def      | HPmax    | rest     |\n");
    printf("+----------------------+----------+----------+----------+----------+\n");

    ListeClasse tmp = lst;
    for(;tmp;tmp = tmp->suivant){
        Classe C = tmp->C;
        AfficheClasse(&C);
    }

    printf("+----------------------+----------+----------+----------+----------+\n");
}
//FIN PARTIE CLASSE#############################################################################

//DEBUT PARTIE PERSONNAGE
typedef struct{
    char *nom;
    int attbonus;
    int defbonus;
    int HPbonus;
    int restbonus;
    int strred;
    int prix;
}Accessorie;

typedef struct{
    char *nom;
    char *nomClasse;
    int att;
    int def;
    int HPmax;
    int rest;
    int HP; //HP actual different que HPmax qui est contenue dans classe
    int str;
    int nbcomb;
    int isDefending;//entier ou bien 1(defend) ou bien 0(ne defend pas)
    Accessorie* A1;
    Accessorie* A2;
}Personnage;


Personnage* CreerPersonnage(const char* nom,Classe* classe,int str,int nbcomb){
    Personnage* P = (Personnage*)malloc(sizeof(Personnage));
    if (!P){
        return NULL;
    }
    P->nom = (char*)malloc(sizeof(char)*(strlen(nom)+1));
    if(!P->nom){
        free(P);
        return NULL;
    }
    strcpy(P->nom,nom);
    P->nomClasse = (char*)malloc( sizeof(char)* (strlen(classe->nom) +1));
    if(!P->nomClasse){
        free(P);
        return NULL;
    }
    strcpy(P->nomClasse,classe->nom);
    P->att = classe->att;
    P->def = classe->def;
    P->HPmax = classe->HPmax;
    P->rest = classe->rest;
    P->HP = classe->HPmax;
    P->str = str;
    P->nbcomb = nbcomb;
    P->isDefending = 0;
    P->A1 = (Accessorie*)malloc(sizeof(Accessorie));
    P->A2 = (Accessorie*)malloc(sizeof(Accessorie));
    if(!(P->A1) || !(P->A2)){
        return NULL;
    }
    return P;
}
int AjouteAccessoriePersonnage(Personnage* P,Accessorie* A){
    if (!P->A1){
        P->A1 = A;
        P->HP += P->A1->HPbonus;
        P->HPmax += P->A1->HPbonus;
        P->att += P->A1->attbonus;
        P->def += P->A1->defbonus;
        P->rest += P->A1->restbonus;
        return 1;
    }
    else{
        P->A2 = A;
        P->HP += P->A2->HPbonus;
        P->HPmax += P->A2->HPbonus;
        P->att += P->A2->attbonus;
        P->def += P->A2->defbonus;
        P->rest += P->A2->restbonus;
        return 1;
    }
    return 0;
}

int EnleveAccessoriePersonnage(Personnage* P,Accessorie* A){
    if (P->A1 && strcmp(P->A1->nom,A->nom) == 0){
        int HPfinal = (P->HP) - (P->A1->HPbonus);
        if(HPfinal <= 0){
            P->HP = 1;
        }
        else{
            P->HP = HPfinal;
        }
        P->HPmax -= P->A1->HPbonus;
        P->att -= P->A1->attbonus;
        P->def -= P->A1->defbonus;
        P->rest -= P->A1->restbonus;
        free(P->A1);
        P->A1 = NULL;
        return 1;
    }
    if (P->A2 && strcmp(P->A2->nom,A->nom)==0){
        int HPfinal = (P->HP) - (P->A2->HPbonus);
        if(HPfinal <= 0){
            P->HP = 1;
        }
        else{
            P->HP = HPfinal;
        }
        P->HPmax -= P->A2->HPbonus;
        P->att -= P->A2->attbonus;
        P->def -= P->A2->defbonus;
        P->rest -= P->A2->restbonus;
        free(P->A2);
        P->A2 = NULL;
        return 1;
    }
    return 0;}


void AffichePersonnage(Personnage* P) {
    printf("| %-10s | %-18s | %3d | %3d | %2d/%-4d | %4d | %3d | %6d |\n",P->nom,P->nomClasse,P->att,P->def,
           P->HP,P->HPmax,P->rest,P->str,P->nbcomb);
}


typedef struct noeudPersonnage{
    Personnage* P;
    struct noeudPersonnage* suivant;
}noeudPersonnage;
typedef noeudPersonnage* ListePersonnage;


ListePersonnage AlloueCellulePersonnage(Personnage *P){
    ListePersonnage tmp = (ListePersonnage)malloc(sizeof(noeudPersonnage));
    if(!tmp){
        return NULL;
    }
    tmp->P = P;
    tmp->suivant = NULL;
    return tmp;
}

void AjoutePersonnageListe(ListePersonnage* lst,Personnage* P){
    ListePersonnage tmp = *lst;
    if(!tmp){return;}
    for(;tmp->suivant;tmp = tmp->suivant){
        ;
    }
    tmp->suivant = AlloueCellulePersonnage(P);
}

void AffichePersonnageListe(ListePersonnage lst){
    if(!lst){
        printf("Plus de personnage diponible!");
    }
    else{
        printf("+------------+--------------------+-----+-----+---------+------+-----+--------+\n");
        printf("| Nom        | Classe             | att | def | HP/HPmax| rest | str | nbcomb |\n");
        printf("+------------+--------------------+-----+-----+---------+------+-----+--------+\n");
        ListePersonnage tmp = lst;
        for(;tmp;tmp = tmp->suivant){
            Personnage *P = tmp->P;
            AffichePersonnage(P);
        }}
        printf("+------------+--------------------+-----+-----+---------+------+-----+--------+\n");
    }
int RecherchePersonnage(ListePersonnage lst,char* nom){
    if (!lst || !nom){
        return 0;
    }
    while(lst){
        if(strcmp(lst->P->nom,nom)==0){
            return 1;
        }
        lst = lst->suivant;
    }
    return 0; //False
}



int CalculeLenDeListePerso(ListePersonnage lst){
    int len = 0;
    ListePersonnage tmp = lst;
    if(!tmp){
        return len;
    }
    for(;tmp;tmp=tmp->suivant){
        len++;
    }
    return len;
}

void LibererListePersonnage(ListePersonnage lst){
    while(lst){
        ListePersonnage tmp = lst;
        lst = lst->suivant;
        free(tmp);
    }
}
void LibererPersonnage(Personnage* P) {
    if (P) {
        free(P->nom);
        free(P->nomClasse);
        free(P->A1);
        free(P->A2);
        free(P);
    }
}
void EnlevePersonnageDeListe(ListePersonnage* lst, Personnage* P) {
    if (!(*lst)) return; 
    if (strcmp(((*lst)->P)->nom, P->nom) == 0) { 
        ListePersonnage temp = *lst;
        *lst = (*lst)->suivant;
        LibererPersonnage(temp->P); 
        free(temp); 
        return;
    }
    ListePersonnage tmp = *lst;
    while (tmp->suivant && strcmp(tmp->suivant->P->nom, P->nom) != 0) {
        tmp = tmp->suivant;
    }
    if (tmp->suivant) { 
        ListePersonnage temp = tmp->suivant;
        tmp->suivant = tmp->suivant->suivant;
        LibererPersonnage(temp->P); 
        free(temp); 
    }
}

Personnage* ExtraitPersonnageListeByNom(ListePersonnage *lst,char* nom){
    if(!(*lst)){
        return NULL;
    }
    if (strcmp(((*lst)->P)->nom,nom)==0) { // Suppression de la tête
        ListePersonnage temp = *lst; // Sauvegarde de la tête
        Personnage* P_trouver = temp->P; // Personnage à extraire
        *lst = temp->suivant; // Mise à jour de la tête
        free(temp); // Libération de la cellule
        return P_trouver;
    }
    ListePersonnage tmp = *lst;
    while (tmp->suivant && strcmp(tmp->suivant->P->nom,nom)!= 0) {
        tmp = tmp->suivant;
    }
    if (tmp->suivant) { // Trouvé
        ListePersonnage a_supprimer = tmp->suivant; // Cellule à supprimer
        Personnage* P_trouver = a_supprimer->P; // Sauvegarde du personnage
        tmp->suivant = a_supprimer->suivant; // Mise à jour du lien
        free(a_supprimer); // Libération de la cellule
        return P_trouver;
    }; 
    return NULL;

}

ListePersonnage ExtraitNpersonnageDeLaListe(ListePersonnage* lst,int n){
    if (lst == NULL || *lst == NULL || n <= 0) {
        return NULL; // Liste vide ou nombre invalide
    }
    ListePersonnage tmp = NULL;  // Nouvelle liste pour les personnages extraits
    ListePersonnage courant = NULL;  // Pointeur pour parcourir la nouvelle liste
    ListePersonnage CaseEnleve = NULL;
    for (int i = 0; i < n; i++) {
        if (*lst == NULL) {
            break; // Si la liste originale est épuisée avant d'atteindre 'n'
        }
        
        // Allouer un nouveau nœud pour le personnage extrait
        CaseEnleve = *lst;
        ListePersonnage nouvelleCase = AlloueCellulePersonnage(CaseEnleve->P);
        // Enlever le personnage de la liste originale
        *lst = (*lst)->suivant;
        free(CaseEnleve);
        // Ajouter le nœud à la nouvelle liste
        if (tmp == NULL) {
            tmp = nouvelleCase; // Premier nœud
        } else {
            courant->suivant = nouvelleCase;
        }
        courant = nouvelleCase;
    }

    return tmp;
}
void DefensePerso(Personnage* P){ //fonction qui change la defense du personnage 
    P->isDefending = 1;
}

void ResetDefense(ListePersonnage* lst){
    ListePersonnage tmp = *lst;
    if(!tmp){
        return;
    }
    while(tmp){
        if(tmp->P->isDefending == 1){
        tmp->P->isDefending = 0;
        }
        tmp = tmp->suivant;
    }
}

//FIN PARTIE PERSONNAGE #####################################

//DEBUT PARTIE ACCESSORIE#######################################################


Accessorie* CreerAccesorie(char* nom,int attbonus,int defbonus,int HPbonus,int restbonus,int strred,int prix){
    Accessorie* A = (Accessorie*)malloc(sizeof(Accessorie));
    if (!A){
        return NULL;
    }
    A->nom = (char*)malloc(sizeof(char)*(strlen(nom)+1));
    if(!nom){
        free(A);
        return NULL;
    }
    strcpy(A->nom,nom);
    A->attbonus = attbonus;
    A->defbonus = defbonus;
    A->HPbonus = HPbonus;
    A->restbonus = restbonus;
    A->strred = strred;
    A->prix = prix;
    return A;
}
void AfficheAccessorie(Accessorie* A) {
    if (A) {
        printf("| %-20s | %8d | %8d | %8d | %8d | %8d |\n",
               A->nom, A->attbonus, A->defbonus, A->HPbonus, A->restbonus, A->strred);
    } else {
        printf("| %-20s | %8s | %8s | %8s | %8s | %8s |\n",
               "NULL", "-", "-", "-", "-", "-");
    }
}

typedef struct noeudAccessorie{
    Accessorie* A;
    struct noeudAccessorie* suivant;
}noeudAccessorie;
typedef noeudAccessorie* ListeAccessorie;

ListeAccessorie AlloueCelluleAccessorie(Accessorie* A){
    ListeAccessorie lst = (ListeAccessorie)malloc(sizeof(noeudAccessorie));
    if (!lst){
        return NULL;
    }
    lst->A = A;
    lst->suivant = NULL;
    return lst;
}

void AjouteAccessorieListe(ListeAccessorie* lst,Accessorie* A){
    ListeAccessorie Ajoute = AlloueCelluleAccessorie(A);
    if(!Ajoute){
        return;
    }
    if (!(*lst)){
        *lst = Ajoute;
        return;
    }
    ListeAccessorie tmp = *lst;
    while(tmp->suivant){
        tmp = tmp->suivant;
    }
    tmp->suivant = Ajoute;}

void EnleveAccessorieLsite(ListeAccessorie* lst,Accessorie* A){
    if (!(*lst)){
        return;
    }
    if (strcmp(((*lst)->A)->nom,A->nom)==0) {
        ListeAccessorie tmp = *lst;
        *lst = (*lst)->suivant;
        free(tmp);
        return;
    }
    ListeAccessorie tmp = *lst;
    while (tmp->suivant && strcmp(tmp->suivant->A->nom,A->nom)!= 0) {
        tmp = tmp->suivant;
    }
    if (tmp->suivant) {
        ListeAccessorie temp = tmp->suivant;
        tmp->suivant = tmp->suivant->suivant;
        free(temp);
    }
}

void AfficheListeAccessorie(ListeAccessorie lst) {
    printf("+----------------------+----------+----------+----------+----------+----------+\n");
    printf("| Nom                  | attbonus | defbonus | HPbonus  | restbonus| strred   |\n");
    printf("+----------------------+----------+----------+----------+----------+----------+\n");

    ListeAccessorie tmp = lst;
    while (tmp) {
        AfficheAccessorie(tmp->A);
        tmp = tmp->suivant;
    }

    printf("+----------------------+----------+----------+----------+----------+----------+\n");
}


int RechercheAccessorieByNom(ListeAccessorie Accessorie_Dispo,char* nom){
    if(!Accessorie_Dispo || !nom){
        return 0;
    }
    while(Accessorie_Dispo){
        if(strcmp(Accessorie_Dispo->A->nom,nom) == 0){
            return 1; 
        }
        Accessorie_Dispo = Accessorie_Dispo->suivant;
    }
    return 0;

}

Accessorie* ExtraitAccesorieListeByNom(ListeAccessorie *lst,char* nom){
    if(!(*lst)){
        return NULL;
    }
    if (strcmp(((*lst)->A)->nom,nom)==0) { // Suppression de la tête
        ListeAccessorie temp = *lst; // Sauvegarde de la tête
        Accessorie* A_trouver = temp->A; // Personnage à extraire
        *lst = temp->suivant; // Mise à jour de la tête
        free(temp); // Libération de la cellule
        return A_trouver;
    }
    ListeAccessorie tmp = *lst;
    while (tmp->suivant && strcmp(tmp->suivant->A->nom,nom)!= 0) {
        tmp = tmp->suivant;
    }
    if (tmp->suivant) { // Trouvé
        ListeAccessorie a_supprimer = tmp->suivant; // Cellule à supprimer
        Accessorie* A_trouver = a_supprimer->A; // Sauvegarde du personnage
        tmp->suivant = a_supprimer->suivant; // Mise à jour du lien
        free(a_supprimer); // Libération de la cellule
        return A_trouver;
    }; 
    return NULL;

}

//FIN PARTIE ACCESSORIE############################################################

//DEBUT PARTIE ENNEMIE #######################################

typedef struct{
    char *nom;
    int niveau;
    int atten;
    int defen;
    int HPenn;
    int attstrenn;
}Ennemi;

Ennemi* CreerEnemi(const char* nom,int niveau,int atten,int defen,int HPenn,int attstrenn){
    Ennemi* E = (Ennemi*)malloc(sizeof(Ennemi));
    if(!E){
        return NULL;
    }
    E->nom = (char*)malloc(sizeof(char)*(strlen(nom)+1));
    strcpy(E->nom,nom);
    E->niveau = niveau;
    E->atten = atten;
    E->defen = defen;
    E->HPenn = HPenn;
    E->attstrenn = attstrenn;
    return E;
}

void AfficheEnnemi(Ennemi* E) {
    if (E) {
        printf("| %-20s | %8d | %8d | %8d | %8d | %8d |\n",
               E->nom, E->niveau, E->atten, E->defen, E->HPenn, E->attstrenn);
    } else {
        printf("| %-20s | %8s | %8s | %8s | %8s | %8s |\n",
               "NULL", "-", "-", "-", "-", "-");
    }
}


typedef struct noeudEnnemi{
    Ennemi* E;
    struct noeudEnnemi* suivant;
}noeudEnnemi;
typedef noeudEnnemi* ListeEnnemi;

ListeEnnemi AlloueCelluleEnnemi(Ennemi* E){
    ListeEnnemi tmp = (ListeEnnemi)malloc(sizeof(noeudEnnemi));
    if(!tmp){
        return NULL;
    }
    tmp->E = E;
    tmp->suivant = NULL;
    return tmp;
}

void AjouteEnnemiListe(ListeEnnemi* lst,Ennemi* E){
    ListeEnnemi Ajoute = AlloueCelluleEnnemi(E);
    if(!Ajoute){
        return;
    }
    if(!(*lst)){
        (*lst) = Ajoute;
        return;
    }
    ListeEnnemi tmp = *lst;
    while(tmp->suivant){
        tmp = tmp->suivant;
    }
    tmp->suivant = Ajoute;
}

void EnleveEnnemiListe(ListeEnnemi* lst,Ennemi* E){
    if (!(*lst)){
        return;
    }
    if (strcmp(((*lst)->E)->nom,E->nom)==0) {
        ListeEnnemi tmp = *lst;
        *lst = (*lst)->suivant;
        free(tmp);
        return;
    }
    ListeEnnemi tmp = *lst;
    while (tmp->suivant && strcmp(tmp->suivant->E->nom,E->nom)!= 0) {
        tmp = tmp->suivant;
    }
    if (tmp->suivant) {
        ListeEnnemi temp = tmp->suivant;
        tmp->suivant = tmp->suivant->suivant;
        free(temp);
    }
}

void AfficheEnnemiListe(ListeEnnemi lst) {
    printf("+----------------------+----------+----------+----------+----------+----------+\n");
    printf("| Nom                 | Niveau   | att      | def      | HP       | Stress   |\n");
    printf("+----------------------+----------+----------+----------+----------+----------+\n");

    while(lst){
        AfficheEnnemi(lst->E);
        lst = lst->suivant;
    }

    printf("+----------------------+----------+----------+----------+----------+----------+\n");
}

//FIN PARTIE ENNEMIE ##################################
//DEBUT PARTIE Sanitarium##############################

void RegenHPDansSanitarium(ListePersonnage* Sanitarium){ //Sanitarium est une liste de personnage
    if(!(*Sanitarium)){
        return;
    }
    ListePersonnage tmp = *Sanitarium;
    while(tmp){
        tmp->P->HP += 7;
        if(tmp->P->HP > tmp->P->HPmax){
            tmp->P->HP = tmp->P->HPmax;
            printf("Le HP de votre personnage %s est plein!",tmp->P->nom);
        }
        tmp = tmp->suivant;
    }
}

//FIN PARTIE Sanitarium

//Debut partie TAVERNE

void DiminueStressDansTaverne(ListePersonnage* Taverne){
    if(!(*Taverne)){
        return;
    }
    ListePersonnage tmp = (*Taverne);
    while(tmp){
        tmp->P->str -= 25;
        if(tmp->P->str <0){
            tmp->P->str =0;
            printf("Le personnage %s a plus de stress!",tmp->P->nom);
        }
        tmp = tmp->suivant;
    }
}


//fin partie taverne

//DEBUT PARTIE COMBAT#############################

void AttaquePersonnageSurEnnemi(Personnage* P,Ennemi* E){
    float roll= FlottantPourAtt();
    int res = (P->att - E->defen)*roll;
    printf("%d",res);
    E->HPenn -= res;
    if(E->HPenn<0){
        E->HPenn = 0;
    }
}

void AttaqueEnnemiSurPersonnage(Personnage* P, Ennemi* E) {
    int choix = ChoixAttaque();
    printf("\nTour de l'ennemi %s :\n", E->nom);
    if (choix == 1 || P->str == 100) { // Attaque physique
        printf("L'ennemi attaque physiquement %s !\n", P->nom);
        float roll = FlottantPourAtt();
        int res = (P->isDefending == 0) ? (E->atten - P->def) * roll : (E->atten - (P->def * 1.1)) * roll;
        if (res < 0) res = 0;
        printf("Dégâts infligés : %d\n", res);
        P->HP -= res;
        if (P->HP <= 0) P->HP = 0;
    } else if (choix == 2) { // Attaque stressante
        printf("L'ennemi utilise une attaque stressante sur %s !\n", P->nom);
        int strred = (P->A1 ? P->A1->strred : 0) + (P->A2 ? P->A2->strred : 0);
        float roll = FlottantPourAtt();
        int res = (E->attstrenn - strred) * roll;
        if (res < 0) res = 0;
        printf("Stress infligé : %d\n", res);
        P->str += res;
        if (P->str > 100) P->str = 100;
    }
    printf("%s après l'attaque : HP = %d, Stress = %d\n", P->nom, P->HP, P->str);
}

void RegenPersonnage(Personnage* P_Depart,Personnage* P_Destination){
    if(strcmp(P_Depart->nom,P_Destination->nom)==0){
        P_Depart->HP += P_Depart->rest;
        if(P_Depart->HP > P_Depart->HPmax){
            P_Depart->HP = P_Depart->HPmax;
        }
    }
    else{
        P_Destination->HP += P_Depart->rest;
        if(P_Destination->HP>P_Destination->HPmax){
            P_Destination->HP = P_Destination->HPmax;
        }

    }


}


//FIN PARTIE COMBAT###################################

//DEBUT FONCTION JEU PRINCIPALE

Personnage* ChoisirCible(ListePersonnage* lst) {
    if (lst == NULL || *lst == NULL) {
        printf("Erreur: Liste de personnages vide.\n");
        return NULL;
    }

    ListePersonnage tmp = *lst;
    int index = 0;

    printf("Choisissez une cible pour la restauration:\n");
    while (tmp != NULL) {
        printf("%d: %s (HP: %d/%d)\n", index + 1, tmp->P->nom, tmp->P->HP, tmp->P->HPmax);
        tmp = tmp->suivant;
        index++;
    }

    int choix;
    
    while (1) {
        printf("Entrez le numéro de la cible: ");
        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n'); 
            printf("Entrée invalide. Veuillez entrer un numéro valide.\n");
        } else if (choix < 1 || choix > index) {
            printf("Choix invalide. Veuillez entrer un numéro entre 1 et %d.\n", index);
        } else {
            break;  
        }
    }

    tmp = *lst;
    for (int i = 1; i < choix; i++) {
        tmp = tmp->suivant;
    }

    return tmp->P;
}

Personnage* ChoisirCibleAleatoire(ListePersonnage* lst) {
    if (lst == NULL || *lst == NULL) {
        return NULL; // Liste vide
    }

    // Compter les personnages vivants
    int count = 0;
    ListePersonnage tmp = *lst;
    while (tmp != NULL) {
        if (tmp->P->HP > 0) {
            count++;
        }
        tmp = tmp->suivant;
    }

    if (count == 0) {
        return NULL; // Aucun personnage vivant
    }

    // Choisir un index aléatoire
    srand(time(NULL));
    int index = rand() % count;

    // Parcourir la liste jusqu'au personnage choisi
    tmp = *lst;
    for (int i = 0; i < index; i++) {
        if (tmp->P->HP > 0) {
            tmp = tmp->suivant;
        }
    }

    return tmp->P;
}

ListePersonnage ChoixCombattant(ListePersonnage* PersonnageDispo, int niveau) {
    if (!(*PersonnageDispo) || (niveau < 1 || niveau > 10)) {
        printf("Erreur : Liste de personnages vide ou niveau invalide.\n");
        return NULL;
    }

    int maxCombattants = (niveau <= 5) ? 2 : 3;
    printf("Niveau %d : Choix de %d combattant(s) !\n", niveau, maxCombattants);

    char choix[50];
    ListePersonnage Combattants = NULL;

    for (int i = 0; i < maxCombattants; i++) {
        printf("Choisissez le combattant %d/%d :\n", i + 1, maxCombattants);
        AffichePersonnageListe(*PersonnageDispo);

        while (1) { 
            printf("Entrez le nom du personnage ou 'N' pour arrêter : ");
            if (fgets(choix, sizeof(choix), stdin) == NULL) {
                printf("Erreur de lecture. Réessayez.\n");
                continue;
            }
            choix[strcspn(choix, "\n")] = '\0'; // Supprime '\n'

            if (strcmp(choix, "N") == 0) {
                printf("Fin de la sélection des combattants.\n");
                return Combattants; 
            }

            if (RecherchePersonnage(*PersonnageDispo, choix)) {
                Personnage* nouveauCombattant = ExtraitPersonnageListeByNom(PersonnageDispo, choix);
                if (!Combattants) {
                    Combattants = AlloueCellulePersonnage(nouveauCombattant);
                } else {
                    AjoutePersonnageListe(&Combattants, nouveauCombattant);
                }
                printf("Combattant '%s' sélectionné !\n", choix);
                break; 
            } else {
                printf("Choix invalide : '%s'. Veuillez réessayer.\n", choix);
            }
        }
    }

    printf("Fin de la sélection des combattants.\n");
    printf("Vos combattants sont :\n");
    AffichePersonnageListe(Combattants);
    return Combattants;
}


void ChoixAccessoireAuxPersonnage(ListePersonnage combattants, ListeAccessorie* accessoiresDispo) {
    if (!combattants) {
        printf("Aucun combattant sélectionné.\n");
        return;
    }
    if (!accessoiresDispo || !(*accessoiresDispo)) {
        printf("Aucun accessoire disponible.\n");
        return;
    }

    char choix[50];
    ListePersonnage tmp = combattants;
    while (tmp) {
        Personnage* combattant = tmp->P;
        printf("\nAttribuer des accessoires pour le combattant : %s\n", combattant->nom);
        AfficheListeAccessorie(*accessoiresDispo);

        // Premier accessoire
        printf("Choisissez le premier accessoire pour %s (ou entrez 'N' pour ignorer) : ", combattant->nom);
        fgets(choix, sizeof(choix), stdin);
        choix[strcspn(choix, "\n")] = '\0';

        if (strcmp(choix, "N") != 0 && RechercheAccessorieByNom(*accessoiresDispo, choix)) {
            Accessorie* premierAccessoire = ExtraitAccesorieListeByNom(accessoiresDispo, choix);
            AjouteAccessoriePersonnage(combattant, premierAccessoire);
            printf("Accessoire '%s' attribué à %s.\n", premierAccessoire->nom, combattant->nom);
        } else {
            printf("Aucun accessoire attribué pour le premier choix.\n");
        }

        // Vérification pour le second accessoire
        if (*accessoiresDispo) {
            printf("Choisissez le second accessoire pour %s (ou entrez 'N' pour ignorer) : ", combattant->nom);
            fgets(choix, sizeof(choix), stdin);
            choix[strcspn(choix, "\n")] = '\0';

            if (strcmp(choix, "N") != 0 && RechercheAccessorieByNom(*accessoiresDispo, choix)) {
                Accessorie* secondAccessoire = ExtraitAccesorieListeByNom(accessoiresDispo, choix);
                AjouteAccessoriePersonnage(combattant, secondAccessoire);
                printf("Accessoire '%s' attribué à %s.\n", secondAccessoire->nom, combattant->nom);
            } else {
                printf("Aucun accessoire attribué pour le second choix.\n");
            }
        } else {
            printf("Aucun accessoire disponible pour le second choix.\n");
        }

        tmp = tmp->suivant;
    }

    printf("\nTous les accessoires ont été attribués.\n");
}

char RecupereAction(Personnage* P){
    char choix;
    printf("Action de %s (A,D,R) : ",P->nom);
    scanf( "%c", &choix);
    getchar();

    while(choix != 'A' && choix != 'D' && choix != 'R') {
        printf("Choix invalide! Veuillez entrer A, D ou R : ");
        scanf(" %c", &choix);
        getchar();
    }
    return choix;
}


void DerouleCombat(ListePersonnage* lst, Ennemi* E) {
    if (lst == NULL || *lst == NULL || E == NULL) {
        printf("Erreur : Liste de personnages ou ennemi invalide.\n");
        return;
    }

    ListePersonnage tmp;
    char choix;
    printf("\n=== Début du combat ===\n");
    AfficheEnnemi(E);

    while (E->HPenn > 0) {
        tmp = *lst;

        while (tmp) {
            if (tmp->P->HP > 0) {
                printf("\nAction de %s (A, D, R) : ", tmp->P->nom);
                choix = RecupereAction(tmp->P);

                if (choix == 'A') {
                    printf("%s attaque l'ennemi !\n", tmp->P->nom);
                    AttaquePersonnageSurEnnemi(tmp->P, E);
                    printf("Dommages infligés : %d HP, %d HP restants.\n", (tmp->P->att - E->defen), E->HPenn);
                } else if (choix == 'D') {
                    printf("%s se défend !\n", tmp->P->nom);
                    DefensePerso(tmp->P);
                } else if (choix == 'R') {
                    printf("%s restaure un allié !\n", tmp->P->nom);
                    Personnage* cible = ChoisirCible(lst);
                    if (cible) {
                        RegenPersonnage(tmp->P, cible);
                        printf("%d HP restaurés à %s, %d HP sur %d max.\n", tmp->P->rest, cible->nom, cible->HP, cible->HPmax);
                    }
                }
            }
            tmp = tmp->suivant;
        }

        if (E->HPenn <= 0) {
            printf("\n=== Combat terminé ===\n");
            printf("L'ennemi %s a été vaincu !\n", E->nom);
            return;
        }

        printf("\nTour de l'ennemi !\n");
        Personnage* victime = ChoisirCibleAleatoire(lst);
        if (victime) {
            AttaqueEnnemiSurPersonnage(victime, E);
            printf("L'ennemi attaque %s pour %d HP. %s descend à %d HP.\n",
                   victime->nom, E->atten, victime->nom, victime->HP);
            if (victime->HP <= 0) {
                printf("%s est mort et atteint les profondeurs du donjon.\n", victime->nom);
                EnlevePersonnageDeListe(lst, victime);
            }
        }
    }
}


ListePersonnage CreerPersonnages(Classe* Furie, Classe* Vestale, Classe* Chasseur, Classe* MaitreChien) {
    Personnage* Boudicca = CreerPersonnage("Boudicca", Furie, 0, 0);
    Personnage* Junia = CreerPersonnage("Junia", Vestale, 0, 0);
    Personnage* Flash = CreerPersonnage("Flash", Chasseur, 0, 0);
    Personnage* Gordi = CreerPersonnage("Gordi", MaitreChien, 0, 0);
    Personnage* Tritus = CreerPersonnage("Tritus", Chasseur, 0, 0);
    Personnage* Ragnard = CreerPersonnage("Ragnard", Furie, 0, 0);
    ListePersonnage personnages = AlloueCellulePersonnage(Boudicca);
    AjoutePersonnageListe(&personnages, Junia);
    AjoutePersonnageListe(&personnages, Flash);
    AjoutePersonnageListe(&personnages, Gordi);
    AjoutePersonnageListe(&personnages, Tritus);
    AjoutePersonnageListe(&personnages, Ragnard);

    return personnages;
}

ListeEnnemi CreerLiseEnnemis() {
    const char* nomsEnnemis[] = {"Brigand", "Squelette", "Goule", "Gargouille", "Zombie",
        "Vampire", "Lord Voldemort", "Sauron", "Dragon", "Seigneur Noir"};
    ListeEnnemi listeEnnemis = NULL;
    for (int i = 1; i <= 10; i++) {
        int attenn = 3 * i;
        int defenn = 2 + i;
        int HPenn = 10 + 3 * i;
        int attstrenn = 0;  // Déclaration ici
        if (i >= 3) {
            attstrenn = 5 * (i - 2);
        }
        Ennemi* ennemi = CreerEnemi(nomsEnnemis[i - 1], i, attenn, defenn, HPenn, attstrenn);
        AjouteEnnemiListe(&listeEnnemis, ennemi);
    }
    return listeEnnemis;
}

ListeAccessorie CreerAccessoires() {
    ListeAccessorie accessoires = NULL;
    
    AjouteAccessorieListe(&accessoires, CreerAccesorie("Pendatif Tranchant", 5, 1, 0, 0, 0,0));
    AjouteAccessorieListe(&accessoires, CreerAccesorie("Bouclier Enchanté", 0, 5, 0, 0, 0,0));
    AjouteAccessorieListe(&accessoires, CreerAccesorie("Anneau Vital", 0, 0, 10, 0, 0,0));
    AjouteAccessorieListe(&accessoires, CreerAccesorie("Potion de Guérison", 0, 0, 0, 5, 0,0));
    AjouteAccessorieListe(&accessoires, CreerAccesorie("Cape de Résistance", 0, 2, 0, 0, 2,0));
    AjouteAccessorieListe(&accessoires, CreerAccesorie("Dague Mortelle", 7, 0, 0, 0, 0,0));
    AjouteAccessorieListe(&accessoires, CreerAccesorie("Amulette Dorée", 2, 3, 0, 0, 1,0));
    AjouteAccessorieListe(&accessoires, CreerAccesorie("Talisman Ancestral", 0, 0, 15, 2, 0,0));
    AjouteAccessorieListe(&accessoires, CreerAccesorie("Heaume Protecteur", 0, 6, 5, 0, 0,0));
    AjouteAccessorieListe(&accessoires, CreerAccesorie("Épée Légendaire", 10, 0, 0, 0, 0,0));

    return accessoires;
}

ListeEnnemi InitialiserNiveaux() {
    return CreerLiseEnnemis();
}
void GestionSanitariumEtTaverne(ListePersonnage* disponibles, ListePersonnage* sanitarium, ListePersonnage* taverne) {
    char choix[50];

    printf("\n=== Gestion du Sanitarium ===\n");
    RegenHPDansSanitarium(sanitarium);
    ListePersonnage tmp = *sanitarium;
    ListePersonnage precedent = NULL;

    while (tmp) {
        printf("\nVoulez-vous faire sortir %s du sanitarium ? (Y/N) : ", tmp->P->nom);
        fgets(choix, sizeof(choix), stdin);
        choix[strcspn(choix, "\n")] = '\0';

        if (strcmp(choix, "Y") == 0 || strcmp(choix, "y") == 0) {
            Personnage* perso = tmp->P;
            if (precedent == NULL) {
                *sanitarium = tmp->suivant;
            } else {
                precedent->suivant = tmp->suivant; 
            }

            // Libère la cellule actuelle
            ListePersonnage aLiberer = tmp;
            tmp = tmp->suivant;
            free(aLiberer);

            // Ajouter le personnage à la liste des disponibles
            AjoutePersonnageListe(disponibles, perso);
            printf("%s a quitté le sanitarium et est maintenant disponible.\n", perso->nom);
        } else {
            precedent = tmp;
            tmp = tmp->suivant;
        }
    }
    int nbPersonnagesSanitarium = CalculeLenDeListePerso(*sanitarium);
    if (nbPersonnagesSanitarium < 2) {
        printf("\nIl y a de la place pour %d personnage(s) au sanitarium.\n", 2 - nbPersonnagesSanitarium);
        while (nbPersonnagesSanitarium < 2) {
            printf("Indiquez le nom d'un personnage à envoyer au sanitarium (ou 'N' pour ignorer) : ");
            fgets(choix, sizeof(choix), stdin);
            choix[strcspn(choix, "\n")] = '\0';

            if (strcmp(choix, "N") == 0 || strcmp(choix, "n") == 0) {
                printf("Fin des ajouts au sanitarium.\n");
                break;
            }

            if (RecherchePersonnage(*disponibles, choix)) {
                Personnage* perso = ExtraitPersonnageListeByNom(disponibles, choix);
                AjoutePersonnageListe(sanitarium, perso);
                printf("%s a été envoyé au sanitarium.\n", perso->nom);
                nbPersonnagesSanitarium++;
            } else {
                printf("Personnage introuvable : '%s'. Réessayez.\n", choix);
            }
        }
    } else {
        printf("\nLe sanitarium est plein.\n");
    }

    printf("\n=== Gestion de la Taverne ===\n");
    DiminueStressDansTaverne(taverne);
    tmp = *taverne;
    precedent = NULL;

    while (tmp) {
        printf("\nVoulez-vous faire sortir %s de la taverne ? (Y/N) : ", tmp->P->nom);
        fgets(choix, sizeof(choix), stdin);
        choix[strcspn(choix, "\n")] = '\0';

        if (strcmp(choix, "Y") == 0 || strcmp(choix, "y") == 0) {
            Personnage* perso = tmp->P;
            if (precedent == NULL) {
                *taverne = tmp->suivant;
            } else {
                precedent->suivant = tmp->suivant;
            }
            ListePersonnage aLiberer = tmp;
            tmp = tmp->suivant;
            free(aLiberer);
            AjoutePersonnageListe(disponibles, perso);
            printf("%s a quitté la taverne et est maintenant disponible.\n", perso->nom);
        } else {
            precedent = tmp;
            tmp = tmp->suivant;
        }
    }
    printf("\nIndiquez le nom d'un personnage à envoyer à la taverne (ou 'N' pour ignorer) : ");
    while (1) {
        fgets(choix, sizeof(choix), stdin);
        choix[strcspn(choix, "\n")] = '\0';

        if (strcmp(choix, "N") == 0 || strcmp(choix, "n") == 0) {
            printf("Fin des ajouts à la taverne.\n");
            break;
        }

        if (RecherchePersonnage(*disponibles, choix)) {
            Personnage* perso = ExtraitPersonnageListeByNom(disponibles, choix);
            AjoutePersonnageListe(taverne, perso);
            printf("%s a été envoyé à la taverne.\n", perso->nom);
        } else {
            printf("Personnage introuvable : '%s'. Réessayez.\n", choix);
        }
    }
}

void JouerNiveau(ListePersonnage* personnages, ListeEnnemi* ennemis, ListeAccessorie* accessoires, int niveau, int* orTotal) {
    printf("\n--- Niveau %d ---\n", niveau);
    ListeEnnemi tmpEnnemi = *ennemis;
    for (int i = 1; i < niveau && tmpEnnemi; i++) {
        tmpEnnemi = tmpEnnemi->suivant;
    }
    if (!tmpEnnemi || !tmpEnnemi->E) {
        printf("Erreur : Aucun ennemi pour ce niveau.\n");
        return;
    }
    Ennemi* ennemi = tmpEnnemi->E;
    ListePersonnage combattants = ChoixCombattant(personnages, niveau);
    if (!combattants) {
        printf("Aucun combattant sélectionné. Partie terminée.\n");
        exit(0);
    }
    ChoixAccessoireAuxPersonnage(combattants, accessoires);
    DerouleCombat(&combattants, ennemi);
    if (combattants == NULL) {
        printf("Vous avez perdu au niveau %d. Partie terminée.\n", niveau);
        exit(0);
    }

    printf("\nVictoire au niveau %d !\n", niveau);
    *orTotal += 10;
    printf("Or gagné : 10 (Total : %d)\n", *orTotal);
    Accessorie* nouveauAccessoire = CreerAccesorie("Accessoire Spécial", 5, 2, 10, 3, 0, 0);
    AjouteAccessorieListe(accessoires, nouveauAccessoire);
    printf("Récompense : %s ajouté aux accessoires disponibles.\n", nouveauAccessoire->nom);
    if (niveau % 2 == 0) {
        Personnage* nouveauPerso = NULL;
        if (niveau == 2) nouveauPerso = CreerPersonnage("Nouveau Vestale", CreerClasse("Vestale", 3, 0, 20, 10), 0, 0);
        if (niveau == 4) nouveauPerso = CreerPersonnage("Nouveau Maître Chien", CreerClasse("Maître Chien", 10, 6, 17, 5), 0, 0);
        if (niveau == 6) nouveauPerso = CreerPersonnage("Nouveau Chasseur", CreerClasse("Chasseur de Primes", 7, 3, 25, 3), 0, 0);
        if (niveau == 8) nouveauPerso = CreerPersonnage("Nouvelle Furie", CreerClasse("Furie", 13, 0, 20, 0), 0, 0);
        if (nouveauPerso) {
            AjoutePersonnageListe(personnages, nouveauPerso);
            printf("Un nouveau personnage rejoint votre équipe : %s\n", nouveauPerso->nom);
        }
    }
    ListePersonnage sanitarium = NULL;
    ListePersonnage taverne = NULL;
    GestionSanitariumEtTaverne(personnages, &sanitarium, &taverne);
}

void GererNiveaux() {
    Classe* Furie = CreerClasse("Furie", 13, 0, 20, 0);
    Classe* Vestale = CreerClasse("Vestale", 3, 0, 20, 10);
    Classe* Chasseur = CreerClasse("Chasseur de Primes", 7, 3, 25, 3);
    Classe* MaitreChien = CreerClasse("Maître Chien", 10, 6, 17, 5);

    ListePersonnage personnages = CreerPersonnages(Furie, Vestale, Chasseur, MaitreChien);
    ListeEnnemi ennemis = InitialiserNiveaux();
    ListeAccessorie accessoires = CreerAccessoires();

    int orTotal = 0;

    for (int niveau = 1; niveau <= 10; niveau++) {
        JouerNiveau(&personnages, &ennemis, &accessoires, niveau, &orTotal);
    }

    printf("\nFélicitations ! Vous avez terminé tous les niveaux !\n");
}

void LibererClasse(Classe* C) {
    if (C) {
        free(C->nom);
        free(C);
    }
}

void LibererAccessoire(Accessorie* A) {
    if (A) {
        free(A->nom);
        free(A);
    }
}



//FIN FONCTIONS DE PARTIE PRINCIPALE

int main() {
    srand(time(NULL)); 
    GererNiveaux();
    return 0;
}


//COMPILATION: clang -std=c17 -Wall -Wextra -Werror Projet.c -o Projet
//COMPILATION sur windows/powershell terminal: gcc Projet.c -o Projet