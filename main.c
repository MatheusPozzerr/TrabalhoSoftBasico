#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <sys/types.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include "error.h"
#include "stdbool.h"

#define program_name\
  _("tagCloud\n")

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

int
main (int argc, char *argv[])
{
  bool ok;
  char Linha[100];
  char* palavras[20000];
  bool isCsv;
  int numPalavras = 0;
  int i = 0;
  int optc;
  int sizeWord;
  size_t nfiles;
  char **files;
  char *files_from = NULL;
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

      default:
        usage (1);
    }

    if (isCsv == true)
    {   
        arq = fopen(argv[2], "rt");
        if (arq == NULL)
        return EXIT_FAILURE;
        while(fgets(Linha, sizeof Linha, arq) != NULL)
    {
        palavras[i] = strdup(Linha);

//         char *strdup(const char *src) {
//         char *dst = malloc(strlen (src) + 1);  // Space for length plus nul
//         if (dst == NULL) return NULL;          // No memory
//         strcpy(dst, src);                      // Copy the characters
//         return dst;                            // Return the new string
// }

        i++;

        numPalavras++;
    }

    int j = 0;

    for (j = 0; j < numPalavras; j++)
        printf("\n%s", palavras[j]); //Exibi as palavras que estao no vetor.

    printf("\n\n");

    fclose(arq);    
    }
    
}