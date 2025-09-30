#include "fsm.h"
#include <stdio.h> // Para printf (debug)

void fsm_init(FSM *fsm) {
    fsm->current_state = FSM_STATE_IDLE;
    fsm->stan = 0; // Inicializa o STAN
    printf("FSM: Inicializada. Estado atual: %s\n", fsm_state_to_string(fsm->current_state));
}

// A função principal que controla o fluxo
void fsm_handle_event(FSM *fsm, TransactionEvent event) {
    TransactionState next_state = fsm->current_state;

    switch (fsm->current_state) {
        case FSM_STATE_IDLE:
            if (event == FSM_EVENT_CARD_INSERT) {
                next_state = FSM_STATE_CARD_INSERTED;
                fsm->stan++; // Incrementa o STAN para a nova transação
            }
            break;

        case FSM_STATE_CARD_INSERTED:
            if (event == FSM_EVENT_PIN_OK) {
                // Aqui seria o ponto para chamar o nosso montador ISO 8583
                printf("FSM: PIN OK. Montando mensagem ISO...\n");
                next_state = FSM_STATE_ISO_SENT;
            }
            break;

        case FSM_STATE_ISO_SENT:
            if (event == FSM_EVENT_SEND_SUCCESS) {
                // A mensagem foi enviada, agora esperamos a resposta
                next_state = FSM_STATE_WAITING_RESPONSE;
            }
            break;

        case FSM_STATE_WAITING_RESPONSE:
            if (event == FSM_EVENT_RESPONSE_RECEIVED_APPROVED) {
                next_state = FSM_STATE_APPROVED;
            } else if (event == FSM_EVENT_RESPONSE_RECEIVED_DECLINED) {
                next_state = FSM_STATE_DECLINED;
            } else if (event == FSM_EVENT_TIMEOUT) {
                // Lógica de reverso por time-out
                printf("FSM: TIMEOUT! Iniciando reverso...\n");
                next_state = FSM_STATE_REVERSAL;
            }
            break;
        
        case FSM_STATE_APPROVED:
        case FSM_STATE_DECLINED:
        case FSM_STATE_REVERSAL:
            if (event == FSM_EVENT_FINISH_TRANSACTION) {
                // Transação finalizada, volta ao estado ocioso
                next_state = FSM_STATE_IDLE;
            }
            break;
    }

    if (next_state != fsm->current_state) {
        printf("FSM: Transicao de %s -> %s\n", 
               fsm_state_to_string(fsm->current_state), 
               fsm_state_to_string(next_state));
        fsm->current_state = next_state;
    }
}

const char* fsm_state_to_string(TransactionState state) {
    switch (state) {
        case FSM_STATE_IDLE: return "IDLE";
        case FSM_STATE_CARD_INSERTED: return "CARD_INSERTED";
        case FSM_STATE_ISO_SENT: return "ISO_SENT";
        case FSM_STATE_WAITING_RESPONSE: return "WAITING_RESPONSE";
        case FSM_STATE_APPROVED: return "APPROVED";
        case FSM_STATE_DECLINED: return "DECLINED";
        case FSM_STATE_REVERSAL: return "REVERSAL";
        default: return "UNKNOWN";
    }
}