// Inclusão de bibliotecas padrão
#include <stdio.h>  // Para entrada e saída (printf, scanf)
#include <stdlib.h> // Para funções de utilidade geral (system, exit, malloc, free)
#include <string.h> // Para manipulação de strings (strcpy, strcmp, strlen)
#include <ctype.h>  // Para manipulação de caracteres (toupper, tolower)

// Bibliotecas adicionais que podem ser úteis dependendo da implementação:
// #include <time.h>   // Se precisar lidar com datas e horários de forma mais complexa
// #include <stdbool.h> // Para usar o tipo booleano (true/false)

// --- Definições de Estruturas (Structs) ---
// Estas estruturas representam os "objetos" do seu sistema, como Pacientes, Funcionários, etc.
// Use-as para armazenar dados de forma organizada.

#define MAX_FUNCIONARIOS 100
#define MAX_PACIENTES 100
#define MAX_AGENDAMENTOS 400
#define MAX_PRONTUARIOS 200

//Arrays de armazenamento
int total_funcionarios = 0;
int total_pacientes = 0;
int total_agendamentos = 0;
int total_prontuarios = 0;

typedef struct {
    char usuario[50];
    char senha[50];
    int tipo_acesso; // 1 para Paciente, 2 para Funcionário, 3 para Administrador
} Usuario;

typedef struct {
    int id_funcionario;
    char nome_funcionario[100];
    char email_funcionario[100];
    int ativo_funcionario;
    Usuario conta_funcionario;
} Funcionario;
Funcionario funcionarios[MAX_FUNCIONARIOS];

typedef struct {
    char nome[100];
    char cpf[15]; // Ex: "123.456.789-00"
    char telefone[20];
    char endereco[150];
    int id;
    int ativo;
    Usuario conta_paciente;
} Paciente;
Paciente pacientes[MAX_PACIENTES];

typedef struct {
    char cpf_paciente[15];
    char data[11];   // Ex: "DD/MM/AAAA"
    char horario[6]; // Ex: "HH:MM"
    char descricao[200]; // Descrição do agendamento/consulta
    int confirmado; // 0 para Não, 1 para Sim
} Agendamento;
Agendamento agendamentos[MAX_AGENDAMENTOS];

typedef struct {
    char texto[1000];
    char cpf_paciente[15];
} Prontuario;
Prontuario prontuarios[MAX_PRONTUARIOS];
// --- Protótipos das Funções ---
// Declarar todas as funções que serão usadas no programa.

// Funções de Autenticação/Login
int exibir_login(char cpf_paciente_logado[]);
int realizar_login(char usuario[], char senha[], Usuario *usuario_logado); // Retorna o tipo de acesso

// Menus Principais
void menu_paciente(const char cpf_paciente[]);
void menu_funcionarios();
void menu_administradores();

// Funções de Acesso Paciente
void exibir_agendamentos_paciente(const char cpf_paciente[]);
void confirmar_agendamento(const char cpf_paciente[]);

// Funções de Acesso Funcionários
void exibir_agendamentos_funcionarios();
void gerar_prontuario_eletronico();
void agendamento_de_consultas(); // Novo Agendamento (endereço, data, horário, info paciente)
void area_do_paciente_funcionario(); // Sub-menu da área do paciente para funcionários
void cadastro_de_pacientes();
void listar_pacientes();
void listar_pacientes_por_cpf();
void acessar_prontuario_do_paciente();

// Funções de Acesso Administradores
void criar_conta_funcionarios();
void deletar_conta_funcionarios();

// Funções Utilitárias
void limpar_tela();
void pausar_execucao();
void exibir_mensagem_erro(const char *mensagem);

// --- Função Principal (main) ---
int main() {
    int tipo_acesso_logado; // 1: Paciente, 2: Funcionário, 3: Administrador
    char cpf_paciente_logado[15] = "";
    do {
        limpar_tela();
        tipo_acesso_logado = exibir_login(cpf_paciente_logado); // Chama a tela de login

        switch (tipo_acesso_logado) {
            case 1: // Acesso Paciente
                printf("Login de Paciente realizado com sucesso!\n");
                pausar_execucao();
                menu_paciente(cpf_paciente_logado);
                break;
            case 2: // Acesso Funcionários
                printf("Login de Funcionario realizado com sucesso!\n");
                pausar_execucao();
                menu_funcionarios();
                break;
            case 3: // Acesso Administradores
                printf("Login de Administrador realizado com sucesso!\n");
                pausar_execucao();
                menu_administradores();
                break;
            case 0: // Sair (opção adicionada ao login, se desejar)
                printf("Saindo do programa. Ate mais!\n");
                break;
            default:
                exibir_mensagem_erro("Credenciais invalidas ou erro no login. Tente novamente.");
                pausar_execucao();
                break;
        }
    } while (tipo_acesso_logado != 0); // O loop continua até que o usuário decida sair do login (se houver essa opção)

    return 0;
}

// --- Implementação das Funções ---

// Funções de Autenticação/Login
int exibir_login(char cpf_paciente_logado[]) {
    char usuario_input[50];
    char senha_input[50];

    printf("--- Tela de Login ---\n");
    printf("Usuario: ");
    scanf("%49s", usuario_input);
    printf("Senha: ");
    scanf("%49s", senha_input);

    // Limpa o buffer do teclado
    while (getchar() != '\n');

    // Verifica admin fixo
    if (strcmp(usuario_input, "admin") == 0 && strcmp(senha_input, "admin123") == 0) {
        return 3; // Administrador
    }

    // Verifica funcionários cadastrados
    for (int i = 0; i < total_funcionarios; i++) {
        if (funcionarios[i].ativo_funcionario == 1 &&
            strcmp(funcionarios[i].conta_funcionario.usuario, usuario_input) == 0 &&
            strcmp(funcionarios[i].conta_funcionario.senha, senha_input) == 0) {
            return 2; // Funcionário
        }
    }

    // Verifica pacientes cadastrados
    for (int i = 0; i < total_pacientes; i++) {
        if (pacientes[i].ativo == 1 &&
            strcmp(pacientes[i].conta_paciente.usuario, usuario_input) == 0 &&
            strcmp(pacientes[i].conta_paciente.senha, senha_input) == 0) {
            strcpy(cpf_paciente_logado, pacientes[i].cpf); // Salva o CPF do paciente logado
            return 1; // Paciente
        }
    }

    // Se não encontrou
    return -1;
}

// Menus Principais
void menu_paciente(const char cpf_paciente[]) {
    int escolha;

    do {
        limpar_tela();
        printf("--- Menu do Paciente ---\n");
        printf("1. Exibir Agendamentos\n");
        printf("2. Confirmar Agendamento\n");
        printf("0. Voltar ao Login\n");
        printf("------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        while (getchar() != '\n'); // Limpa o buffer

        switch (escolha) {
            case 1:
                exibir_agendamentos_paciente(cpf_paciente);
                break;
            case 2:
                confirmar_agendamento(cpf_paciente);
                break;
            case 0:
                printf("Voltando a tela de Login...\n");
                break;
            default:
                exibir_mensagem_erro("Opcao invalida. Tente novamente.");
                break;
        }
        pausar_execucao(); // Pausa antes de limpar a tela para o próximo menu
    } while (escolha != 0);
}

void menu_funcionarios() {
    int escolha;
    do {
        limpar_tela();
        printf("--- Menu de Funcionarios ---\n");
        printf("1. Exibir Agendamentos\n");
        printf("2. Agendamento de Consultas\n");
        printf("3. Area do Paciente\n");
        printf("0. Voltar ao Login\n");
        printf("----------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        while (getchar() != '\n'); // Limpa o buffer

        switch (escolha) {
            case 1:
                exibir_agendamentos_funcionarios();
                break;
            case 2:
                agendamento_de_consultas();
                break;
            case 3:
                area_do_paciente_funcionario();\
                break;
            case 0:
                printf("Voltando a tela de Login...\n");
                break;
            default:
                exibir_mensagem_erro("Opcao invalida. Tente novamente.");
                break;
        }
        pausar_execucao();
    } while (escolha != 0);
}

void menu_administradores() {
    int escolha;
    do {
        limpar_tela();
        printf("--- Menu de Administradores ---\n");
        printf("1. Criar Conta de Funcionarios\n");
        printf("2. Deletar Conta de Funcionarios\n");
        printf("0. Voltar ao Login\n");
        printf("-------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        while (getchar() != '\n'); // Limpa o buffer

        switch (escolha) {
            case 1:
                criar_conta_funcionarios();
                break;
            case 2:
                deletar_conta_funcionarios();
                break;
            case 0:
                printf("Voltando a tela de Login...\n");
                break;
            default:
                exibir_mensagem_erro("Opcao invalida. Tente novamente.");
                break;
        }
        pausar_execucao();
    } while (escolha != 0);
}

// Funções de Acesso Paciente (Implementações Simplificadas)
void exibir_agendamentos_paciente(const char cpf_paciente[]) {
    limpar_tela();
    printf("--- Meus Agendamentos (Paciente: %s) ---\n", cpf_paciente);

    int encontrou = 0;
    for (int i = 0; i < total_agendamentos; i++) {
        if (strcmp(agendamentos[i].cpf_paciente, cpf_paciente) == 0) {
            printf("Data: %s | Horário: %s | Status: %s | Descrição: %s\n",
                   agendamentos[i].data,
                   agendamentos[i].horario,
                   agendamentos[i].confirmado == 1 ? "Confirmado" : "Pendente",
                   agendamentos[i].descricao);
            encontrou = 1;
        }
    }
    if (!encontrou) {
        printf("Nenhum agendamento encontrado para este CPF.\n");
    }
    pausar_execucao();
}

void confirmar_agendamento(const char cpf_paciente[]) {
    limpar_tela();
    printf("--- Confirmar Agendamento (Paciente: %s) ---\n", cpf_paciente);

    int indices[MAX_AGENDAMENTOS];
    int count = 0;
    // Listar agendamentos do paciente
    for (int i = 0; i < total_agendamentos; i++) {
        if (strcmp(agendamentos[i].cpf_paciente, cpf_paciente) == 0) {
            printf("%d) Data: %s | Horário: %s | Status: %s | Descrição: %s\n",
                   count + 1,
                   agendamentos[i].data,
                   agendamentos[i].horario,
                   agendamentos[i].confirmado == 1 ? "Confirmado" : "Pendente",
                   agendamentos[i].descricao);
            indices[count] = i;
            count++;
        }
    }
    if (count == 0) {
        printf("Nenhum agendamento encontrado para este CPF.\n");
        pausar_execucao();
        return;
    }

    int escolha;
    printf("\nDigite o número do agendamento que deseja confirmar (0 para cancelar): ");
    scanf("%d", &escolha);
    while (getchar() != '\n');

    if (escolha > 0 && escolha <= count) {
        int idx = indices[escolha - 1];
        agendamentos[idx].confirmado = 1;
        printf("Agendamento confirmado com sucesso!\n");
    } else if (escolha == 0) {
        printf("Operação cancelada.\n");
    } else {
        printf("Opção inválida.\n");
    }
    pausar_execucao();
}

// Funções de Acesso Funcionários (Implementações Simplificadas)
void exibir_agendamentos_funcionarios() {
    limpar_tela();
    if (totalAgendamentos == 0) {
        printf("Nenhum agendamento encontrado.\n");
        return;
    }

    printf("\n--- Lista de Agendamentos ---\n");
    for (int i = 0; i < totalAgendamentos; i++) {
        printf("%d. Nome: %s | CPF: %s | Data: %s | Hora: %s\n", 
               i + 1, 
               agendamentos[i].paciente.nome, 
               agendamentos[i].paciente.cpf, 
               agendamentos[i].data, 
               agendamentos[i].hora);
    }

    int escolha;
    printf("\nDigite o número do agendamento para gerar prontuário (ou 0 para voltar): ");
    scanf("%d", &escolha);

    if (escolha <= 0 || escolha > totalAgendamentos) {
        printf("Voltando ao menu...\n");
        return;
    }

    int index = escolha - 1;
    printf("Você selecionou o agendamento de %s. Deseja gerar prontuário? (1 - Sim | 0 - Não): ", agendamentos[index].paciente.nome);
    
    int opcao;
    scanf("%d", &opcao);

    if (opcao == 1) {
        if (totalProntuarios < MAX_USUARIOS) {
            strcpy(prontuarios[totalProntuarios].texto, "Consulta realizada com perguntas padrão e respostas do paciente.");
            strcpy(prontuarios[totalProntuarios].cpf_paciente, agendamentos[index].paciente.cpf);
            totalProntuarios++;
            printf("Prontuário gerado com sucesso!\n");
        } else {
            printf("Limite de prontuários atingido.\n");
        }
    } else {
        printf("Prontuário não gerado.\n");
    }
    pausar_execucao();
}

void gerar_prontuario_eletronico() {
    limpar_tela();
    if (totalProntuarios == 0) {
        printf("Nenhum prontuário encontrado.\n");
        return;
    }

    printf("\n--- Prontuários Cadastrados ---\n");
    for (int i = 0; i < totalProntuarios; i++) {
        printf("%d. CPF do Paciente: %s\n", i + 1, prontuarios[i].cpf_paciente);
    }

    int escolha;
    printf("\nDigite o número do prontuário que deseja visualizar (ou 0 para voltar): ");
    scanf("%d", &escolha);

    if (escolha <= 0 || escolha > totalProntuarios) {
        printf("Voltando ao menu...\n");
        return;
    }

    int index = escolha - 1;
    printf("\n--- Detalhes do Prontuário ---\n");
    printf("CPF do Paciente: %s\n", prontuarios[index].cpf_paciente);
    printf("Conteúdo do Prontuário:\n%s\n", prontuarios[index].texto);
    pausar_execucao();
}

void agendamento_de_consultas() {
    int c;
    limpar_tela();

    // 1. Verificar se há espaço para novos agendamentos
    if (total_agendamentos >= MAX_AGENDAMENTOS) {
        printf("Limite de agendamentos atingido. Não é possível criar novos agendamentos.\n");
        pausar_execucao();
        return;
    }

    Agendamento novo_agendamento;

    printf("--- Agendamento de Consultas ---\n");

    // 2. Coletar o CPF do paciente
    printf("CPF do Paciente para o agendamento (Ex: 123.456.789-00): ");
    fgets(novo_agendamento.cpf_paciente, sizeof(novo_agendamento.cpf_paciente), stdin);
    novo_agendamento.cpf_paciente[strcspn(novo_agendamento.cpf_paciente, "\n")] = 0;

    // Opcional: Aqui você pode adicionar uma lógica para verificar se o CPF existe no array de pacientes
    int paciente_encontrado = 0;
    for (int i = 0; i < total_pacientes; i++) {
        if (strcmp(pacientes[i].cpf, novo_agendamento.cpf_paciente) == 0 && pacientes[i].ativo == 1) {
            paciente_encontrado = 1;
            break;
        }
    }
    if (!paciente_encontrado) {
        printf("ERRO: Paciente com o CPF informado não encontrado ou inativo.\n");
        pausar_execucao();
        return;
    }

    // 3. Coletar a data
    printf("Data do agendamento (DD/MM/AAAA): ");
    fgets(novo_agendamento.data, sizeof(novo_agendamento.data), stdin);
    novo_agendamento.data[strcspn(novo_agendamento.data, "\n")] = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Limpa o buffer

    // 4. Coletar o horário
    printf("Horário do agendamento (HH:MM): ");
    fgets(novo_agendamento.horario, sizeof(novo_agendamento.horario), stdin);
    novo_agendamento.horario[strcspn(novo_agendamento.horario, "\n")] = 0;
    while ((c = getchar()) != '\n' && c != EOF); // Limpa o buffer

    // 5. Coletar a descrição
    printf("Descrição breve do agendamento/consulta: ");
    fgets(novo_agendamento.descricao, sizeof(novo_agendamento.descricao), stdin);
    novo_agendamento.descricao[strcspn(novo_agendamento.descricao, "\n")] = 0;

    // 6. Definir o agendamento como não confirmado inicialmente
    novo_agendamento.confirmado = 0; // Por padrão, não confirmado

    // 7. Armazenar o novo agendamento no array global
    agendamentos[total_agendamentos++] = novo_agendamento;

    printf("\nAgendamento Cadastrado com Sucesso!\n");
    printf("--- Detalhes do Agendamento ---\n");
    printf("CPF do Paciente: %s\n", novo_agendamento.cpf_paciente);
    printf("Data: %s\n", novo_agendamento.data);
    printf("Horário: %s\n", novo_agendamento.horario);
    printf("Descrição: %s\n", novo_agendamento.descricao);
    printf("Status: %s\n", novo_agendamento.confirmado == 1 ? "Confirmado" : "Pendente");

    pausar_execucao();
}

void area_do_paciente_funcionario() {
    int escolha;
    do {
        limpar_tela();
        printf("--- Area do Paciente (Funcionario) ---\n");
        printf("1. Cadastro de Pacientes\n");
        printf("2. Listar Pacientes\n");
        printf("3. Listar Pacientes por CPF\n");
        printf("4. Acessar Prontuario do Paciente\n");
        printf("0. Voltar ao Menu de Funcionarios\n");
        printf("--------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        while (getchar() != '\n'); // Limpa o buffer

        switch (escolha) {
            case 1:
                cadastro_de_pacientes();
                break;
            case 2:
                listar_pacientes();
                break;
            case 3:
                listar_pacientes_por_cpf();
                break;
            case 4:
                acessar_prontuario_do_paciente();
                break;
            case 0:
                printf("Voltando...\n");
                break;
            default:
                exibir_mensagem_erro("Opcao invalida. Tente novamente.");
                break;
        }
        pausar_execucao();
    } while (escolha != 0);
}

void cadastro_de_pacientes() {
    limpar_tela();
    
    if(total_pacientes >= MAX_PACIENTES){
        printf("Limite de pacientes atigindo \n");
        return;
    }
    Paciente P;
    P.id = total_pacientes  + 1;
    P.ativo = 1;

    printf("Nome do Paciente: \n"); //nome do Paciente
    fgets(P.nome, sizeof(P.nome), stdin);
    P.nome[strcspn(P.nome, "\n")] = 0;

    printf("CPF do Paciente: \n"); //email do Paciente
    fgets(P.cpf, sizeof(P.cpf), stdin);
    P.cpf[strcspn(P.cpf, "\n")] = 0;

    printf("Usuário do Paciente: \n"); //usuario 
    fgets(P.conta_paciente.usuario, sizeof(P.conta_paciente.usuario), stdin);
    P.conta_paciente.usuario[strcspn(P.conta_paciente.usuario, "\n")] = 0;

    printf("Senha do Paciente: \n"); //senha
    fgets(P.conta_paciente.senha, sizeof(P.conta_paciente.senha), stdin);
    P.conta_paciente.senha[strcspn(P.conta_paciente.senha, "\n")] = 0;

    P.conta_paciente.tipo_acesso = 1; // 1=Paciente/2=Funcionario

    pacientes[total_pacientes++] = P;

    printf("\nPaciente Cadastrado com Sucesso! \n"); // Adicionei \n para espaçamento
    printf("ID do Paciente: %d \n", P.id); // Imprimir o ID
    printf("Nome do Paciente: %s \n", P.nome); // Corrigido para %s
    printf("CPF do Paciente: %s \n", P.cpf); // Corrigido para %s
    printf("Usuário do Paciente: %s \n", P.conta_paciente.usuario); // Corrigido para %s
    printf("Senha do Paciente: %s \n", P.conta_paciente.senha); // Corrigido para %s

    pausar_execucao();
}

void listar_pacientes() {
    limpar_tela();
    printf("--- Listagem de Pacientes ---\n");

    if (total_pacientes == 0) {
        printf("Nenhum paciente cadastrado no momento.\n");
    } else {
        // Cabeçalho das colunas
        // Ajuste os números (ex: %-5s, %-30s) conforme o tamanho máximo esperado para cada campo
        printf("%-5s %-30s %-15s %-15s %-40s %-20s %-8s %-20s\n",
               "ID", "Nome", "CPF", "Telefone", "Endereco", "Usuario", "Status", "Senha");
        printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        for (int i = 0; i < total_pacientes; i++) {
            printf("%-5d %-30s %-15s %-15s %-40s %-20s %-8s %-20s\n",
                   pacientes[i].id,
                   pacientes[i].nome,
                   pacientes[i].cpf,
                   pacientes[i].telefone,
                   pacientes[i].endereco,
                   pacientes[i].conta_paciente.usuario,
                   pacientes[i].ativo == 1 ? "Ativo" : "Inativo",
                   pacientes[i].conta_paciente.senha);
        }
    }
    pausar_execucao();
}

void listar_pacientes_por_cpf() {
    char cpf_busca[15];
    limpar_tela();
    printf("--- Listar Paciente por CPF ---\n");
    printf("Digite o CPF do paciente para buscar (Ex: 123.456.789-00): ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin);
    cpf_busca[strcspn(cpf_busca, "\n")] = 0; // Remove o '\n'

    int paciente_encontrado = 0; // Flag para verificar se o paciente foi encontrado

    // Cabeçalho das colunas (mesmo da listagem geral de pacientes)
    printf("\n%-5s %-30s %-15s %-15s %-40s %-20s %-8s %-20s\n",
           "ID", "Nome", "CPF", "Telefone", "Endereco", "Usuario", "Status", "Senha");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < total_pacientes; i++) {
        if (strcmp(pacientes[i].cpf, cpf_busca) == 0) {
            // Paciente encontrado, exibe os detalhes
            printf("%-5d %-30s %-15s %-15s %-40s %-20s %-8s %-20s\n",
                   pacientes[i].id,
                   pacientes[i].nome,
                   pacientes[i].cpf,
                   pacientes[i].telefone,
                   pacientes[i].endereco,
                   pacientes[i].conta_paciente.usuario,
                   pacientes[i].ativo == 1 ? "Ativo" : "Inativo",
                   pacientes[i].conta_paciente.senha);
            paciente_encontrado = 1;
            break; // Sai do loop assim que encontrar o paciente
        }
    }

    if (!paciente_encontrado) {
        printf("\nPaciente com o CPF '%s' não encontrado.\n", cpf_busca);
    }

    pausar_execucao();
}

void acessar_prontuario_do_paciente() {
    char cpf_acesso[15];
    limpar_tela();
    printf("--- Acessar Prontuario do Paciente ---\n");
    printf("Digite o CPF do paciente para acessar o prontuario: ");
    scanf("%14s", cpf_acesso);
    while (getchar() != '\n'); // Limpa o buffer
    printf("Lógica para acessar e exibir/editar o prontuário do paciente com CPF: %s\n", cpf_acesso);
    pausar_execucao();
}

// Funções de Acesso Administradores (Implementações Simplificadas)
void criar_conta_funcionarios() {
    limpar_tela();
    if (total_funcionarios >= MAX_FUNCIONARIOS){
        printf("Limite de funcionários atingido.\n");
        return;
    }

    Funcionario f;
    f.id_funcionario = total_funcionarios + 1;
    f.ativo_funcionario = 1;
    
    printf("--- Criar Conta de Funcionarios ---\n");
    printf("Nome do Funcionário: ");
    // Usar fgets para leitura segura de strings com espaços e evitar buffer overflows
    fgets(f.nome_funcionario, sizeof(f.nome_funcionario), stdin);
    // Remover o caractere de nova linha que fgets inclui
    f.nome_funcionario[strcspn(f.nome_funcionario, "\n")] = 0; 

    printf("Email do Funcionário: ");
    fgets(f.email_funcionario, sizeof(f.email_funcionario), stdin);
    f.email_funcionario[strcspn(f.email_funcionario, "\n")] = 0;

    printf("Login do Funcionário: ");
    fgets(f.conta_funcionario.usuario, sizeof(f.conta_funcionario.usuario), stdin);
    f.conta_funcionario.usuario[strcspn(f.conta_funcionario.usuario, "\n")] = 0;
    
    printf("Senha do Funcionário: ");
    fgets(f.conta_funcionario.senha, sizeof(f.conta_funcionario.senha), stdin);
    f.conta_funcionario.senha[strcspn(f.conta_funcionario.senha, "\n")] = 0; // remove o caractere de \n que o fgets pode registrar

    f.conta_funcionario.tipo_acesso = 2; //2 = Funcionário | 1 = Paciente 

    funcionarios[total_funcionarios++] = f; // Atribui a struct e incrementa total_funcionarios

    // exemplo usado somente para mostrar as informações registradas

    printf("\nFuncionário Cadastrado com Sucesso! \n"); // Adicionei \n para espaçamento
    printf("ID do funcionário: %d \n", f.id_funcionario); // Imprimir o ID
    printf("Nome do funcionário: %s \n", f.nome_funcionario); // Corrigido para %s
    printf("Email do funcionário: %s \n", f.email_funcionario); // Corrigido para %s
    printf("Usuário do funcionário: %s \n", f.conta_funcionario.usuario); // Corrigido para %s
    printf("Senha do funcionário: %s \n", f.conta_funcionario.senha); // Corrigido para %s

    // fim do exemplo

    // A linha "Informações do cadastro" é um pouco redundante aqui, mas se quiser mantê-la:
    printf("\nInformações do cadastro\n"); // Adicionei \n para espaçamento

    pausar_execucao(); 
}

void deletar_conta_funcionarios() {
    limpar_tela();
    printf("--- Deletar Conta de Funcionarios ---\n");
    
    pausar_execucao();
}

// Funções Utilitárias
void limpar_tela() { //Limpar Tela
    system("cls");
}

void pausar_execucao() { //Pausar Execução
    printf("\nPressione ENTER para continuar...");
    while (getchar() != '\n'); // Espera pelo ENTER
}

void exibir_mensagem_erro(const char *mensagem) { // Mensagem de erro
    printf("\nERRO: %s\n", mensagem);
}