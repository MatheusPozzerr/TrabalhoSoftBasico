#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <sys/types.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "stdbool.h"


#define Num_MAX 500
#define Caracter_MAX 64

  #define FATAL_ERROR(Message)						\
  do									\
    {									\
      error (0, 0, (Message));						\
      usage (EXIT_FAILURE);						\
    }									\
  while (0)

void * malloc (size_t size);

enum operating_mode
  {
    undefined_mode,

    /* Output characters that are in the given bytes. */
    byte_mode,

    /* Output the given delimiter-separated fields. */
    field_mode
  };

void
usage (int status)
{
  if (status != 0)
    printf("Tente tagCloud --help para mais informações. \n");
  else
    {
      printf ("\
Uso: tagCloud [OPTION]... [FILE]...\n\
Ou:  tagCloud [OPTION]... --files0-from=F\n\
");
      printf ("\
O programa tem como objetivo emitir as palavras mais utilizadas, podendo ser realizada saídas em formato texto e gráfico\n\
");

      printf ("\
\n\
As opções possíveis estão descritas abaixo:\n\
-c                     Saída com as palavras mais utilizadas em formato csv\n\
-h                     Saída com as palavras mais utilizadas em formato HTML\n\
");
      printf ("--help                 mostra esta ajuda e sai, \n");
      printf ("--version              informa a versão e sai.\n");
    }
  exit (status);
}

void
version (int status)
{
  if (status != 0)
    printf("Tente tagCloud --help para mais informações.");
  else
    {
      printf ("Versão 1.0 \nEste é um software livre: você é livre para alterá-lo e redistribuí-lo. NÃO HÁ QUALQUER GARANTIA, na máxima extensão permitida em lei.\nEscrito por Matheus Pozzer Moraes, Diogo Almeida e Douglas Sobreira.\n");
    }
  exit (status);
}


char** read_from_file (char* nomeArquivo, int *index)
{ 
    char **palavras = malloc (Num_MAX * sizeof *palavras);
    char buf[Caracter_MAX] = {0};
    FILE* arq = fopen (nomeArquivo, "r");

    if (arq == NULL){
        fprintf (stderr, "Can't open file\n");
        exit(1);
    }
    *index = 0;

    while (fgets (buf, Caracter_MAX, arq))
    {
        char *p = buf;
        size_t len = strlen (p);
        while (len && (p[len-1] == '\r' || p[len-1] == '\n'))
            p[--len] = 0;

//        O que o strdup Realiza:
//        char *strdup(const char *src) {
//        char *dst = malloc(strlen (src) + 1);  // Space for length plus nul
//        if (dst == NULL) return NULL;          // No memory
//        strcpy(dst, src);                      // Copy the characters
//        return dst;                            // Return the new string
        palavras[(*index)++] = strdup (buf);

        if (*index == Num_MAX) {
            fprintf (stderr, "warning: N_MAX words read.\n");
            break;
        }
    }
    fclose(arq);

    return palavras;
}

struct palavrasTotais
{
    char palavra[30];
    int qtd;

} palavrasTexto[10000];

bool adicionaVerificaPalavra(char* palavraVerificada, int contador){
    if (contador > 10000)
    {
        printf("\n Caracteres demais a serem processados, o máximo é 10000 \n");
        exit(EXIT_FAILURE);
    }
    
    if (contador == 0)
    {
        strcpy(palavrasTexto[0].palavra, palavraVerificada);
        palavrasTexto[0].qtd = 1;
    }
    else{
        for (size_t i = 0; i < contador; i++)
        {
            if (strcmp(palavrasTexto[i].palavra , palavraVerificada) == 0 )
            {
            palavrasTexto[i].qtd = palavrasTexto[i].qtd + 1;
            return true;
            }
        }
        strcpy(palavrasTexto[contador++].palavra, palavraVerificada);
        palavrasTexto[contador++].qtd = 1;
        return false;
    }
}

int analisaTexto (char** palavras, int numFrases)
{
    int j = 0;
    int contador = 0;
    char * pch;
        for (j = 0; j < numFrases; j++){
            pch = strtok (palavras[j]," ,.-");
            while (pch != NULL)
            {
                bool verifica = adicionaVerificaPalavra(pch, contador);
                if (verifica == false)
                {
                    contador ++;
                }
                pch = strtok (NULL, " ,.-");
            }
    }

    return contador;
}

void imprimeCsv (int contador)
{
    FILE* arqCsv = fopen ("PalavrasUtilizadas.csv", "w");

    fputs("Palavra, Recorrência \r\n", arqCsv);
    for (size_t i = 0; i < contador; i++)
    {
        fprintf(arqCsv, "%s, %i\n", palavrasTexto[i].palavra, palavrasTexto[i].qtd);
    }
    fclose(arqCsv);
}


int
main (int argc, char *argv[])
{
  bool ok;
  char Linha[100];
  char* palavras[20000];
  bool isCsv;
  bool isHtml;
  int i = 0;
  int optc;
  int sizeWord;
  int numPalavras;
  size_t nfiles;
  char* nomeArquivo;
  FILE *arq;
  struct fstatus *fstatus;

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, NULL, _IOLBF, 0);

  int ret;
  ret = strcmp(argv[1] , "--help");
  if (argc > 1){
            if (strcmp(argv[1] , "--help") == 0)
            {
                optc = 0;
            }
            if (strcmp(argv[1] , "--version") == 0){
                optc = 1;
            }
            if (argc >= 2){          
            if (strcmp(argv[1] , "-c") == 0){
                optc = 2;
            }
            if (strcmp(argv[1] , "-h") == 0){
                optc = 3;
            }      
        }       
  }
    

    switch (optc)
    {
    //   case 'c':
    //     print_bytes = true;
    //     break;

    //   case 'm':
    //     print_chars = true;
    //     break;

    //   case 'l':
    //     print_lines = true;
    //     break;

    //   case 'w':
    //     print_words = true;
    //     break;

    //   case 'L':
    //     print_linelength = true;
    //     break;

      case 0: 
      usage(0);
      break;

      case 1: version(0);
      break;

      case 2: isCsv = true;
      break;

      case 3: isHtml = true;
      break; 

      default:
        usage (1);
    }

    if (isCsv == true)
    {   
        nomeArquivo = argv[2];
        char **words =  read_from_file(nomeArquivo, &numPalavras);

        int contTotal= analisaTexto(words, numPalavras);
        
        imprimeCsv(contTotal);

        for (i = 0; i < numPalavras; i++)
        free (words[i]);
        free (words);

        printf("Tag Cloud por CSV concluído com sucesso! \n");
    }
    if (isHtml == true)
    {   
        nomeArquivo = argv[2];
        char **words =  read_from_file(nomeArquivo, &numPalavras);

        int contTotal = analisaTexto(words, numPalavras);

        for (i = 0; i < numPalavras; i++)
        free (words[i]);
        free (words);

        printf("\n\n");
    }   
}
