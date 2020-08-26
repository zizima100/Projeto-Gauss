#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>

int linhas, colunas;
float **matriz;

void liberarMemoriaMatriz()
{
  free(matriz);
}

void insercaoMatrizManual()
{
  int quantidadeDeVariaveis;

  do
  {
    system("cls");
    printf("\n\tBem vindo ao solucionador de sistemas");
    printf("\n\n\tPor favor, insira o número de variáveis de seu sistema abaixo.\n");

    printf("\n\tNúmero de variáveis: ");
    scanf("%d", &quantidadeDeVariaveis);

  } while (quantidadeDeVariaveis < 2);

  linhas = quantidadeDeVariaveis;
  colunas = quantidadeDeVariaveis + 1;

  system("cls");
  printf("\n\tA matriz terá %d colunas e %d linhas.\n", colunas, linhas);

  matriz = (float **)malloc(linhas * sizeof(float *));
  for (int i = 0; i < linhas; i++)
  {
    printf("\n\tInsira os valores da linha %d.\n", i);

    matriz[i] = (float *)malloc(colunas * sizeof(float));
    for (int j = 0; j < colunas; j++)
    {
      printf("\tValor da coluna %d: ", j);
      scanf("%f", &matriz[i][j]);
    }
  }
}

void lerMatrizPorArquivo()
{
  FILE *arquivo = fopen("matriz.txt", "r");
  int n;

  if (arquivo == NULL)
  {
    insercaoMatrizManual();
  }
  else
  {
    printf("\n\tA matriz contida no arquivo é: \n");

    fscanf(arquivo, "%d", &n);

    linhas = n;
    colunas = n + 1;

    matriz = (float **)malloc(linhas * sizeof(float *)); //Cria o vetor que guarda vetores.

    for (int i = 0; i < linhas; i++)
    {
      matriz[i] = (float *)malloc(colunas * sizeof(float)); //Cria outro vetor dentro do primeiro.
      for (int j = 0; j < colunas; j++)
      {
        fscanf(arquivo, "%f", &matriz[i][j]);
      }
    }
    fclose(arquivo);
  }
}

void imprimirMatriz()
{
  for (int linha = 0; linha < linhas; linha++)
  {
    printf("\n\t");
    for (int coluna = 0; coluna < colunas; coluna++)
    {
      printf(" %.2f ", matriz[linha][coluna]);
    }
    printf("\n");
  }
}

void checarLinhasIguais()
{
  int linhaAtual = 0, quantidadeElementosIguais;

  for (int i = 0; i < linhas - 1; i++)
  {
    linhaAtual++;

    for (linhaAtual; linhaAtual < linhas; linhaAtual++)
    {
      quantidadeElementosIguais = 0;

      for (int j = 0; j < colunas; j++)
      {
        if (matriz[i][j] == matriz[linhaAtual][j])
          quantidadeElementosIguais++;
      }

      printf("\n\n\tComparou a  linha %d com a linha %d.", i, linhaAtual);

      //Termina o programa em caso de linhas iguais.
      if (quantidadeElementosIguais == colunas)
      {
        printf("\n\n\tA linha %d e a linha %d são iguais!", i, linhaAtual);
        printf("\n\tPor conta disso, é impossível resolver o sistema");
        printf("\n\n\tO programa será encerrado agora . . .");

        fflush(stdin);
        getchar();

        exit(0);
      }
    }
    linhaAtual = i + 1;
  }
}

//Realiza o processo de troca de linhas.
void trocaDeLinhas(int linhaTrocada, int linhaNova)
{
  float linhaTemporaria; //Variável temporária usada para o swap de linhas.

  for (int i = 0; i < colunas; i++)
  {
    linhaTemporaria = matriz[linhaTrocada][i];
    matriz[linhaTrocada][i] = matriz[linhaNova][i];
    matriz[linhaNova][i] = linhaTemporaria;
  }

  printf("\n\n\tTrocou linha %d pela linha %d\n", linhaTrocada, linhaNova);
  imprimirMatriz();
}

void checaZerosNaDiagonal()
{
  for (int linha = 0; linha < linhas; linha++)
  {
    int coluna = linha;

    if (matriz[linha][coluna] == 0)
    {
      printf("\n\n\tExiste 0 na diagonal da linha %d!", linha + 1);

      //Checa e efetua trocas possíveis.
      for (int i = 0; i < linhas; i++)
      {
        if (matriz[i][coluna] != 0 && matriz[linha][i] != 0)
        {
          printf("\n\t-Da pra trocar a %dª linha com a %dª!!", linha + 1, i + 1);
          trocaDeLinhas(linha, i);
          break;
        }
      }
    }

    if (matriz[linha][coluna] == 0)
    {
      printf("\n\n\tInfelizmente não tem como eliminar o 0 da diagonal.");
      printf("\n\tPor conta disso, é impossível resolver o sistema.");
      printf("\n\n\tO programa será encerrado . . .");

      fflush(stdin);
      getchar();

      exit(0);
    }
  }
}

// Transforma as diagonais em 1 e as colunas em 0 (Passo 7 ao 13)
void normalizarMatriz()
{
  float divisorLinha, multiplicadorLinha, *vetor = (float *)malloc(colunas * sizeof(float));

  for (int colunaAtual = 0; colunaAtual < colunas - 1; colunaAtual++)
  {
    for (int linhaAtual = 0; linhaAtual < linhas; linhaAtual++)
    {
      printf("\n\n\n\n MATRIZ \n\n\n");
      imprimirMatriz();
      printf("\n\n\n\n MATRIZ \n\n\n");

      // Diagonal da coluna atual
      if (matriz[colunaAtual][colunaAtual] != 1)
      {
        printf("DIAGONAL %d \n", colunaAtual);
        divisorLinha = matriz[colunaAtual][colunaAtual];
        printf("DIVISOR = %f \n", divisorLinha);
        for (int k = 0; k < colunas; k++)
        {
          matriz[colunaAtual][k] /= divisorLinha;
        }
      }

      // Se estiver apontando para uma diagonal ou o campo já estiver 0, continuo o for
      if (linhaAtual == colunaAtual || matriz[linhaAtual][colunaAtual] == 0)
      {
        printf("continuei\n");
        continue;
      }

      multiplicadorLinha = matriz[linhaAtual][colunaAtual] * -1;
      printf("\n\n MULTIPLICADOR %f \n", multiplicadorLinha);

      // Procurando linha que o valor na coluna atual é 1 (Possivel refatoração sem usar loop)
      for (int k = 0; k < linhas; k++)
      {
        if (matriz[k][colunaAtual] == 1)
          vetor = matriz[k];
      }

      // Multiplicando linha anterior para somar com a linha atual e zerar o campo apontado
      for (int k = 0; k < colunas; k++)
      {
        printf("vetor[%d] = %f \n", k, vetor[k]);
        float novoValor = vetor[k] * multiplicadorLinha;

        printf("multiplicado vetor[%d] = %f \n", k, novoValor);
        printf("matriz[%d][%d] = %f \n", linhaAtual, k, matriz[linhaAtual][k]);

        matriz[linhaAtual][k] += novoValor;
        printf("subtraida matriz[%d][%d] = %f \n", linhaAtual, k, matriz[linhaAtual][k]);
      }
    }
  }

  printf("\n\n\n\n MATRIZ \n\n\n");
  imprimirMatriz();
  free(vetor);
}

int main()
{
  setlocale(LC_ALL, "portuguese");
  system("cls");

  lerMatrizPorArquivo();

  imprimirMatriz();

  checarLinhasIguais();

  checaZerosNaDiagonal();

  normalizarMatriz();

  liberarMemoriaMatriz();

  fflush(stdin);
  getchar();
  return (0);
}
