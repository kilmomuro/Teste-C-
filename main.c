#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 40
#define MAX_INVENTORY 99

typedef enum {
    RACE_HUMANO = 1,
    RACE_ELFO,
    RACE_ANDAO
} Race;

typedef struct {
    int sementes;
    int colheitas;
    int peixes;
    int minerio;
} Inventario;

typedef struct {
    char nome[MAX_NAME];
    Race raca;
    int dia;
    int ouro;
    int energia;
    int hp;
    int nivelFazenda;
    int nivelPesca;
    int nivelMineracao;
    Inventario inv;
} Jogador;

static void limparEntrada(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static int randomRange(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

static const char *nomeRaca(Race r) {
    switch (r) {
        case RACE_HUMANO: return "Humano";
        case RACE_ELFO: return "Elfo";
        case RACE_ANDAO: return "Anão";
        default: return "Desconhecida";
    }
}

static void configurarRaca(Jogador *j) {
    switch (j->raca) {
        case RACE_HUMANO:
            j->energia = 100;
            j->hp = 100;
            j->ouro = 80;
            break;
        case RACE_ELFO:
            j->energia = 120;
            j->hp = 90;
            j->ouro = 70;
            j->nivelPesca = 1;
            break;
        case RACE_ANDAO:
            j->energia = 90;
            j->hp = 120;
            j->ouro = 90;
            j->nivelMineracao = 1;
            break;
        default:
            j->energia = 100;
            j->hp = 100;
            j->ouro = 80;
            break;
    }
}

static void cabecalho(const Jogador *j) {
    printf("\n================ DIA %d ================\n", j->dia);
    printf("%s (%s) | Ouro: %d | Energia: %d | HP: %d\n",
           j->nome, nomeRaca(j->raca), j->ouro, j->energia, j->hp);
    printf("Níveis -> Fazenda:%d Pesca:%d Mineração:%d\n",
           j->nivelFazenda, j->nivelPesca, j->nivelMineracao);
    printf("Inventário -> Sementes:%d Colheitas:%d Peixes:%d Minério:%d\n",
           j->inv.sementes, j->inv.colheitas, j->inv.peixes, j->inv.minerio);
}

static void plantar(Jogador *j) {
    if (j->energia < 10) {
        puts("Você está cansado demais para trabalhar na fazenda.");
        return;
    }
    if (j->inv.sementes <= 0) {
        puts("Sem sementes. Compre no mercado da vila.");
        return;
    }

    int colhido = randomRange(1, 3) + j->nivelFazenda;
    if (colhido > j->inv.sementes) {
        colhido = j->inv.sementes;
    }

    j->inv.sementes -= colhido;
    j->inv.colheitas += colhido;
    j->energia -= 10;

    if (randomRange(1, 100) <= 40) {
        j->nivelFazenda++;
        puts("Sua habilidade de fazenda aumentou!");
    }

    printf("Você cultivou e colheu %d alimentos do campo medieval.\n", colhido);
}

static void pescar(Jogador *j) {
    if (j->energia < 12) {
        puts("Energia insuficiente para pescar no lago real.");
        return;
    }

    int chanceBase = 55 + (j->nivelPesca * 5);
    int rolagem = randomRange(1, 100);

    if (rolagem <= chanceBase) {
        int captura = randomRange(1, 2) + j->nivelPesca;
        j->inv.peixes += captura;
        printf("Você pescou %d peixe(s).\n", captura);
        if (randomRange(1, 100) <= 35) {
            j->nivelPesca++;
            puts("Você melhorou sua técnica de pesca!");
        }
    } else {
        puts("Nada mordeu o anzol hoje.");
    }

    j->energia -= 12;
}

static void minerar(Jogador *j) {
    if (j->energia < 15) {
        puts("Você está sem forças para entrar na mina.");
        return;
    }

    int chanceMonstro = randomRange(1, 100);
    if (chanceMonstro <= 20) {
        int dano = randomRange(5, 15);
        j->hp -= dano;
        printf("Goblin emboscou você na mina! Dano recebido: %d\n", dano);
        if (j->hp <= 0) {
            puts("Você desmaiou e perdeu parte do minério.");
            j->hp = 50;
            j->inv.minerio /= 2;
        }
    }

    int extraido = randomRange(1, 3) + j->nivelMineracao;
    j->inv.minerio += extraido;
    j->energia -= 15;

    if (randomRange(1, 100) <= 45) {
        j->nivelMineracao++;
        puts("Seu nível de mineração aumentou!");
    }

    printf("Você extraiu %d minério(s) de ferro.\n", extraido);
}

static void mercado(Jogador *j) {
    int escolha;
    puts("\n--- Mercado da Vila ---");
    puts("1) Comprar sementes (5 ouro cada)");
    puts("2) Vender colheitas (12 ouro cada)");
    puts("3) Vender peixes (15 ouro cada)");
    puts("4) Vender minério (18 ouro cada)");
    puts("5) Sair");
    printf("Escolha: ");

    if (scanf("%d", &escolha) != 1) {
        limparEntrada();
        puts("Opção inválida.");
        return;
    }

    int qtd;
    switch (escolha) {
        case 1:
            printf("Quantidade de sementes: ");
            if (scanf("%d", &qtd) != 1 || qtd < 1) {
                limparEntrada();
                puts("Quantidade inválida.");
                return;
            }
            if (qtd * 5 > j->ouro) {
                puts("Ouro insuficiente.");
                return;
            }
            j->ouro -= qtd * 5;
            j->inv.sementes += qtd;
            puts("Compra realizada com sucesso!");
            break;
        case 2:
            printf("Quantidade de colheitas para vender: ");
            if (scanf("%d", &qtd) != 1 || qtd < 1 || qtd > j->inv.colheitas) {
                limparEntrada();
                puts("Quantidade inválida.");
                return;
            }
            j->inv.colheitas -= qtd;
            j->ouro += qtd * 12;
            puts("Venda de colheitas concluída.");
            break;
        case 3:
            printf("Quantidade de peixes para vender: ");
            if (scanf("%d", &qtd) != 1 || qtd < 1 || qtd > j->inv.peixes) {
                limparEntrada();
                puts("Quantidade inválida.");
                return;
            }
            j->inv.peixes -= qtd;
            j->ouro += qtd * 15;
            puts("Venda de peixes concluída.");
            break;
        case 4:
            printf("Quantidade de minério para vender: ");
            if (scanf("%d", &qtd) != 1 || qtd < 1 || qtd > j->inv.minerio) {
                limparEntrada();
                puts("Quantidade inválida.");
                return;
            }
            j->inv.minerio -= qtd;
            j->ouro += qtd * 18;
            puts("Venda de minério concluída.");
            break;
        case 5:
            puts("Você deixa o mercado e volta para sua propriedade.");
            break;
        default:
            puts("Opção inválida.");
            break;
    }
}

static void dormir(Jogador *j) {
    j->dia++;
    j->energia = 100 + (j->raca == RACE_ELFO ? 20 : 0);
    j->hp += 10;
    if (j->hp > 120) j->hp = 120;
    puts("Você descansou na taverna e iniciou um novo dia.");
}

static int venceu(const Jogador *j) {
    return j->ouro >= 500 && j->nivelFazenda >= 3 && j->nivelPesca >= 3 && j->nivelMineracao >= 3;
}

int main(void) {
    srand((unsigned int)time(NULL));

    Jogador j = {0};
    j.dia = 1;
    j.inv.sementes = 5;

    puts("=== Reinos de Vale Verde ===");
    puts("Simulador medieval inspirado em farming RPG.");

    printf("Nome do herói: ");
    if (fgets(j.nome, sizeof(j.nome), stdin) == NULL) {
        strcpy(j.nome, "Aventureiro");
    }
    j.nome[strcspn(j.nome, "\n")] = '\0';
    if (strlen(j.nome) == 0) {
        strcpy(j.nome, "Aventureiro");
    }

    puts("Escolha sua raça:");
    puts("1) Humano (equilibrado)");
    puts("2) Elfo (+energia, bônus em pesca)");
    puts("3) Anão (+vida, bônus em mineração)");
    printf("> ");
    int escolhaRaca;
    if (scanf("%d", &escolhaRaca) != 1 || escolhaRaca < 1 || escolhaRaca > 3) {
        escolhaRaca = 1;
    }
    limparEntrada();

    j.raca = (Race)escolhaRaca;
    configurarRaca(&j);

    int opcao = 0;
    while (opcao != 6) {
        cabecalho(&j);
        puts("\nAções do dia:");
        puts("1) Plantar e colher");
        puts("2) Pescar");
        puts("3) Minerar");
        puts("4) Mercado");
        puts("5) Dormir");
        puts("6) Encerrar jogo");
        printf("> ");

        if (scanf("%d", &opcao) != 1) {
            limparEntrada();
            puts("Entrada inválida.");
            continue;
        }
        limparEntrada();

        switch (opcao) {
            case 1: plantar(&j); break;
            case 2: pescar(&j); break;
            case 3: minerar(&j); break;
            case 4: mercado(&j); break;
            case 5: dormir(&j); break;
            case 6: puts("Obrigado por jogar!"); break;
            default: puts("Opção inexistente."); break;
        }

        if (venceu(&j)) {
            puts("\nVocê transformou a terra em um reino próspero!");
            puts("Parabéns, campeão de Vale Verde!");
            break;
        }
    }

    return 0;
}
