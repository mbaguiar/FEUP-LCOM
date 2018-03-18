#include "MultiplayerCtrl.h"
static int host;
static int timeout;
static unsigned int seed;
static lobby_state_t state;
static lobby_exit_cause_t exit_cause;
static connection_state_t connection;

// initially state = LOOKING_FOR_HOST

void enterLobby(){
        host = 0;
        timeout = 900; //10 seconds for timeout
        state = LOOKING_FOR_HOST;
        connection = NOT_EST;
}

// Lobby sync

int lobbyManager(){
//print_location();
        if (timeout == 0) {
                exit_cause = TIMEOUT;
                state = CANCEL_LOOKING;
        }

        switch (state) {

        case LOOKING_FOR_HOST:

                if (serialport_emptyRecQueue()) {
                        serialport_sendStringToBuffer("pLook");
                        print_dbg("Host not found. Looking for player\n");
                        state = LOOKING_FOR_PLAYER;
                        host = 1;
                } else {
                        serialport_sendStringToBuffer("hLook");
                        char * string = (char *)serialport_getWordFromBuffer();
                        if (string != NULL) {
                                if (strcmp(string, "pLook") == 0) {
                                        host = 0;
                                        state = FOUND_HOST;
                                }
                                free (string);
                        }
                        if (state == FOUND_HOST) {
                                timeout = 600;
                                print_dbg("State: %d | Connection: %d\n", state, connection);
                                print_dbg("Found host. Sending pConnect\n");
                                serialport_sendStringToBuffer("pConnect");
                        }

                }

                break;

        case LOOKING_FOR_PLAYER:

                if (!serialport_emptyRecQueue()) {

                        char * string = (char *)serialport_getWordFromBuffer();
                        if (string != NULL) {
                                if (strcmp(string, "hLook") == 0) {
                                        state = FOUND_PLAYER;
                                }
                                free (string);
                        }
                        if (state == FOUND_PLAYER) {
                                timeout = 600;
                                print_dbg("State: %d | Connection: %d\n", state, connection);
                                print_dbg("Found player. Sending hConnect\n");
                                serialport_sendStringToBuffer("hConnect");
                        }
                }

                break;

        case FOUND_HOST:
                if (host != 0) {
                        print_location();
                        print_dbg("Found host and is host. Canceling\n");
                        exit_cause = ERR;
                        state = CANCEL_LOOKING;
                } else {

                        if (!serialport_emptyRecQueue()) {
                                char * string = (char *)serialport_getWordFromBuffer();
                                if (string != NULL) {
                                        if (strcmp(string, "hConnect") == 0) {
                                                connection = PLAYER;
                                                state = CONNECTED;
                                                timeout = 600;
                                                print_dbg("State: %d | Connection: %d\n", state, connection);
                                                print_dbg("Connected as player\n");
                                        }
                                        free (string);
                                }

                        }
                }
                break;

        case FOUND_PLAYER:
                if (host != 1) {
                        print_location();
                        print_dbg("Found player and is player. Canceling\n");
                        exit_cause = ERR;
                        state = CANCEL_LOOKING;
                } else {
                        if (!serialport_emptyRecQueue()) {
                                char * string = (char *)serialport_getWordFromBuffer();
                                if (string != NULL) {
                                        if (strcmp(string, "pConnect") == 0) {
                                                connection = HOST;
                                                state = CONNECTED;
                                                timeout = 600;
                                                print_dbg("State: %d | Connection: %d\n", state, connection);
                                                print_dbg("Connected as host\n");
                                        }
                                        free (string);
                                }
                        }
                }

                break;

        case CONNECTED:
                if (connection == NOT_EST) {
                        print_location();
                        print_dbg("No connection found. Canceling\n");
                        exit_cause = ERR;
                        state = CANCEL_LOOKING;
                } else if (connection == HOST) {
                        seed = generateSeed();
                        char seedMsg[11];
                        sprintf(seedMsg, "s%u", seed);
                        serialport_sendStringToBuffer(seedMsg); //send seed
                        state = READY_HOST;
                        timeout = 600;
                        print_dbg("Seed sent: %s. Host ready\n", seedMsg);
                } else if (connection == PLAYER) { //connection == PLAYER
                        if (!serialport_emptyRecQueue()) {
                                char * string = (char *)serialport_getWordFromBuffer();
                                if (string != NULL) {
                                        if (string[0] == 's') {
                                                print_dbg("identified as seed\n");
                                                seed = strtol(string + 1, NULL, 10);
                                                if (seed == 0x0L) {
                                                        exit_cause = ERR;
                                                        state = CANCEL_LOOKING;
                                                        return 1;
                                                }
                                                serialport_sendStringToBuffer("pReady");
                                                timeout = 600;
                                                state = READY_PLAYER;
                                                print_dbg("Seed %u received. Player ready\n", seed);
                                        }
                                        free (string);
                                }
                        }
                }
                break;

        case READY_HOST:
                if (!serialport_emptyRecQueue()) {
                        char * string = (char *)serialport_getWordFromBuffer();
                        if (string != NULL) {
                                if (strcmp(string, "pReady") == 0) {
                                        serialport_sendStringToBuffer("hReady");
                                        state = READY_BOTH;
                                        timeout = 600;
                                        print_dbg("both ready");
                                }
                                free (string);
                        }

                }
                break;

        case READY_PLAYER:
                if (!serialport_emptyRecQueue()) {
                        char * string = (char *)serialport_getWordFromBuffer();
                        if (string != NULL) {
                                if (strcmp(string, "hReady") == 0) {
                                        state = READY_BOTH;
                                        timeout = 600;
                                        print_dbg("both ready");
                                }
                                free (string);
                        }

                }
                break;

        case READY_BOTH:
                print_location();
                print_dbg("Start game\n");
                exit_cause = SYNCED;
                return 0;
                break;

        case CANCEL_LOOKING:
                print_location();
                print_dbg("Exiting. Cause: %d\n", exit_cause);
                return -1;

        }
        timeout--;
        return 1;
}

unsigned int getSeed() {
        return seed;
}
