#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX 100

typedef struct{
  char nome[MAX];
  int ra;
  float p1;
  float p2;
  float t;
  float po;
  float media;
} Aluno;

struct node_st{
  Aluno* aluno;
  struct node_st* prev;
  struct node_st* next;
};
typedef struct node_st node;

struct hlist_st{
  node* head;
};
typedef struct hlist_st hlist;

//PROTOTIPACAO DE FUNCOES AUXILIARES
hlist* criarLista();
float calculaMedia(float, float, float, float);
node* criarNo(char[MAX], int, float, float, float, float);

//FUNCOES DO MENU
bool cadastrarAluno(hlist*, node*);//OK ORDENAR DURANTE O CADASTRO??
int buscarAluno(hlist*, char[MAX]); // OK
int lerDadosArquivo(hlist*, char[MAX]); // OK
int gerarArqAprovados(hlist*); // OK
int gerarArqReprovados(hlist*); // OK


int main(){

  int opcao = 0;
  hlist* lista;

  //VARIAVEIS AUXILIARES
  char nome[MAX], nomearq[MAX];
  int ra, aux_main;
  float p1, p2, t, po;
  node *novo;


  lista = criarLista();

  while(opcao != 6) {
    
    printf("\n\n");

    printf(
          "1- Cadastrar um novo aluno\n"
          "2- Buscar aluno\n"
          "3- Ler arquivo com listagem de alunos\n"
          "4- Gerar arquivo com dados dos alunos aprovados\n"
          "5- Gerar arquivo com dados dos alunos reprovados\n"
          "6- Encerrar\n"
          );

    scanf("%d", &opcao);

    printf("\n\n");

    switch(opcao){
      case 1:
        printf("Informe os dados do aluno a ser cadastrado (seguidos da tecla ENTER):\n");
        scanf(" %[^(0-9)]", nome);
        scanf("%d", &ra);
        scanf("%f", &p1);
        scanf("%f", &p2);
        scanf("%f", &t);
        scanf("%f", &po);

        novo = criarNo(nome, ra, p1, p2, t, po);

        if(cadastrarAluno(lista, novo))
          printf("Aluno cadastrado\n");
        else
          printf("Erro no cadastro\n");

        break;

      case 2:
        printf("Informe o nome para busca:\n");

        scanf("%s", nome);
        aux_main = buscarAluno(lista, nome);
        
        
        if(aux_main == 0)
          printf("Nenhum aluno cadastrado com este nome.\n"); 
        else
          printf("Número de alunos encontrados: %d\n", aux_main);  
        
        break;       
      
      case 3:
        printf("Entre com o nome do arquivo a ser lido: \n");
        scanf("%s", nome);
        
        aux_main = lerDadosArquivo(lista, nome);

        if(aux_main == -1)
          printf("Entrada inválida, por favor tente novamente.\n");        
        else
          printf("Total de alunos cadastrados: %d\n", aux_main);

        break;

      case 4: 
        aux_main = gerarArqAprovados(lista);

        printf("Arquivo \"aprovados.txt\" gerado com sucesso.\n");

        break;

      case 5:
        aux_main = gerarArqReprovados(lista);

        printf("Arquivo \"reprovados.txt\" gerado com sucesso.\n");

        break;
      
      case 6:
        break;
      
      default:
        printf("Opção inválida.\n");
    }
        
  }


}

hlist* criarLista(){
  hlist* lista = malloc(sizeof(hlist));
  lista->head = malloc(sizeof(node));
  lista->head->next = NULL;
  return lista;
}

float calculaMedia(float p1, float p2, float t, float po){
  if(p1 <= p2 && po > p1){
    p1 = po;
  }else if(p2 <= p1 && po > p2){
    p2 = po;
  }

  return (p1*0.35) + (p2*0.35) + (t*0.3);
}

node* criarNo(char nome[MAX], int ra, float p1, float p2, float t, float po){
  node* novo;

  //PADRONIZANDO CADASTRO COM LETRA MAIUSCULA
  for(int i = 0; i < strlen(nome); i++)
    nome[i] = toupper(nome[i]);
  
  //INICIANDO DADOS REFERENTES AO NOVO NÓ
  novo = malloc(sizeof(node));
  novo->aluno = malloc(sizeof(Aluno));
  strcpy(novo->aluno->nome, nome);
  novo->aluno->ra = ra;
  novo->aluno->p1 = p1;
  novo->aluno->p2 = p2;
  novo->aluno->t = t;
  novo->aluno->po = po;
  novo->aluno->media = calculaMedia(p1, p2, t, po);

  novo->next = NULL;
  novo->prev = NULL;

  return novo;
}

bool cadastrarAluno(hlist* lista, node* novo){
  node *atual;
  atual = lista->head->next;

  if(atual == NULL){
    lista->head->next = novo;
    novo->prev = lista->head;
  }else{
    while(atual != NULL){
      
      if(strcmp(novo->aluno->nome, atual->aluno->nome) == 0){
        printf("Aluno ja foi cadastrado.\n");
        return false;
      }   

      //COLOCA ANTES DO NÓ ATUAL E SAI DO LAÇO
      if(strcmp(novo->aluno->nome, atual->aluno->nome) < 0){
        novo->prev = atual->prev;
        novo->next = atual;
        atual->prev->next = novo;
        atual->prev = novo;
        return true;

      }else if(atual->next != NULL){
        //COLOCA ENTRE O NÓ ATUAL E O PROXIMO NÓ E SAI DO LAÇO
        if(strcmp(novo->aluno->nome, atual->aluno->nome) > 0 && strcmp(novo->aluno->nome, atual->next->aluno->nome) < 0){
          novo->next = atual->next;
          novo->prev = atual;
          atual->next->prev = novo;
          atual->next = novo;
          return true;

        //CONTINUA PERCORRENDO A LISTA
        }else if(strcmp(novo->aluno->nome, atual->next->aluno->nome) > 0){
          atual = atual->next;
        }

      }else{
        atual->next = novo;
        novo->prev = atual;
        return true;
      }      
    }
  }
}

int buscarAluno(hlist* lista, char nome[MAX]){
  bool cond;
  int i;
  int cont = 0;

  node *atual = lista->head->next;

  for(i = 0; i < strlen(nome); i++)
    nome[i] = toupper(nome[i]);

  while(atual != NULL){
    cond = true;
    for(i = 0; i < strlen(nome); i++){
      if(nome[i] != atual->aluno->nome[i])
        cond = false;
    }
    if(cond){
      if(atual->aluno->media >= 6.0)
        printf("%s MF: %.2f ST: Aprovado\n", atual->aluno->nome, atual->aluno->media);
      else
        printf("%s MF: %.2f ST: Reprovado\n", atual->aluno->nome, atual->aluno->media);

      cont++;
    }

    atual = atual->next;
  }

  return cont;
}

int gerarArqAprovados(hlist* lista){
  FILE* arq = fopen("aprovados.txt", "w");
  int cont = 0;

  node* atual = lista->head->next;
  if(atual == NULL){
    printf("LISTA VAZIA\n");
    return 0;
  }
  while(atual != NULL){
    if(atual->aluno->media >= 6){
      fprintf(arq, "%s  MF: %.2f\n", atual->aluno->nome, atual->aluno->media);
      cont++;
    }
    atual = atual->next;
  }

  fprintf(arq, "\nAlunos aprovados: %d\n", cont);
  fclose(arq);

  return cont;
}

int gerarArqReprovados(hlist* lista){
  FILE* arq = fopen("reprovados.txt", "w");
  int cont = 0;

  node* atual = lista->head->next;
  if(atual == NULL){
    printf("LISTA VAZIA\n");
    return 0;
  }

  while(atual != NULL){
    if(atual->aluno->media < 6){
      fprintf(arq, "%s  MF: %.2f\n", atual->aluno->nome, atual->aluno->media);
      cont++;
    }
    atual = atual->next;
  }

  fprintf(arq, "\nAlunos Reprovados: %d\n", cont);
  fclose(arq);

  return cont;
}

int lerDadosArquivo(hlist* lista, char nomearq[MAX]){
  FILE* arq = fopen(nomearq, "r");
  
  //DADOS PARA CADASTRO
  char nome[MAX] = "\0";
  int ra, cont;
  float p1, p2, t, po; 

  if(arq != NULL){
    cont = 0;
    while(fscanf(arq, " %[^(0-9)]", nome) != -1){
      fscanf(arq, "%d", &ra);
      fscanf(arq, "%f", &p1);
      fscanf(arq, "%f", &p2);
      fscanf(arq, "%f", &t);
      fscanf(arq, "%f", &po);
      if(cadastrarAluno(lista, criarNo(nome, ra, p1, p2, t, po)))
        cont++;
    }
  }else{
    return -1;
  }

  fclose(arq);
  return cont;
}
