// Inclusão de bibliotecas padrãoAdd commentMore actions
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

typedef struct {
    char nome[100];
    char cpf[15]; // Ex: "123.456.789-00"
    char telefone[20];
    char endereco[150];
    // Adicione outros campos relevantes para o paciente
} Paciente;

typedef struct {
    char usuario[50];
    char senha[50];
    int tipo_acesso; // 1 para Paciente, 2 para Funcionário, 3 para Administrador
    // Adicione outros campos relevantes para o usuário/funcionário
} Usuario;

typedef struct {
    char cpf_paciente[15];
    char data[11];   // Ex: "DD/MM/AAAA"
    char horario[6]; // Ex: "HH:MM"
    char descricao[200]; // Descrição do agendamento/consulta
    int confirmado; // 0 para Não, 1 para Sim
    // Adicione outros campos relevantes para o agendamento
} Agendamento;

// --- Protótipos das Funções ---
// Declarar todas as funções que serão usadas no programa.

// Funções de Autenticação/Login
int exibir_login();
int realizar_login(char usuario[], char senha[], Usuario *usuario_logado); // Retorna o tipo de acesso

// Menus Principais
void menu_paciente();
void menu_funcionarios();
void menu_administradores();

// Funções de Acesso Paciente
void exibir_agendamentos_paciente(const char cpf_paciente[]);
void confirmar_agendamento(const char cpf_paciente[]);

// Funções de Acesso Funcionários
void exibir_agendamentos_funcionarios();
void cancelar_remarcar_agendamentos();
void realizar_consulta();
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
// Poderíamos ter funções para carregar/salvar dados em arquivos (ex: carregar_usuarios(), salvar_agendamentos())

// --- Função Principal (main) ---
int main() {
    int tipo_acesso_logado; // 1: Paciente, 2: Funcionário, 3: Administrador
    Usuario usuario_atual;   // Armazena os dados do usuário logado

    do {
        limpar_tela();
        tipo_acesso_logado = exibir_login(); // Chama a tela de login

        switch (tipo_acesso_logado) {
            case 1: // Acesso Paciente
                printf("Login de Paciente realizado com sucesso!\n");
                pausar_execucao();
                menu_paciente();
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
int exibir_login() {
    char usuario_input[50];
    char senha_input[50];
    Usuario usuario_autenticado; // Estrutura para armazenar o usuário se o login for bem-sucedido

    printf("--- Tela de Login ---\n");
    printf("Usuario: ");
    scanf("%49s", usuario_input);
    printf("Senha: ");
    scanf("%49s", senha_input);

    // Limpa o buffer do teclado
    while (getchar() != '\n');

    // Aqui você implementaria a lógica de verificação de usuário e senha.
    // Isso envolveria ler de um arquivo ou um banco de dados de usuários.
    // Por simplicidade, vou usar um exemplo fixo.
    if (strcmp(usuario_input, "admin") == 0 && strcmp(senha_input, "admin123") == 0) {
        return 3; // Administrador
    } else if (strcmp(usuario_input, "func") == 0 && strcmp(senha_input, "func123") == 0) {
        return 2; // Funcionário
    } else if (strcmp(usuario_input, "paciente") == 0 && strcmp(senha_input, "paciente123") == 0) {
        return 1; // Paciente
    } else {
        return -1; // Login falhou
    }
}

// Menus Principais
void menu_paciente() {
    int escolha;
    char cpf_paciente[15]; // Supondo que o CPF do paciente logado esteja disponível aqui

    // Em um sistema real, o CPF do paciente logado seria passado como parâmetro ou obtido de forma segura.
    printf("Informe seu CPF para acesso (apenas para simulação): ");
    scanf("%14s", cpf_paciente);
    while (getchar() != '\n'); // Limpa o buffer

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
                area_do_paciente_funcionario();
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
    printf("Aqui seriam listados os agendamentos do paciente com o CPF: %s\n", cpf_paciente);
    // Lógica para carregar e exibir agendamentos do paciente
    pausar_execucao();
}

void confirmar_agendamento(const char cpf_paciente[]) {
    limpar_tela();
    printf("--- Confirmar Agendamento (Paciente: %s) ---\n", cpf_paciente);
    printf("Aqui o paciente poderia confirmar um agendamento existente.\n");
    // Lógica para permitir ao paciente confirmar um agendamento
    pausar_execucao();                                              
}

// Funções de Acesso Funcionários (Implementações Simplificadas)
void exibir_agendamentos_funcionarios
    char agendamentos[30] = {
        printf ("Lista de agendamento: \n");
   for (int i = 0; i < total_agendamentos; i++) {
    printf("%d. %s\n", i + 1, agendamentos[i]);
}

     limpar_tela();
    printf("--- Exibir Agendamento do Funcionario) ---\n");
    printf("Solicitar: Endereco da Clinica/Data/Horario/Informacoes do Paciente.\n");
    // Coletar dados para o novo agendamento

void cancelar_remarcar_agendamentos() {
    limpar_tela();
    printf("--- Cancelar/Remarcar Agendamentos ---\n");
    printf("Lógica para cancelar ou remarcar agendamentos por um funcionário.\n");
    pausar_execucao();
}

void realizar_consulta() {
    limpar_tela();
    printf("--- Realizar Consulta ---\n");
    printf("Aqui seria o fluxo para o funcionário registrar uma consulta.\n");
    // Após a consulta, você pode chamar gerar_prontuario_eletronico()
    gerar_prontuario_eletronico(); // Chamada dentro do fluxo, como no fluxograma
    pausar_execucao();
}

void gerar_prontuario_eletronico() {
    printf("\n--- Gerar Prontuario Eletronico ---\n");
    printf("Lógica para criar ou atualizar o prontuário eletrônico do paciente.\n");
    // Esta função pode ser chamada dentro de "Realizar Consulta" ou de forma independente
    // pausar_execucao(); // Comentado pois é chamada de dentro de outra função
}

void agendamento_de_consultas() {
    limpar_tela();
    printf("--- Agendamento de Consultas (Funcionario) ---\n");
    printf("Solicitar: Endereco da Clinica/Data/Horario/Informacoes do Paciente.\n");
    // Coletar dados para o novo agendamento
    printf("Nome do funcionario: \n");
    scanf("%[^\n]", f.nome_funcionario);
    printf("data da consulta: \n");
    scanf("%[^\n]", f.data_consulta);
    printf("Horario do agendamento: \n");
    scanf("%[^\n]", f.horario_agendamento);
    printf("Endereco da Clinica: \n");
    scanf("%[^\n]", f.endereco_da_clinica);
    printf("Informacoes do paciente: \n");
    scanf("%[^\n]", f.informacoes_do_paciente);
    printf("Agendamento Realizado com Sucesso!\n"); // Simulação de confirmação
    

    printf("\nAgenda Cadastrada com Sucesso! \n"); // Adicionei \n para espaçamento
    printf("Nome do funcionário: %s \n", f.nome_funcionario); // Corrigido para %s
    printf("Data da consulta: %s \n", f.data_consulta); // Corrigido para %s
    printf("Horario do agendamento: %s \n", f.horario_agendamento); // Corrigido para %s
    printf("Endereco da Clinica: %s \n", f.endereco_da_clinica); // Corrigido para %s
    printf("Informacoes do paciente: %s \n", f.informacoes_do_paciente); // corrigido para %s

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
    printf("--- Cadastro de Pacientes ---\n");
    printf("Lógica para adicionar um novo paciente ao sistema.\n");
    // Coletar nome, CPF, telefone, endereço, etc.
    pausar_execucao();
}

void listar_pacientes() {
    limpar_tela();
    printf("--- Listar Pacientes ---\n");
    printf("Lógica para exibir uma lista de todos os pacientes cadastrados.\n");
    pausar_execucao();
}

void listar_pacientes_por_cpf() {
    char cpf_busca[15];
    limpar_tela();
    printf("--- Listar Pacientes por CPF ---\n");
    printf("Digite o CPF do paciente para buscar: ");
    scanf("%14s", cpf_busca);
    while (getchar() != '\n'); // Limpa o buffer
    printf("Lógica para buscar e exibir dados do paciente com CPF: %s\n", cpf_busca);
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
    printf("--- Criar Conta de Funcionarios ---\n");
    printf("Lógica para adicionar um novo usuário/funcionário ao sistema (com nome de usuário, senha e tipo de acesso).\n");
    pausar_execucao();
}

void deletar_conta_funcionarios() {
    limpar_tela();
    printf("--- Deletar Conta de Funcionarios ---\n");
    printf("Lógica para remover um usuário/funcionário existente.\n");
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
}Add comment
