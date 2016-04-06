// ClosedHashing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#include "string.h"

#define SIZE 1000

typedef struct Player{
    int id;
    char name[100];
    char classPlayer[100];
    float points;
    struct Player* nextPlayer;
	int hashValue;
}Player;

//Function Signatures
int hashFunction(int id);
void manualInsertPlayers(Player* table[]);
void searchPlayers(Player* table[]);
void insertPlayer(Player* table[], Player player);
void deletePlayer(Player* table[], int id);
void readFile(Player* table[], FILE *fp);
void applicationMenu(Player* table[], FILE *fp);
void showPlayers(Player* table[]);

int _tmain(int argc, _TCHAR* argv[])
{
    Player* table[SIZE];
	FILE *fp;
    int i = 0;

	fp = fopen("AlunosIFE.txt", "rt");

	//table start
    for(i; i < SIZE; i++)
        table[i] = NULL;

	applicationMenu(table, fp);

    
   // system("PAUSE");
    return 0;
}

int hashFunction(int id){
    return id % SIZE;
}

void manualInsertPlayers(Player* table[]){
    Player player;

    printf("\nId: "); scanf("%d", &player.id); fflush(stdin);
    printf("\nNome: "); gets(player.name); fflush(stdin);
    printf("\nClasse: "); gets(player.classPlayer); fflush(stdin);
    printf("\nPontos: "); scanf("%f", &player.points); fflush(stdin);

    insertPlayer(table, player);
}

void searchPlayers(Player* table[]){
	int id = 0, position = 0, listPosition = 0;
	Player* helper = (Player*) malloc(sizeof(Player));

	printf("Informe o o ID do Player: ");
	scanf("%d", &id);

	position = hashFunction(id);

	helper = table[position];

	if(helper == NULL)
		printf("\nJOGADOR NAO ENCONTRADO!!");
	else{
		while(helper!= NULL){
			if(helper->id == id){
				printf("\n%s - Posicao Hash %d - Posicao Lista %d", helper->name, helper->hashValue, listPosition);
			}
			else if((helper->id != id) && (helper->nextPlayer == NULL))
				printf("\nJOGADOR NAO ENCONTRADO!!!!");

			helper = helper->nextPlayer;
			listPosition++;
		}
	}

}

void insertPlayer(Player* table[], Player player){
    int position = 0;

    Player* newPlayer = (Player*) malloc(sizeof(Player));
    Player* helper = (Player*) malloc(sizeof(Player));

    newPlayer->id = player.id;
    strcpy(newPlayer->name, player.name);
    strcpy(newPlayer->classPlayer, player.classPlayer);
    newPlayer->points = player.points;

    position = hashFunction(player.id);

	newPlayer->hashValue = position;
	newPlayer->nextPlayer =  NULL;

    if(table[position] ==  NULL){
        table[position] = newPlayer;
    }
    else{
        helper = table[position];
        while(helper->nextPlayer != NULL){
           helper = helper->nextPlayer;
        }
        helper->nextPlayer = newPlayer;
    }
}


void deletePlayer(Player* table[], int id) {
    int position = hashFunction(id);
    Player* helper;

    if (table[position] != NULL) {

        if (table[position]->id == id) {
            helper = table[position];
            table[position] = table[position]->nextPlayer;
            free(helper);
			printf("\nJOGADOR EXCLUIDO");
        } else {
            helper = table[position]->nextPlayer;
            Player* precedentPlayer = table[position];
            while (helper != NULL && helper->id != id) {
                helper = helper->nextPlayer;
                precedentPlayer = helper;
            }
            if (helper != NULL) {
                precedentPlayer->nextPlayer = helper->nextPlayer;
                free(helper);
				printf("\nJOGADOR DELETADO");
            } else {
                printf("\n\nJOGADOR NAO ENCONTRADO");
            }
        }

    } else
        printf("\n\nJOGADOR NAO ENCONTRADO");
}

void readFile(Player* table[], FILE *fp){
	char nameAndClass[200];
	
	int h, y, quant;//index for name and classes

	char character;
	int i = 0; 
	int id = 0, position = 0;
	float points = 0;
	char classPlayer[100];
	char name[100];
	
	//SCROLL TO THE END OF FILE
	while(!feof(fp)){
		
		//GETTING ID         
		fscanf(fp,"%d", &id); 
		
		//JUMPING THE CHARACTER ; 
		fgetc(fp); 
		
		//GETTING NAME AND CLASS

		y = 0;
		quant = 0;
		while(quant < 2){
			fscanf(fp, "%c", &character);
			if(character == ';')
				quant++;
			if(quant < 2){
				nameAndClass[y] = character;
				y++;
			}
		}

		nameAndClass[y] = '\0';
		
		///separating the name and class putting in their respective vectors
		int z = 0;
		int x = 0;
		while(1){
			name[z] = nameAndClass[z];

			if(nameAndClass[z] == ';'){
				name[z] = '\0';
				while(nameAndClass[z] != '\0'){
					classPlayer[x] = nameAndClass[z+1];
					x++; z++;
				}
				classPlayer[y] = '\0';
				break;
			}

			z++;
		}

		//GETTING THE POINTS
		fscanf(fp,"%f", &points);

		//ADDING IN THE HASH TABLE
		position = hashFunction(id);
		Player player;

		player.id = id;
		strcpy(player.name, name);
		strcpy(player.classPlayer, classPlayer);
		player.points = points;

		insertPlayer(table, player);
		i++;
	}
	fclose(fp);
}

void showPlayers(Player* table[]){
	int i, listPosition;
	Player* helper;
	
	for(i = 0; i < SIZE; i++){
		if(table[i] != NULL){

			 helper = table[i]; 
			 listPosition = 0;

			 while(helper != NULL){
				 printf("\n%s - Posicao Hash %d - Posicao Lista %d", helper->name, helper->hashValue, listPosition);
				 helper = helper->nextPlayer;
				 listPosition++;
			}
		}
	}
}

void applicationMenu(Player* table[], FILE *fp){
	int selected = 0, loop = 1, id = 0;

	do{
        printf("INFORME O NUMERO CORRESPONDENTE A OPCAO\n");
        printf("1 - Carregar arquivo\n");
        printf("2 - Pesquisar Jogador\n");
		printf("3 - Exibir jogadores\n");
        printf("4 - Deletar jogador\n");
        printf("5 - Sair\n");
        printf("=> ");scanf("%d", &selected);

		if(selected < 1 || selected > 5)
			printf("OPCAO INVALIDA");
		else{
			switch(selected){

				case 1:
					readFile(table, fp);
				   // manualInsertPlayers(table);
					printf("\nARQUIVO CARREGADO!");
					break;

				case 2:
					searchPlayers(table);
					break;

				case 3: 
					showPlayers(table);
					break;

				case 4:
					printf("\nDigite o ID do jogador: "); scanf("%d", &id);
					deletePlayer(table, id);
					break;

				case 5:
					loop = 0;
			}
			printf("\n================================================\n\n");
		}
    }while(loop);
	
}


	


