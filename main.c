#include <stdio.h>
#include <assert.h>
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


// Métodos de operação
enum operating_mode
  {
    undefined_mode,

    //Opção que usa bytes como delimitador.
    byte_mode,

    //Opção que usa campos como delimitador.
    field_mode
  };

//Método referente ao menu de ajuda do programa
void
usage (int status)
{
  if (status != 0)
    printf("Tente tagCloud --help para mais informações. \n");
  else
    {
      printf ("\
Uso: [OPTION]... [FILE]...\n\
Ou:  [OPTION]... --files0-from=F\n\
");
      printf ("\
O programa tem como objetivo emitir as palavras mais utilizadas, podendo ser realizada saídas em formato CSV e HTML\n\
");

      printf ("\
\n\
As opções possíveis estão descritas abaixo:\n\
-c                          Saída com as palavras mais utilizadas em formato CSV\n\
-h                          Saída com as palavras mais utilizadas em formato HTML\n\
-h -color                   Saída com as palavras mais utilizadas em formato HTML e com Cor\n\
-c -r numero(EXEMPLO: 10)   Saída com as palavras mais utilizadas em formato CSV e com alcance de palavras mais utilizadas\n\
");
      printf ("--help                 mostra esta ajuda e sai, \n");
      printf ("--version              informa a versão e sai.\n");
    }
  exit (status);
}

//Método que mostra a versão do programa.
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

// Método que lê o arquivo.
char** read_from_file (char* nomeArquivo, int *index)
{ 
    char **palavras = malloc (Num_MAX * sizeof *palavras);
    char buf[Caracter_MAX] = {0};
    FILE* arq = fopen (nomeArquivo, "r");

    if (arq == NULL){
        fprintf (stderr, "Can't open file\nUtilize --help para verificar os possiveis comandos\n");
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

} palavrasTexto[10001];


// método para adicionar as palavras
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

// Método que analisa a repetição das palavras
int analisaTexto (char** palavras, int numFrases)
{
    int j = 0;
    int contador = 0;
    char * pch;
        for (j = 0; j < numFrases; j++){
            pch = strtok (palavras[j]," ;,.-");
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

// Método que imprime as palavras em CSV
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

//Método que imprime as palavras mais utilizadas
void imprimeCsvLimitePalavras (int contador)
{
    FILE* arqCsv = fopen ("PalavrasUtilizadas.csv", "w");

    fputs("Palavra, Recorrência \r\n", arqCsv);
    for (size_t i = 0; i < contador; i++)
    {
        fprintf(arqCsv, "%s, %i\n", palavrasTexto[i].palavra, palavrasTexto[i].qtd);
    }
    fclose(arqCsv);
}

//Método que imprime em html sem cor.
void imprimeHtmlSemCor(int contador){
    FILE* arqHTML = fopen ("PalavrasUtilizadasSemCor.html", "w");
    if (contador < 40)
    {
        if (contador == 0)
        {
            printf("Texto sem palavras");
        }
        if (contador == 1)
        {
            fprintf(arqHTML, "<html> \n");
            fprintf(arqHTML, "<title> Palavras e Recorrencias </title> \n");
            fprintf(arqHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"styleNoColor.css\">\n");
            fprintf(arqHTML, "</head>\n");
            fprintf(arqHTML, "<body> \n");
            fprintf(arqHTML, "<div class=\"words\"> \n");
            fprintf(arqHTML, "<span class=\"text5\">%s</span> \n", palavrasTexto[0].palavra);
            fprintf(arqHTML, "</div>");
            fprintf(arqHTML, "</body>");
            fprintf(arqHTML, "</html>");       
        }
        if (contador > 1)
        {
            fprintf(arqHTML, "<html> \n");
            fprintf(arqHTML, "<title> Palavras e Recorrencias </title> \n");
            fprintf(arqHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"styleNoColor.css\">\n");
            fprintf(arqHTML, "</head>\n");
            fprintf(arqHTML, "<body> \n");
            fprintf(arqHTML, "<div class=\"words\"> \n");
            for (size_t i = 1; i <= contador/2; i++)
            {
                fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[i].palavra);
            }
            fprintf(arqHTML, "<span class=\"text5\">%s</span> \n", palavrasTexto[0].palavra);
            for (size_t i = contador/2+1; i < contador; i++)
            {
                fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[i].palavra);
            }
            fprintf(arqHTML, "</div>");
            fprintf(arqHTML, "</body>");
            fprintf(arqHTML, "</html>"); 
        }
    }
    else{
        fprintf(arqHTML, "<html> \n");
        fprintf(arqHTML, "<title> Palavras e Recorrencias </title> \n");
        fprintf(arqHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"styleNoColor.css\">\n");
        fprintf(arqHTML, "</head>\n");
        fprintf(arqHTML, "<body> \n");
        fprintf(arqHTML, "<div class=\"words\"> \n");
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[15].palavra);
        fprintf(arqHTML, "<span class=\"text2\">%s</span> \n", palavrasTexto[12].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[16].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[17].palavra);
        fprintf(arqHTML, "<span class=\"text5\">%s</span> \n", palavrasTexto[0].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[18].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[19].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[26].palavra);
        fprintf(arqHTML, "<span class=\"text3\">%s</span> \n", palavrasTexto[10].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[27].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[28].palavra);
        fprintf(arqHTML, "<span class=\"text2\">%s</span> \n", palavrasTexto[14].palavra);
        fprintf(arqHTML, "<span class=\"text2\">%s</span> \n", palavrasTexto[10].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[29].palavra);
        fprintf(arqHTML, "<span class=\"text4\">%s</span> \n", palavrasTexto[4].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[30].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[31].palavra);
        fprintf(arqHTML, "<span class=\"text5\">%s</span> \n", palavrasTexto[1].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[32].palavra);
        fprintf(arqHTML, "<span class=\"text4\">%s</span> \n", palavrasTexto[5].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[33].palavra);
        fprintf(arqHTML, "<span class=\"text5\">%s</span> \n", palavrasTexto[2].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[34].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[35].palavra);
        fprintf(arqHTML, "<span class=\"text4\">%s</span> \n", palavrasTexto[6].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[36].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[37].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[38].palavra);
        fprintf(arqHTML, "<span class=\"text3\">%s</span> \n", palavrasTexto[7].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[39].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[40].palavra);
        fprintf(arqHTML, "<span class=\"text3\">%s</span> \n", palavrasTexto[8].palavra);
        fprintf(arqHTML, "<span class=\"text3\">%s</span> \n", palavrasTexto[9].palavra);
        fprintf(arqHTML, "<span class=\"text2\">%s</span> \n", palavrasTexto[11].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[21].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[22].palavra);
        fprintf(arqHTML, "<span class=\"text4\">%s</span> \n", palavrasTexto[3].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[20].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[23].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[24].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[25].palavra);
        fprintf(arqHTML, "<span class=\"text2\">%s</span> \n", palavrasTexto[13].palavra);
    }



    fclose(arqHTML);
}

//Método que imprime em html com cor
void imprimeHtmlComCor(int contador){
    FILE* arqHTML = fopen ("PalavrasUtilizadas.html", "w");
    if (contador < 40)
    {
        if (contador == 0)
        {
            printf("Texto sem palavras");
        }
        if (contador == 1)
        {
            fprintf(arqHTML, "<html> \n");
            fprintf(arqHTML, "<title> Palavras e Recorrencias </title> \n");
            fprintf(arqHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n");
            fprintf(arqHTML, "</head>\n");
            fprintf(arqHTML, "<body> \n");
            fprintf(arqHTML, "<div class=\"words\"> \n");
            fprintf(arqHTML, "<span class=\"text5\">%s</span> \n", palavrasTexto[0].palavra);
            fprintf(arqHTML, "</div>");
            fprintf(arqHTML, "</body>");
            fprintf(arqHTML, "</html>");       
        }
        if (contador > 1)
        {
            fprintf(arqHTML, "<html> \n");
            fprintf(arqHTML, "<title> Palavras e Recorrencias </title> \n");
            fprintf(arqHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n");
            fprintf(arqHTML, "</head>\n");
            fprintf(arqHTML, "<body> \n");
            fprintf(arqHTML, "<div class=\"words\"> \n");
            for (size_t i = 1; i <= contador/2; i++)
            {
                fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[i].palavra);
            }
            fprintf(arqHTML, "<span class=\"text5\">%s</span> \n", palavrasTexto[0].palavra);
            for (size_t i = contador/2+1; i < contador; i++)
            {
                fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[i].palavra);
            }
            fprintf(arqHTML, "</div>");
            fprintf(arqHTML, "</body>");
            fprintf(arqHTML, "</html>"); 
        }
    }
    else{
        fprintf(arqHTML, "<html> \n");
        fprintf(arqHTML, "<title> Palavras e Recorrencias </title> \n");
        fprintf(arqHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n");
        fprintf(arqHTML, "</head>\n");
        fprintf(arqHTML, "<body> \n");
        fprintf(arqHTML, "<div class=\"words\"> \n");
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[15].palavra);
        fprintf(arqHTML, "<span class=\"text2\">%s</span> \n", palavrasTexto[12].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[16].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[17].palavra);
        fprintf(arqHTML, "<span class=\"text5\">%s</span> \n", palavrasTexto[0].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[18].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[19].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[26].palavra);
        fprintf(arqHTML, "<span class=\"text3\">%s</span> \n", palavrasTexto[10].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[27].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[28].palavra);
        fprintf(arqHTML, "<span class=\"text2\">%s</span> \n", palavrasTexto[14].palavra);
        fprintf(arqHTML, "<span class=\"text2\">%s</span> \n", palavrasTexto[10].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[29].palavra);
        fprintf(arqHTML, "<span class=\"text4\">%s</span> \n", palavrasTexto[4].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[30].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[31].palavra);
        fprintf(arqHTML, "<span class=\"text5\">%s</span> \n", palavrasTexto[1].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[32].palavra);
        fprintf(arqHTML, "<span class=\"text4\">%s</span> \n", palavrasTexto[5].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[33].palavra);
        fprintf(arqHTML, "<span class=\"text5\">%s</span> \n", palavrasTexto[2].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[34].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[35].palavra);
        fprintf(arqHTML, "<span class=\"text4\">%s</span> \n", palavrasTexto[6].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[36].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[37].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[38].palavra);
        fprintf(arqHTML, "<span class=\"text3\">%s</span> \n", palavrasTexto[7].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[39].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[40].palavra);
        fprintf(arqHTML, "<span class=\"text3\">%s</span> \n", palavrasTexto[8].palavra);
        fprintf(arqHTML, "<span class=\"text3\">%s</span> \n", palavrasTexto[9].palavra);
        fprintf(arqHTML, "<span class=\"text2\">%s</span> \n", palavrasTexto[11].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[21].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[22].palavra);
        fprintf(arqHTML, "<span class=\"text4\">%s</span> \n", palavrasTexto[3].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[20].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[23].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[24].palavra);
        fprintf(arqHTML, "<span class=\"text1\">%s</span> \n", palavrasTexto[25].palavra);
        fprintf(arqHTML, "<span class=\"text2\">%s</span> \n", palavrasTexto[13].palavra);
        fprintf(arqHTML, "</div>");
        fprintf(arqHTML, "</body>");
        fprintf(arqHTML, "</html>");
    }



    fclose(arqHTML);
}

// Método que ordena as palavras
void ordenaPalavrasUtilizacao(int contador){
    int i, j, aux;
    if (contador == 0)
    {
        return;
    }
    for( i=0; i<contador; i++ ){
        for( j=i; j<contador; j++ ){
            if( palavrasTexto[i].qtd < palavrasTexto[j].qtd ){
                 palavrasTexto[10001] = palavrasTexto[i];
                 palavrasTexto[i] = palavrasTexto[j];
                 palavrasTexto[j] = palavrasTexto[10001];
            }
        }
     }
}

// Método principal
int
main (int argc, char *argv[])
{
  bool ok;
  char Linha[100];
  char* palavras[20000];
  bool isCsv;
  bool isHtml;
  bool isHtmlColor;
  bool isCsvRange;
  int i = 0;
  int optc;
  int sizeWord;
  int numPalavras;
  size_t nfiles;
  char* nomeArquivo;
  FILE *arq;
  struct fstatus *fstatus;

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
            if (strcmp(argv[1] , "-h") == 0 && strcmp(argv[2] , "-color") == 0){
            optc = 4;
            }
            if (strcmp(argv[1] , "-c") == 0 && strcmp(argv[2] , "-r") == 0 && argc >= 4){
            optc = 5;
            }            
        }       
  }
    

    switch (optc)
    {
      case 0: 
      usage(0);
      break;

      case 1: version(0);
      break;

      case 2: isCsv = true;
      break;

      case 3: isHtml = true;
      break;

      case 4: isHtmlColor = true;
      break;

      case 5: isCsvRange = true;
      break;  

      default:
        usage (1);
    }

    if (isCsv == true)
    {   
        nomeArquivo = argv[2];
        char **words =  read_from_file(nomeArquivo, &numPalavras);

        int contTotal= analisaTexto(words, numPalavras);
        
        ordenaPalavrasUtilizacao(contTotal);
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

        ordenaPalavrasUtilizacao(contTotal);
        
        imprimeHtmlSemCor(contTotal);

        for (i = 0; i < numPalavras; i++)
        free (words[i]);
        free (words);

        printf("Tag Cloud por Html concluído com sucesso!\n");
    }
        if (isHtmlColor == true)
    {   
        nomeArquivo = argv[3];
        char **words =  read_from_file(nomeArquivo, &numPalavras);

        int contTotal = analisaTexto(words, numPalavras);

        ordenaPalavrasUtilizacao(contTotal);
        
        imprimeHtmlComCor(contTotal);

        for (i = 0; i < numPalavras; i++)
        free (words[i]);
        free (words);

        printf("Tag Cloud por Html com cor concluído com sucesso!\n");
    }
        if (isCsvRange == true)
    {   
        nomeArquivo = argv[4];
        char **words =  read_from_file(nomeArquivo, &numPalavras);

        int contTotal= analisaTexto(words, numPalavras);
        int range = atoi(argv[3]);
        ordenaPalavrasUtilizacao(contTotal);
        if (range < contTotal)
        {
            contTotal = range;
        }
        imprimeCsvLimitePalavras(contTotal);

        for (i = 0; i < numPalavras; i++)
        free (words[i]);
        free (words);

        printf("Tag Cloud por CSV com alcance de palavras concluído com sucesso! \n");
    }
        
}
