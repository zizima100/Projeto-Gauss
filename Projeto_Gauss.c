#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int linhas, colunas;
float **matriz;

//Libera a memória do computador.
void liberarMemoria()
{
  free(matriz);
}

//Tenta ler a matriz por um arquivo. Se não conseguir, ativa o modo manual.
void lerArquivo()
{
  FILE *ARQ = fopen("matriz.txt", "r");
	int n;
	int i, j;

	if(ARQ == NULL)
  {
		insercaoManual();
	}
  else
  {
    printf("\n\tA matriz contida no arquivo é: \n");

    fscanf(ARQ, "%d", &n);
    
    linhas = n;
    colunas = n + 1;

    matriz = (float**) malloc(linhas * sizeof(float*));//Cria o vetor que guarda vetores.

    for (i = 0; i < linhas; i++)
    {
      matriz[i] = (float*) malloc(colunas * sizeof(float));//Cria outro vetor dentro do primeiro.
      for (j = 0; j < colunas; j++)
      {
        fscanf(ARQ, "%f", &matriz[i][j]);
      }
    }
    fclose(ARQ); 
  }
}

//Define o tamanho e insere manualmente os valores na matriz dinâmica.
void insercaoManual()
{
  int nVariaveis;

  do
  {
    system("clear");
    printf("\n\tBem vindo ao solucionador de sistemas");
    printf("\n\n\tPor favor, insira o número de variáveis de seu sistema abaixo.\n");

    printf("\n\tNúmero de variáveis: ");
    scanf("%d", &nVariaveis);

  } while(nVariaveis < 2);

  //Define o número de linhas e colunas.
  linhas = nVariaveis;
  colunas = nVariaveis + 1;

  system("clear");
  printf("\n\tA matriz terá %d colunas e %d linhas.\n", colunas, linhas);

  matriz = (float**) malloc(linhas * sizeof(float*));
  for (int i = 0; i < linhas; i++)
  {
    printf("\n\tInsira os valores da linha %d.\n", i);

		matriz[i] = (float*) malloc(colunas * sizeof(float));
		for (int j = 0; j < colunas; j++)
    {
      printf("\tValor da coluna %d: ", j);
      scanf("%f", &matriz[i][j]);
		}
	}
}

//Imprime a matriz.
void printMatriz()
{
  for(int linha = 0; linha < linhas; linha++)
  {
    printf("\n\t");
    for(int coluna = 0; coluna < colunas; coluna++)
    {
      printf(" %.2f ", matriz[linha][coluna]);
    }
    printf("\n");
  }
}

//Checa se existem linhas iguais na matriz.
void checkLinhasIguais()
{
  int iComparada = 1, qntElementosIguais;

  printf("\nRodou o check!");

  for(int i = 0; i < linhas - 1; i++)//Compara 0 com 1,2; 1 com 2 e n compara a 2;
  {
    printf("Rodou o primeiro for!");
    qntElementosIguais = 0;

    for(int j = 0; j < colunas; j++)
    {
      printf("Rodou o segundo for!");
      // if(matriz[i][j] == matriz[i + iComparada][j])
      //   qntElementosIguais++;
    }
    
    printf("\n\nComparou a  linha %d com a linha %d.", i, i + iComparada);

    if(qntElementosIguais == colunas)
    {
      printf("\n\nExistem linhas iguais!");
      printf("\nPor conta disso, é impossível resolver o sistema ;(");
      printf("\nO programa será encerrado agora . . .\n");
      fflush(stdin);
      getchar();
      exit(0);
    }

    if(i + iComparada < linhas)
      iComparada++;
  }
}

//Realiza o processo de troca de linhas.
void trocaDeLinhas(int linhaRecebe, int linhaVai)
{
  float temp; //Variável temporária usada para o swap de linhas.

  for(int i = 0; i < colunas; i++)
  {
    temp = matriz[linhaRecebe][i];
    matriz[linhaRecebe][i] = matriz[linhaVai][i];
    matriz[linhaVai][i] = temp;
  }

  printf("\n\n\tTrocou linha %d pela linha %d\n", linhaRecebe, linhaVai);
  printMatriz();
}

//Checa se existem zeros na diagonal da matriz.
void checkZeroDiagonal()
{
  for(int linha = 0; linha < linhas; linha++)
  {
    int coluna = linha;

    //Checa se existem 0s na diagonal.
    if(matriz[linha][coluna] == 0)
    {
      printf("\n\tExiste 0 na diagonal da linha %d!", linha + 1);

      //Checa e efetua trocas possíveis.
      for(int i = 0; i < linhas; i++)
      {
        if(matriz[i][coluna] != 0 && matriz[linha][i] != 0)
        {
          printf("\n\t-Da pra trocar a %dª linha com a %dª!!", linha + 1, i + 1);
          trocaDeLinhas(linha, i);
          break;
        }
      }
    }

    //Checa se foi feita mudança. Se não foi, perdeu playboy.
    if(matriz[linha][coluna] == 0)
    {
      printf("\n\n\tInfelizmente não tem como eliminar o 0 da diagonal.");
      printf("\n\tPor conta disso, é impossível resolver o sistema.");
      printf("\n\n\tO programa será encerrado . . .");
      exit(0);
    }
  }
}

int main()
{
  setlocale(LC_ALL, "portuguese");
  system("clear");

  int nEqua;

  lerArquivo();

  printMatriz();

  //Está com erro. Julio Corrigindo.
  //checkLinhasIguais();

  checkZeroDiagonal();

  getchar();getchar();
  return(0);
}
