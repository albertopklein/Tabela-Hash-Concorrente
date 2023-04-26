#include "hash.h"
#include <stdlib.h>
#include <assert.h>
//#include <pthread.h>

// ------------------------------------ FIM FUNCOES DE TESTE -------------------------------------

void imprime(elemento_lista_t *lista){
  elemento_lista_t *anterior;
  while(lista){
    printf("Chave %lu -- Conteudo %lu -- Endereco %p\n",lista->elemento->chave, *lista->elemento->conteudo, (void *) lista->elemento);
    anterior = lista;
    lista = lista->proximo;
    free(anterior);
  }
}

void * Teste_pares_insercao (void* ptr) { 
    unsigned long int i;
    conteudo_t *cont;
    hash_t *tabela;
    tabela = (hash_t*) ptr;
    
    //Insere 10 elementos sem colisão 
    //printf("Inserindo elementos:\n");
    for(i = 0; i < 200000000; i+=1) {     //Insere elementos de 1 a 9 na tabela
        cont = malloc(sizeof(conteudo_t));
        *cont = i;
        if(ht_insere_conteudo(tabela, cont)== 0) {
            //printf("\tErro na inserção do elemento %d\n", i);
        }
            
        else {
            //printf("\tElemento %d inserido com sucesso\n", i);
        }
    }
}

void * Teste_impares_insercao (void* ptr) { 
    unsigned long int i;
    conteudo_t *cont;
    hash_t *tabela;
    tabela = (hash_t*) ptr;
    
    //Insere 10 elementos sem colisão 
    //printf("Inserindo elementos:\n");
    for(i = 50000000; i < 100000000; i+=1) {     //Insere elementos de 1 a 9 na tabela
        cont = malloc(sizeof(conteudo_t));
        *cont = i;
        if(ht_insere_conteudo(tabela, cont)== 0) {
           // printf("\tErro na inserção do elemento %d\n", i);
        }            
        else {
            //printf("\tElemento %d inserido com sucesso\n", i);
        }
    }
}


void *busca(void *ht){
  unsigned long u;
  for(u=0; u<100000000; u++){
    if (!ht_busca_conteudo(ht, &u)){
      //printf("ERRO\n");;
    }
  }
  printf("CERTO\n");
}

void *busca1(void *ht){
  unsigned long u;
  for(u=10000; u<20000; u++){
    if (!ht_busca_conteudo(ht, &u))
      printf("ERRO\n");;
  }
  printf("CERTO\n");
}


int verifica_0_19(hash_t *ht){
  unsigned long u;
  for(u=0; u<20; u++){
    if (!ht_busca_conteudo(ht, &u))
      return 0;
  }
  return 1;
}

int verifica_impares(hash_t *ht){
  unsigned long u;
  for(u=1; u<20; u=u+2){
    if (!ht_busca_conteudo(ht, &u))
      return 0;
  }
  for(u=0; u<20; u=u+2){
    if (ht_busca_conteudo(ht, &u))
      return 0;
  }

  return 1;
}

int verifica_impares_colisoes(hash_t *ht){
  unsigned long u;
  for(u=1; u<20; u=u+2){
    if (!ht_busca_conteudo(ht, &u))
      printf("---%lu---%lu---\n", ht_hash(ht, &u), *ht->armazenamento[ht_hash(ht, &u)].conteudo);
      return 1;
  }
  for(u=0; u<20; u=u+2){
    if (ht_busca_conteudo(ht, &u))
      printf("---%lu---\n", *ht->armazenamento[ht_hash(ht, &u)].conteudo);
      return 0;                                               //<------------------------------
  }

  return 1;
}

// ------------------------------------ FIM FUNCOES DE TESTE -------------------------------------

int main () {
    
    
    elemento_lista_t *lista;
    entrada_hash_t *remover;
    hash_t *ht = ht_init(0);
    assert(ht==NULL);
    int i;

    //printf("Criando a tabela:\n");
    if(!(ht = ht_init(20000000))) { //Inicia uma tabela hash
       printf("Erro na criação da tabela");
    }
    else{
        printf("\tTabela criada com sucesso!\n");  
    }

    Teste_pares_insercao(ht);
    //busca(ht);
 
    return 0;
}