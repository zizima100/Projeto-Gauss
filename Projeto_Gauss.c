#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#define INT_MAX 2147483647;

int linhas, colunas;
float **matriz;
int *vetor_ordem;

void liberarMemoriaMatriz()
{
  free(vetor_ordem);
  for (int i = 0; i < linhas; i++)
  {
    free(matriz[i]);
  }
  free(matriz);
}

void fecharPrograma()
{
  fflush(stdin);
  getchar();
  liberarMemoriaMatriz();
  exit(0);
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
  int quantidadeElementosIguais;
  float *resultadoDivisoes = (float *)malloc(linhas * sizeof(float));

  for (int i = 0; i < linhas - 1; i++)
  {
    for (int j = i + 1; j < linhas; j++)
    {
      quantidadeElementosIguais = 0;

      for (int k = 0; k < colunas - 1; k++)
      {
        if (matriz[j][k] != 0)
        {
          resultadoDivisoes[k] = (float)matriz[i][k] / (float)matriz[j][k];
        }
        else
        {
          resultadoDivisoes[k] = INT_MAX;
        }

        if (k > 0)
        {
          if (resultadoDivisoes[k] == resultadoDivisoes[k - 1])
          {
            quantidadeElementosIguais++;
          }
        }
      }

      if (quantidadeElementosIguais == colunas - 2)
      {
        printf("\n\n\t Os resultados das divisões da linha %d e a linha %d são iguais!", i, i + 1);
        printf("\n\tPor conta disso, é impossível resolver o sistema");
        printf("\n\n\tO programa será encerrado agora . . .");
        free(resultadoDivisoes);
        fecharPrograma();
      }
    }
  }

  free(resultadoDivisoes);
}

// Transforma as diagonais em 1 e as colunas em 0 (Passo 7 ao 13)
void normalizarMatriz()
{
  float divisorLinha, multiplicadorLinha, *vetor = (float *)malloc(colunas * sizeof(float));

  for (int colunaAtual = 0; colunaAtual < colunas - 1; colunaAtual++)
  {
    for (int linhaAtual = 0; linhaAtual < linhas; linhaAtual++)
    {
      // Diagonal da coluna atual
      if (matriz[colunaAtual][colunaAtual] != 1)
      {
        divisorLinha = matriz[colunaAtual][colunaAtual];
        for (int k = 0; k < colunas; k++)
        {
          matriz[colunaAtual][k] /= divisorLinha;
        }
      }

      // Se estiver apontando para uma diagonal ou o campo já estiver 0, continuo o for
      if (linhaAtual == colunaAtual || matriz[linhaAtual][colunaAtual] == 0)
      {
        continue;
      }

      multiplicadorLinha = matriz[linhaAtual][colunaAtual] * -1;

      // Procurando linha que o valor na coluna atual é 1 (Possivel refatoração sem usar loop)
      for (int k = 0; k < linhas; k++)
      {
        if (matriz[k][colunaAtual] == 1)
        {
          vetor = matriz[k];
        }
      }

      // Multiplicando linha anterior para somar com a linha atual e zerar o campo apontado
      for (int k = 0; k < colunas; k++)
      {
        float novoValor = vetor[k] * multiplicadorLinha;

        matriz[linhaAtual][k] += novoValor;
      }
    }
  }

  free(vetor);
}

// verificando se o vetor_ordem � o �ltimo caso
int ultimoCasoOrdem()
{
  int j;
  int cont_iguais = 0;

  j = 0;
  for (int i = linhas - 1; i >= 0; i--)
  {
    if (vetor_ordem[j] == i)
      cont_iguais++;
    j++;
  }

  if (cont_iguais == linhas)
    return 1;
  else
    return 0;
}

void permutarMatriz()
{
  int *vetor_naoexistentes = (int *)malloc(linhas * sizeof(int));
  int cont_diferentes;
  int cont;

  int i, j, k;
  int tem_zero;
  int evoluiu;

  int indice;

  vetor_ordem = (int *)malloc(linhas * sizeof(int));

  for (i = 0; i < linhas; i++)
  {
    vetor_ordem[i] = i;
  }

  do
  {
    tem_zero = 0;

    for (i = 0; i < linhas; i++)
    {
      if (matriz[vetor_ordem[i]][i] == 0)
      {
        tem_zero = 1;
        break;
      }
    }

    if (tem_zero == 1)
    {
      for (i = 0; i < linhas; i++)
      {
        vetor_naoexistentes[i] = -1;
      }

      // preencher com o maior valido e existente
      evoluiu = 0;
      for (i = linhas - 1; i >= 0; i--)
      {
        // for para percorrer indices 0 ate qtdeEquacoes
        for (j = 0; j < linhas; j++)
        {
          if (j <= vetor_ordem[i])
          {
            continue;
          }

          cont_diferentes = 0;
          cont = 0;

          for (k = i - 1; k >= 0; k--)
          {
            if (vetor_ordem[k] != j)
            {
              cont_diferentes++;
            }
            cont++;
          }

          if ((cont_diferentes == cont) && (j > vetor_ordem[i]))
          {
            vetor_ordem[i] = j;
            indice = i;
            evoluiu = 1;
            break;
          }
        }

        if (evoluiu == 1)
        {
          break;
        }
      }

      k = 0;
      for (i = 0; i < linhas; i++)
      {
        cont_diferentes = 0;
        for (j = 0; j <= indice; j++)
        {
          if (i != vetor_ordem[j])
          {
            cont_diferentes++;
          }
        }

        if (cont_diferentes == (indice + 1))
        {
          vetor_naoexistentes[k] = i;
          k++;
        }
      }

      j = 0;
      for (i = indice + 1; i < linhas; i++)
      {
        if (vetor_naoexistentes[j] == -1)
        {
          break;
        }

        vetor_ordem[i] = vetor_naoexistentes[j];
        j++;
      }
    }
  } while ((tem_zero == 1) && (ultimoCasoOrdem() == 0));

  // substituindo matriz original
  if (tem_zero == 0)
  {
    // copiando matriz original
    float **matriz_copia = (float **)malloc(linhas * sizeof(float *));
    for (i = 0; i < linhas; i++)
    {
      matriz_copia[i] = (float *)malloc(colunas * sizeof(float));
      for (j = 0; j < colunas; j++)
      {
        matriz_copia[i][j] = matriz[i][j];
      }
    }

    for (i = 0; i < linhas; i++)
    {
      for (j = 0; j < colunas; j++)
      {
        matriz[i][j] = matriz_copia[vetor_ordem[i]][j];
      }
    }

    for (i = 0; i < linhas; i++)
    {
      free(matriz_copia[i]);
    }

    free(matriz_copia);
    free(vetor_naoexistentes);
  }

  if (ultimoCasoOrdem() == 1)
  {
    printf("\n\nN�o tem como tirar os zeros da diagonal principal. Sistema imposs�vel\n\n");
    fecharPrograma();
  }
}

int main()
{
  setlocale(LC_ALL, "portuguese");
  system("cls");

  lerMatrizPorArquivo();

  imprimirMatriz();

  printf("\n\tPressione enter para continuar . . .");
  fflush(stdin);
  getchar();
  system("cls");

  checarLinhasIguais();

  permutarMatriz();

  normalizarMatriz();

  printf("\n\tA solução do sistema é:\n");
  imprimirMatriz();

  fecharPrograma();
}
