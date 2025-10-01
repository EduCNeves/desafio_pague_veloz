#include <stdio.h>
#include <assert.h>
#include "fsm.h"

int main() {
    FSM fsm;

    printf("Executando testes da Maquina de Estados (FSM)...\n\n");

    printf("--- Iniciando teste de fluxo APROVADO ---\n");
    fsm_init(&fsm);
    assert(fsm.current_state == FSM_STATE_IDLE);

    fsm_handle_event(&fsm, FSM_EVENT_CARD_INSERT);
    assert(fsm.current_state == FSM_STATE_CARD_INSERTED);

    fsm_handle_event(&fsm, FSM_EVENT_PIN_OK);
    assert(fsm.current_state == FSM_STATE_ISO_SENT);

    fsm_handle_event(&fsm, FSM_EVENT_SEND_SUCCESS);
    assert(fsm.current_state == FSM_STATE_WAITING_RESPONSE);

    fsm_handle_event(&fsm, FSM_EVENT_RESPONSE_RECEIVED_APPROVED);
    assert(fsm.current_state == FSM_STATE_APPROVED);

    fsm_handle_event(&fsm, FSM_EVENT_FINISH_TRANSACTION);
    assert(fsm.current_state == FSM_STATE_IDLE);
    printf("--- Teste de fluxo APROVADO passou com sucesso ---\n\n");


    printf("--- Iniciando teste de fluxo com TIMEOUT ---\n");
    fsm_init(&fsm);
    assert(fsm.current_state == FSM_STATE_IDLE);

    fsm_handle_event(&fsm, FSM_EVENT_CARD_INSERT);
    fsm_handle_event(&fsm, FSM_EVENT_PIN_OK);
    fsm_handle_event(&fsm, FSM_EVENT_SEND_SUCCESS);
    assert(fsm.current_state == FSM_STATE_WAITING_RESPONSE);

    fsm_handle_event(&fsm, FSM_EVENT_TIMEOUT);
    assert(fsm.current_state == FSM_STATE_REVERSAL);

    fsm_handle_event(&fsm, FSM_EVENT_FINISH_TRANSACTION);
    assert(fsm.current_state == FSM_STATE_IDLE);
    printf("--- Teste de fluxo com TIMEOUT passou com sucesso ---\n\n");

    printf("Todos os testes da FSM passaram com sucesso!\n\n");

    return 0;
}