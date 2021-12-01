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
Usage: tagCloud [OPTION]... [FILE]...\n\
  or:  tagCloud [OPTION]... --files0-from=F\n\
");
      printf ("\
Print newline, word, and byte counts for each FILE, and a total line if\n\
more than one FILE is specified.  A word is a non-zero-length sequence of\n\
printable characters delimited by white space.\n\
");

      printf ("\
\n\
The options below may be used to select which counts are printed, always in\n\
the following order: newline, word, character, byte, maximum line length.\n\
  -c, --bytes            print the byte counts\n\
  -m, --chars            print the character counts\n\
  -l, --lines            print the newline counts\n\
");
      printf ("\
      --files0-from=F    read input from the files specified by\n\
                           NUL-terminated names in file F;\n\
                           If F is - then read names from standard input\n\
  -L, --max-line-length  print the maximum display width\n\
  -w, --words            print the word counts\n\
");
      printf ("--help     mostra esta ajuda e sai, \n");
      printf ("--version  informa a versão e sai.");
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
      printf ("Versão 1.0 \n Este é um software livre: você é livre para alterá-lo e redistribuí-lo. NÃO HÁ QUALQUER GARANTIA, na máxima extensão permitida em lei.\n Escrito por Matheus Pozzer Moraes, Diogo Almeida e Douglas Sobreira.");
    }
  exit (status);
}

int
main (int argc, char *argv[])
{
  bool ok;
  int optc;
  int sizeWord;
  size_t nfiles;
  char **files;
  char *files_from = NULL;
  struct fstatus *fstatus;

  /* Line buffer stdout to ensure lines are written atomically and immediately
     so that processes running in parallel do not intersperse their output.  */
  setvbuf (stdout, NULL, _IOLBF, 0);

  int ret;
  ret = strcmp(argv[1] , "--help");
  if (argc > 1){
       if (argc <= 2){
           printf("Palavra %s", argv[1]);
            if (strcmp(argv[1] , "--help") == 0)
            {
                optc = 0;
            }
            if (strcmp(argv[1] , "--version") == 0){
                optc = 1;
            }  
       }
  }
    
    printf("TESTEEE\n %s", argv[1]);
    printf("TESTEEE %d\n", optc);

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

      default:
        usage (1);
    }
}