#ifndef FSM_H
#define FSM_H

#include <stdint.h>

// Enumeração para todos os possíveis estados da nossa transação
typedef enum {
    FSM_STATE_IDLE,             // Ocioso, esperando um cartão
    FSM_STATE_CARD_INSERTED,    // Cartão inserido, esperando PIN/senha
    FSM_STATE_ISO_SENT,         // Mensagem ISO 8583 enviada
    FSM_STATE_WAITING_RESPONSE, // Aguardando resposta do servidor
    FSM_STATE_APPROVED,         // Transação aprovada
    FSM_STATE_DECLINED,         // Transação recusada
    FSM_STATE_REVERSAL          // Estado para o time-out
} TransactionState;

// Enumeração para todos os eventos que podem acontecer e mudar o estado
typedef enum {
    FSM_EVENT_CARD_INSERT,              // Um cartão foi inserido
    FSM_EVENT_PIN_OK,                   // O PIN foi validado
    FSM_EVENT_SEND_SUCCESS,             // A mensagem foi enviada com sucesso
    FSM_EVENT_RESPONSE_RECEIVED_APPROVED, // Resposta "aprovada" recebida
    FSM_EVENT_RESPONSE_RECEIVED_DECLINED, // Resposta "recusada" recebida
    FSM_EVENT_TIMEOUT,                  // O tempo de espera pela resposta esgotou
    FSM_EVENT_FINISH_TRANSACTION        // Usuário finalizou (removeu cartão, etc.)
} TransactionEvent;

// A estrutura que representa nossa Máquina de Estados
typedef struct {
    TransactionState current_state;
    uint32_t stan; // Systems Trace Audit Number
} FSM;

/**
 * @brief Inicializa a máquina de estados.
 * @param fsm Ponteiro para a FSM.
 */
void fsm_init(FSM *fsm);

/**
 * @brief Processa um evento, mudando o estado da FSM de acordo com as regras.
 * @param fsm Ponteiro para a FSM.
 * @param event O evento que ocorreu.
 */
void fsm_handle_event(FSM *fsm, TransactionEvent event);

/**
 * @brief Retorna uma string que representa o estado atual (para debug).
 * @param state O estado.
 * @return Uma string constante.
 */
const char* fsm_state_to_string(TransactionState state);

#endif // FSM_H