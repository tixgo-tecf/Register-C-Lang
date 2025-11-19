#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <windows.h>

typedef struct
{
    char nomeAnimal[50];
    char nomeDono[50];
    int idade;
    float peso;
} Animal;

void limpaBuffer();
int tamanho(FILE *arq);
void cadastrar(FILE *arq);
void consultar(FILE *arq);
void listarTodos(FILE *arq);
void excluir(FILE *arq);

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");

    FILE *arq;

    arq = fopen("ProjetoFinal_C.dat", "r+b");
    if (arq == NULL)
    {
        arq = fopen("ProjetoFinal_C.dat", "w+b");
        if (arq == NULL)
        {
            printf("Erro ao criar arquivo!\n");
            return 1;
        }
    }

    int opcao;

    do
    {
        printf("\n==============================\n");
        printf("  PET SHOP - BANHO E TOSA\n");
        printf("==============================\n");
        printf("1 - Cadastrar Animal\n");
        printf("2 - Consultar por Código\n");
        printf("3 - Listar Todos\n");
        printf("4 - Excluir Registro\n");
        printf("5 - Mostrar quantidade\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpaBuffer();

        switch (opcao)
        {
        case 1:
            cadastrar(arq);
            break;
        case 2:
            consultar(arq);
            break;
        case 3:
            listarTodos(arq);
            break;
        case 4:
            excluir(arq);
            break;
        case 5:
            printf("Total de registros: %d\n", tamanho(arq));
            break;
        case 0:
            printf("Encerrando...\n");
            break;
        default:
            printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    fclose(arq);
    return 0;
}

void limpaBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int tamanho(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    long bytes = ftell(arq);
    if (bytes < 0)
    {
        return 0;
    }
    long count = bytes / (long)sizeof(Animal);
    return (int)count;
}

void cadastrar(FILE *arq)
{
    Animal a;

    printf("\n=== CADASTRAR ANIMAL ===\n");

    printf("Nome do animal: ");
    fgets(a.nomeAnimal, sizeof(a.nomeAnimal), stdin);
    a.nomeAnimal[strcspn(a.nomeAnimal, "\n")] = 0;

    printf("Nome do proprietário: ");
    fgets(a.nomeDono, sizeof(a.nomeDono), stdin);
    a.nomeDono[strcspn(a.nomeDono, "\n")] = 0;

    printf("Idade (anos): ");
    scanf("%d", &a.idade);
    limpaBuffer();

    printf("Peso (kg): ");
    scanf("%f", &a.peso);
    limpaBuffer();

    fseek(arq, 0, SEEK_END);
    fwrite(&a, sizeof(Animal), 1, arq);

    printf("Animal cadastrado com sucesso!\n");
}

void consultar(FILE *arq)
{
    int cod;
    Animal a;

    printf("\nInforme o código do registro: ");
    scanf("%d", &cod);
    limpaBuffer();

    int total = tamanho(arq);

    if (cod < 0 || cod >= total)
    {
        printf("Código inválido! Total registros: %d\n", total);
        return;
    }

    fseek(arq, cod *sizeof(Animal), SEEK_SET);
    fread(&a, sizeof(Animal), 1, arq);

    printf("\n=== DADOS DO ANIMAL [%d] ===\n", cod);
    printf("Animal: %s (Proprietário: %s)\n", a.nomeAnimal, a.nomeDono);
    printf("Idade:  %d anos\n", a.idade);
    printf("Peso:   %.2f kg\n", a.peso);
}

void listarTodos(FILE *arq)
{
    int total = tamanho(arq);

    if (total == 0)
    {
        printf("\nNenhum registro encontrado.\n");
        return;
    }

    Animal a;
    fseek(arq, 0, SEEK_SET);

    printf("\n=== LISTA DE ANIMAIS ===\n");

    for (int i = 0; i < total; i++)
    {
        fread(&a, sizeof(Animal), 1, arq);
        printf("[%d] Animal: %s - Dono: %s - Idade: %d - Peso: %.2fkg\n",
               i, a.nomeAnimal, a.nomeDono, a.idade, a.peso);
    }
}

void excluir(FILE *arq)
{
    int cod;
    printf("\nInforme o código do registro a excluir: ");
    scanf("%d", &cod);
    limpaBuffer();

    int total = tamanho(arq);

    if (cod < 0 || cod >= total)
    {
        printf("Código inválido!\n");
        return;
    }

    Animal *vet = (Animal *)malloc(total * sizeof(Animal));

    fseek(arq, 0, SEEK_SET);
    fread(vet, sizeof(Animal), total, arq);

    for (int i = cod; i < total - 1; i++)
    {
        vet[i] = vet[i + 1];
    }

    freopen("ProjetoFinal_C.dat", "w+b", arq);

    fwrite(vet, sizeof(Animal), total - 1, arq);

    free(vet);

    printf("Registro excluído com sucesso!\n");

}
