#include "hash.h"
#include <stdlib.h>
#include <assert.h>

void imprime(elemento_lista_t *lista){
  elemento_lista_t *anterior;
  while(lista){
    printf("Chave %lu -- Conteudo %lu -- Endereco %p\n",lista->elemento->chave, *lista->elemento->conteudo, (void *) lista->elemento);
    anterior = lista;
    lista = lista->proximo;
    free(anterior);
  }
}

int verifica_0_9(hash_t *ht){
  unsigned long u;
  for(u=0; u<10; u++){
    if (!ht_busca_conteudo(ht, &u))
      return 0;
  }
  return 1;
}

int verifica_impares(hash_t *ht){
  unsigned long u;
  for(u=1; u<10; u=u+2){
    if (!ht_busca_conteudo(ht, &u))
      return 0;
  }
  for(u=0; u<10; u=u+2){
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

int main(int argc, char ** argv){
  elemento_lista_t * lista;
  int i;
  hash_t * ht = ht_init(0);
  assert(ht==NULL);

  ht = ht_init(10);
  assert(ht);

  /* insere 10 elementos sem colisao */
  for(i=0; i<10; i++){
    conteudo_t * conteudo = (conteudo_t *) calloc(1, sizeof(conteudo_t));


    assert(conteudo);
    *conteudo = i;
    // printf("Insere %d\n", *conteudo);
    ht_insere_conteudo(ht, conteudo);
  }
  printf("Teste ate 10 %s\n", verifica_0_9(ht)?"OK":"ERRO");

  //
  // printf("Lista \n");
  // /* imprime */
  // lista = ht_lista(ht);
  // assert(lista);
  //
  // imprime(lista);
  /* remove os pares */
  for(i=0; i<10; i+=2){
    conteudo_t * cont = (conteudo_t *) malloc(sizeof(conteudo_t));
    assert(cont);
    *cont = i;
    entrada_hash_t * elem = ht_busca_conteudo(ht,cont);
    assert(elem);
    ht_remove_elemento(ht, elem);
    //free(elem);
    printf("passou!!\n");
   // lista = ht_lista(ht);
   // imprime(ht_lista(ht));
  }
  // /* imprime */
  // lista = ht_lista(ht);
  // assert(lista);
  printf("Teste impares %s\n", verifica_impares(ht)?"OK":"ERRO");


  /* colisoes nos impares */
  for(i=1; i<10; i+=2){
    conteudo_t * cont = (conteudo_t *) malloc(sizeof(conteudo_t));
    assert(cont);
    
    *cont = i;
   // ht_insere_conteudo(ht,cont);

    printf("1º %lu\n", *ht->armazenamento[ht_hash(ht, cont)].conteudo); //-----------------------------------

    *cont = *cont + 10;
   // ht_insere_conteudo(ht,cont);


    printf("2º %lu\n", *ht->armazenamento[ht_hash(ht, cont)].conteudo); //-----------------------------------
  }
  // printf("Lista final\n");
  //
  // imprime(ht_lista(ht));
  printf("Teste impares e colisoes %s\n", verifica_impares_colisoes(ht)?"OK":"ERRO");

  /* final cleanout */
  //ht_remove(ht);

  return 0;
}



/*
#include "hash.h"
#include <stdio.h>

int main(int argc, char **argv){
	
	long unsigned int tamanho = 10;						// Tamanho da tabela a ser criada
	long unsigned int valores[5] = {10,20,30,40,50}; 	// Valores como teste inseridos na tabela
	int estado_insert = 0;
	int i;
	

	hash_t *ht = ht_init(10);							// Cria a tabela com tamanho 10 posições

	for (i=0; i < 5; ++i)	{
		estado_insert = ht_insere_conteudo(ht, &valores[i]);
		printf("Estado do INSERT: %i     Conteúdo: %lu\n", estado_insert, valores[i]);
	}

	ht_busca_conteudo(ht, &valores[0]);

	//------------------------BUSCA CHAVE-------------------------

	long unsigned int x = 4;

	if(x < tamanho){				// Verifica se a chave a ser busca está no dentro do intervalo de TAMANHO
		ht_busca_chave(ht, x);
	}

	ht_remove(ht);

	return 0;
}
*/