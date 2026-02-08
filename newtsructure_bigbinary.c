#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Permet d’utiliser des variables de type booléen (’false" ou "true
#define BASE 2

typedef struct{
    int *Tdigits; // Tableau de bits : Tdigits [0] = bit de poids fort , Tdigits [ Taille -1] = bit de poids faible
    int Taille; // Nombre de bits significatifs de l ’ entier
    int Signe; // +1 pour positif , -1 pour négatif , 0 pour nul
} BigBinary;

BigBinary initBigBinary(int taille, int signe){
    BigBinary nb;
    nb.Taille = taille;
    nb.Signe = signe; 
    nb.Tdigits = malloc(sizeof(int) * taille);
    for(int i = 0; i < taille; i++){
        nb.Tdigits[i] = 0;
    }
    return nb;
}

//creation d'un bigbinaire depuis une chaine binaire !!
BigBinary creerBigBinaryDepuisChaine(const char *chaine){
    BigBinary nb;
    int n = strlen(chaine);
    nb.Taille = 0;

    // Comptons uniquement les caractères valides (’0’ ou ’1’)
    for(int i = 0; i < n; i++){
        if(chaine[i] == '0' || chaine[i] == '1'){
            nb.Taille++;
        }
    }

    nb.Tdigits = malloc(sizeof(int) * nb.Taille);
    nb.Signe = +1;
    int index = 0;
    int tousZeros = 1;

    for(int i = 0; i < n; i++){
        if(chaine[i] == '0' || chaine[i] == '1'){
            nb.Tdigits[index] = chaine[i] - '0';
            if(nb.Tdigits[index] == 1) tousZeros = 0;
            index++;
        }
    }

    if (tousZeros) nb.Signe = 0;

    return nb;
}

//liberation de la memoire
void libereBigBinary(BigBinary *nb){
    free(nb->Tdigits);
    nb->Tdigits = NULL;
    nb->Taille = 0;
    nb->Signe = 0;
}

void afficheBigBinary(BigBinary nb){ // pour afficher les valeurs au depart
    if(nb.Signe == -1) printf("-");
    if(nb.Signe == 0 || nb.Taille == 0){
        printf("0\n");
        return;
    } 
    for(int i = 0; i < nb.Taille; i++){
        printf("%d", nb.Tdigits[i]);
    }
    printf("\n");
}

void divisePar2(BigBinary *nb){ // pour diviser par 2 le nombre ecrit en binaire ex: 10110 devient 1011 apres division par 2 et verifier si il st nul aussi
    if(nb->Tdigits[0] == 0 && nb->Taille < 1){ // pour verifier si le nbre est nul dans ce cas la 0 divisé par 0 = 0 donc on ne change rien 
    }
    else{
        nb->Taille--; // il suffit de diminuer la taille de 1 pour obtenir la division par 2 car 10110 par exemple devient --> 1011
    }
}

BigBinary addition(BigBinary nb, BigBinary nb2){ // fonction addition
    int Taillemax;
    if(nb.Taille > nb2.Taille){
        Taillemax = nb.Taille;
    }
    if(nb2.Taille >= nb.Taille){ // >= car dans le cas ou la taille des deux nbres est la meme, on peut prendre la taille d'un des deux
        Taillemax = nb2.Taille;
    }
   
    int tab1[Taillemax];// on initialise deux tableaux qui seront des copies pour stocker les tableaux des binaires à sommer avecla taillemax trouvee au dessus
    int tab2[Taillemax];

    BigBinary nbres = initBigBinary(Taillemax+1, +1); // +1 pour la retenue
    
    for(int i = 0; i < nb.Taille; i++){ 
            tab1[i] = nb.Tdigits[i];
    }
    for(int i = 0; i < nb2.Taille; i++){ 
            tab2[i] = nb2.Tdigits[i];
    }

    if(nb.Taille < Taillemax){ // pour avoir deux nombres de meme taille, on rajoute des zeros pour arriver a la meme taille
        for(int i = nb.Taille; i < Taillemax; i++){
            tab1[i] = 0;
        }
    }
    if(nb2.Taille < Taillemax){ // meme chose 
        for(int i = nb2.Taille; i < Taillemax; i++){
            tab2[i] = 0;
        }
    }
    
    int retenue = 0;
    for(int i = Taillemax-1; i >= 0; i--){ // somme de chaque digits en partant de la droite pour gerer les retenues d'ou i--
        if(tab1[i] + tab2[i] + retenue == 2){ // quand ca fait 2 on met 0 et on rajoute retenue a la colonne d'apres
            nbres.Tdigits[i] = 0;
            retenue = 1;
        }
        else if(tab1[i] + tab2[i] + retenue == 3){ // else if cad si le if juste avant est faux / quand ca fait 3 on met 1 et on rajoute retenue a la colonne d'apres  
            nbres.Tdigits[i] = 1;
            retenue = 1;
        }
        else{
            nbres.Tdigits[i] = tab1[i] + tab2[i] + retenue; // si le else if est faux 
            retenue = 0;
        }
    }

    if(retenue == 1){ // quand il reste une retenue apres la somme de la colonne la plus a gauche donc la premiere 
        for(int i = Taillemax; i > 0; i--){ // au cas ou il y a une retenue a la fin on decale tout pour rajouter 1 bits de poids fort au premier digits a gauche 
            nbres.Tdigits[i] = nbres.Tdigits[i-1];
        }

        nbres.Tdigits[0] = 1; // la retenue au premier digits (bits de poids fort)

        return nbres;
    }
    else{
        BigBinary nbres2 = initBigBinary(Taillemax, +1); // cas ou il n'y a pas de retenue on est oblige de creer un bigbinary avec taillemax-1 
        for(int i = 0; i < Taillemax-1; i++){  // quand il n'y a pas de retenue
            nbres2.Tdigits[i] = nbres.Tdigits[i];
        }

        return nbres2;
    }
}

BigBinary soustraction(BigBinary nb, BigBinary nb2){ // fonction soustraction 
    int Taillemax;
    if(nb.Taille > nb2.Taille){
        Taillemax = nb.Taille;
    }
    if(nb2.Taille >= nb.Taille){ // >= car dans le cas ou la taille des deux nbres est la meme, on peut prendre la taille d'un des deux
        Taillemax = nb2.Taille;
    }

    int tab1[Taillemax];// on initialise deux tableaux qui seront des copies pour stocker les tableaux des binaires à sommer avec la taillemax trouvee au dessus
    int tab2[Taillemax];

    BigBinary nbres = initBigBinary(Taillemax, +1); 
    
    for(int i = 0; i < nb.Taille; i++){ 
            tab1[i] = nb.Tdigits[i];
    }
    for(int i = 0; i < nb2.Taille; i++){ 
            tab2[i] = nb2.Tdigits[i];
    }

    if(nb.Taille < Taillemax){ // pour avoir deux nombres de meme taille, on rajoute des zeros pour arriver a la meme taille
        for(int i = nb.Taille; i < Taillemax; i++){
            tab1[i] = 0;
        }
    }
    if(nb2.Taille < Taillemax){ // meme chose 
        for(int i = nb2.Taille; i < Taillemax; i++){
            tab2[i] = 0;
        }
    }
    
    for(int i = Taillemax-1; i >= 0; i--){ // difference de chaque digits en partant de la droite d'ou i--
        if(tab1[i] >= tab2[i] ){ // quand on peut faire la soustraction cad tous les cas sauf 0-1
            nbres.Tdigits[i] = tab1[i] - tab2[i];
        }
        
        else{ // si jamais on a 0 - 1 on cherche dans chaque colonne vers la gauche un 1 à emprunter, des quon le trouve on met la colonne ou on l'a trouvé à 0 et toutes les colonnes entre celle ci et celle
            // (suite:) ou on se trouvait , on les met à 1
            for(int temp = i; temp>=0; temp--){
                if(tab1[i-1] == 0){
                    tab1[i-1] = 1;
                    temp--;
                }
                else{
                    tab1[i-1] = 0;
                    nbres.Tdigits[i] = 1; // la colonne ou on avaut 0 - 1 on met que c'est egal a 1 et on sort de la boucle for avec le break
                    break;
                }
            }  
        }
    }

    return nbres;
}

bool egal(BigBinary nb, BigBinary nb2){ // fonction de comparaison A==B
    if(nb.Taille == nb2.Taille){
        for(int i = 0; i < nb.Taille; i++){ // on prend la taille d'un des deux peu importe puisque cest =
            if(nb.Tdigits[i] != nb2.Tdigits[i]){
                return false; // des que deux digits sont != on quitte la boucle 
            }
        }
        return true; // si ils sont tous egaux on renvoie true
    }
    else{ // si ils n'ont pas la meme taille on sait deja qu'ils sont !=
        return false;
    }
}

bool inferieur(BigBinary nb, BigBinary nb2){ // fonction de comparaison A<B
    if(nb.Taille < nb2.Taille){ //si taille A < taille B donc A < B
        return true;
    }
    if(nb.Taille > nb2.Taille){ //si taille A > taille B donc A n'est pas < B
        return false;
    }
    else{
        for(int i = 0; i < nb.Taille; i++){ // on prend la taille d'un des deux peu importe puisque cest =
            if(nb.Tdigits[i] < nb2.Tdigits[i]){ // si digits de A < digits de B donc A < B et on sort de la boucle
                return true; // 
            }
            if(nb.Tdigits[i] > nb2.Tdigits[i]){ // si digits de A > digits de B donc A > B et on sort de la boucle
                return false; // 
            }
        }
        return false; // si digits tous egaux donc A = B on renvoie false car A nest pas < B
    }   
}

BigBinary pgcd(BigBinary nb, BigBinary nb2){ 
    int Taillemax; // pour avoir la taillemax qu'on pourrait ensuite utiliser pour le bigbinary resultat
    if(nb.Taille > nb2.Taille){
        Taillemax = nb.Taille;
    }
    if(nb2.Taille >= nb.Taille){ // >= car dans le cas ou la taille des deux nbres est la meme, on peut prendre la taille d'un des deux
        Taillemax = nb2.Taille;
    }

    int tab1[Taillemax];// on initialise deux tableaux qui seront des copies pour stocker les tableaux des binaires à sommer avec la taillemax trouvee au dessus
    int tab2[Taillemax];

    BigBinary nbA = initBigBinary(Taillemax, +1); // copies de a et b ici nb et nb2 pour s'en servir apres dans l'algo d'euclide
    BigBinary nbB = initBigBinary(Taillemax, +1);

    for(int i = 0; i < nb.Taille; i++){ 
            tab1[i] = nb.Tdigits[i];
    }
    for(int i = 0; i < nb2.Taille; i++){ 
            tab2[i] = nb2.Tdigits[i];
    }

    if(nb.Taille < Taillemax){ // pour avoir deux nombres de meme taille, on rajoute des zeros pour arriver a la meme taille
        for(int i = nb.Taille; i < Taillemax; i++){
            tab1[i] = 0;
        }
    }
    if(nb2.Taille < Taillemax){ // meme chose 
        for(int i = nb2.Taille; i < Taillemax; i++){
            tab2[i] = 0;
        }
    }

    for(int i = 0; i < Taillemax; i++){// copies de a et b ici nb et nb2 pour s'en servir apres dans l'algo d'euclide
        nbA.Tdigits[i] = tab1[i];
    }

    for(int i = 0; i < Taillemax; i++){// ''
        nbB.Tdigits[i] = tab2[i];
    }

    //debut de l'algo d'euclide 
    while(egal(nbA, nbB) == false){ // on appelle la fonction egal pour faire tant que a different de b
        if(inferieur(nbA, nbB) == false){ // on appelle la fonction inferieur pour si a > b
            BigBinary vartemp = soustraction(nbA, nbB); 
            libereBigBinary(&nbA); // liberer la memoire du bigbinary pour pouvoir changer ensuite sa valeur en dessous
            nbA = vartemp;
        }
        else{
            BigBinary vartemp = soustraction(nbB, nbA);
            libereBigBinary(&nbB); // liberer la memoire du bigbinary pour pouvoir changer ensuite sa valeur en dessous
            nbB = vartemp;
        }

    }
    return nbA;
}

BigBinary modulo(BigBinary nb, BigBinary nb2){
    int Taillemax; // pour avoir la taillemax qu'on pourrait ensuite utiliser pour le bigbinary resultat
    if(nb.Taille > nb2.Taille){
        Taillemax = nb.Taille;
    }
    if(nb2.Taille >= nb.Taille){ // >= car dans le cas ou la taille des deux nbres est la meme, on peut prendre la taille d'un des deux
        Taillemax = nb2.Taille;
    }

    int tab1[Taillemax];// on initialise deux tableaux qui seront des copies pour stocker les tableaux des binaires à sommer avec la taillemax trouvee au dessus
    int tab2[Taillemax];

    BigBinary nbA = initBigBinary(Taillemax, +1); // copies de a et b ici nb et nb2 pour s'en servir apres dans l'algo d'euclide
    BigBinary nbB = initBigBinary(Taillemax, +1);

    for(int i = 0; i < nb.Taille; i++){ 
            tab1[i] = nb.Tdigits[i];
    }
    for(int i = 0; i < nb2.Taille; i++){ 
            tab2[i] = nb2.Tdigits[i];
    }

    if(nb.Taille < Taillemax){ // pour avoir deux nombres de meme taille, on rajoute des zeros pour arriver a la meme taille
        for(int i = nb.Taille; i < Taillemax; i++){
            tab1[i] = 0;
        }
    }
    if(nb2.Taille < Taillemax){ // meme chose 
        for(int i = nb2.Taille; i < Taillemax; i++){
            tab2[i] = 0;
        }
    }

    for(int i = 0; i < Taillemax; i++){// copies de a et b ici nb et nb2 pour s'en servir apres dans l'algo d'euclide
        nbA.Tdigits[i] = tab1[i];
    }

    for(int i = 0; i < Taillemax; i++){// ''
        nbB.Tdigits[i] = tab2[i];
    }

    // debut algo modulo
    while(inferieur(nbB, nbA) == true){ // on veut si A-B>0 donc si A>B donc si B<A
        BigBinary vartemp = soustraction(nbA, nbB); // on fait A = A - B
        libereBigBinary(&nbA); // liberer la memoire du bigbinary pour pouvoir changer ensuite sa valeur en dessous
        nbA = vartemp; 
    }
    return nbA; // on renvoie le reste qui est A 
}

int main(){

    char chainenb[1026]; // taille suffisamment grande pour 512 digits ou 1024 bits donnes dans les fichiers test ( retenue + caractere \0)
    char chainenb2[1026]; 
    
    scanf("%s", chainenb);
    scanf("%s", chainenb2); 

    BigBinary nb = creerBigBinaryDepuisChaine(chainenb);
    BigBinary nb2 = creerBigBinaryDepuisChaine(chainenb2);

    afficheBigBinary(nb); 
    afficheBigBinary(nb2); 

    BigBinary resultat1 = addition(nb, nb2); // addition
    afficheBigBinary(resultat1);
    libereBigBinary(&resultat1);

    BigBinary resultat2 = soustraction(nb, nb2); // soustraction
    afficheBigBinary(resultat2);
    libereBigBinary(&resultat2);
 
    if(egal(nb, nb2) == 1){ // egal
        printf("true\n");
    }
    else{
        printf("false\n");
    }
     
    if(inferieur(nb, nb2) == 1){ // inferieur
        printf("true\n");
    }
    else{
        printf("false\n");
    }
 
    BigBinary resultat3 = pgcd(nb, nb2); // pgcd
    afficheBigBinary(resultat3);
    libereBigBinary(&resultat3);

    BigBinary resultat4 = modulo(nb, nb2); // modulo
    afficheBigBinary(resultat4);
    libereBigBinary(&resultat4);

    divisePar2(&nb); // division par 2  
    afficheBigBinary(nb);

    libereBigBinary(&nb);
    libereBigBinary(&nb2);
    libereBigBinary(&resultat1);
    libereBigBinary(&resultat2);
    libereBigBinary(&resultat3);
    libereBigBinary(&resultat4);

    return 0;
}