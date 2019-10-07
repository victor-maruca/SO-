#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <list> 
#include <queue>
#include <unistd.h>
#include <sstream>

using namespace std;

struct BAT {
    int uuid;
    int id;
    char queue; //Can be 'n', 'w', 'e' or 's'
    bool hasJumped = false;
};

static int authorizedBAT = -1;
static int k = 2;
static int quit = false;

static queue <BAT> NORTH_QUEUE;
static queue <BAT> EAST_QUEUE;
static queue <BAT> SOUTH_QUEUE;
static queue <BAT> WEST_QUEUE;

static void printMsg(string msg) {
    cout << msg << endl;
}

static char getNextQueue() {
    static char lastVisited = NULL;
    switch(lastVisited) {
         case 'n':
            lastVisited = 'e';
            return lastVisited;
        case 'e':
            lastVisited = 's';
            return lastVisited;
        case 's':
            lastVisited = 'w';
            return lastVisited;
        case 'w':
            lastVisited = 'n';
            return lastVisited;
        case NULL: 
            lastVisited = 'n';
            return lastVisited;
    }
    return '?';
}

static int getNextId() {
    static int previousId = 0;
    previousId++;
    return previousId;
}

void* BAT_Processor(void *_bat) {
    BAT *bat = (BAT*)_bat;
    switch(bat->queue) {
        case 'n':
            NORTH_QUEUE.push(*bat);
            break;
        case 'e':
            EAST_QUEUE.push(*bat);
            break;
        case 's':
            SOUTH_QUEUE.push(*bat);
            break;
        case 'w':
            WEST_QUEUE.push(*bat);
            break;
    }  
    stringstream msg;
    msg << "BAT " << bat->id << " " << (bat->queue) << " chegou no cruzamento.";
    printMsg(msg.str());
    while(authorizedBAT != bat->uuid) { }
    switch(bat->queue) {
        case 'n':
            NORTH_QUEUE.pop();
            break;
        case 'e':
            EAST_QUEUE.pop();
            break;
        case 's':
            SOUTH_QUEUE.pop();
            break;
        case 'w':
            WEST_QUEUE.pop();
            break;
    }  
    stringstream msg2;
    msg2 << "BAT " << bat->id << " " << (bat->queue) << " saiu no cruzamento";
    printMsg(msg2.str());
    authorizedBAT = -1;
}

void* BAT_Manager(void* args) {
    char currentQueueChar = NULL;
    queue <BAT> currentQueue;
    while(true) {
        if(quit) { pthread_exit(NULL); }
        currentQueueChar = getNextQueue();
        switch(currentQueueChar) {
            case 'n':
                currentQueue = NORTH_QUEUE;
                break;
            case 'e':
                currentQueue = EAST_QUEUE;
                break;
            case 's':
                currentQueue = SOUTH_QUEUE;
                break;
            case 'w':
                currentQueue = WEST_QUEUE;
                break;
        }  

        if(currentQueue.size() > 0) {
            if(currentQueue.size() > k && currentQueue.front().hasJumped == false) {
                switch(currentQueueChar) {
                    case 'n':
                        if(EAST_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << EAST_QUEUE.front().id << " E";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        } else if(SOUTH_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << SOUTH_QUEUE.front().id << " S";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        } else if(WEST_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << WEST_QUEUE.front().id << " W";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        } 
                    case 'e':
                        if(SOUTH_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << SOUTH_QUEUE.front().id << " S";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        } else if (WEST_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << WEST_QUEUE.front().id << " W";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        } else if(NORTH_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << NORTH_QUEUE.front().id << " N";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        }
                    case 's':
                        if(WEST_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << WEST_QUEUE.front().id << " W";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        } else if(NORTH_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << NORTH_QUEUE.front().id << " N";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        } else if(EAST_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << EAST_QUEUE.front().id << " E";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        }
                    case 'w':
                        if(NORTH_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << NORTH_QUEUE.front().id << " N";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        } else if(EAST_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << EAST_QUEUE.front().id << " E";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        } else if(SOUTH_QUEUE.size() > 0) {
                            stringstream msg;
                            msg << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue) << " cedeu passagem BAT " << SOUTH_QUEUE.front().id << " S";
                            printMsg(msg.str());
                            currentQueue.front().hasJumped = true;
                            break;
                        }
                    default:
                        authorizedBAT = currentQueue.front().uuid;
                        while(authorizedBAT != -1) { }
                        break;
                }
            } else {
                authorizedBAT = currentQueue.front().uuid;
                while(authorizedBAT != -1) {}
            } 
        }
    }
    return NULL;
}

int main() {
    pthread_t managerThread;
    pthread_create(&managerThread, NULL, BAT_Manager, NULL);

    pthread_t batThreads [100];
    BAT createdBats [100];
    int currentThreadIndex = 0;

    while(true) {
        char input[100];
        cin >> input;
        if(input[0] == 'q') {
            quit = true;
            pthread_exit(NULL);
        } else if(input[0] == 'K') {
            k = (int) input[2];
        } else {
            for (int i = 0; input[i] != '\0'; i++){
                createdBats[currentThreadIndex] = BAT {};
                createdBats[currentThreadIndex].uuid = getNextId();
                createdBats[currentThreadIndex].id = i;
                createdBats[currentThreadIndex].queue = input[i];
                pthread_create(&batThreads[currentThreadIndex], NULL, BAT_Processor, (void *)&createdBats[currentThreadIndex]);
                currentThreadIndex++;
            }
        }
    }
}