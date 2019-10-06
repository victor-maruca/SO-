#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <list> 
#include <queue>
#include <unistd.h>

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
    cout << "BAT " << bat->id << " " << (bat->queue + 32) << " chegou no cruzamento." << endl;
    while(authorizedBAT != bat->uuid) {
        usleep(1000000);
    }
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
    cout << "BAT " << bat->id << " " << (bat->queue + 32) << " saiu no cruzamento" << endl;
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
            if(currentQueue.size() > k && !currentQueue.front().hasJumped) {
                switch(currentQueueChar) {
                    case 'n':
                        if(EAST_QUEUE.size() > 0) {
                            cout << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue + 32) << " cedeu passagem BAT " << EAST_QUEUE.front().id << " E" << endl;
                            break;
                        }
                    case 'e':
                        if(SOUTH_QUEUE.size() > 0) {
                            cout << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue + 32) << " cedeu passagem BAT " << SOUTH_QUEUE.front().id << " S" << endl;
                            break;
                        }
                    case 's':
                        if(WEST_QUEUE.size() > 0) {
                            cout << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue + 32) << " cedeu passagem BAT " << WEST_QUEUE.front().id << " W" << endl;
                            break;
                        }
                    case 'w':
                        if(NORTH_QUEUE.size() > 0) {
                            cout << "BAT " << currentQueue.front().id << " " << (currentQueue.front().queue + 32) << " cedeu passagem BAT " << NORTH_QUEUE.front().id << " N" << endl;
                            break;
                        }
                    default:
                        authorizedBAT = currentQueue.front().uuid;
                        while(authorizedBAT != -1) {
                            usleep(500000);
                        }
                        break;
                }
            } else {
                authorizedBAT = currentQueue.front().uuid;
                while(authorizedBAT != -1) {
                    usleep(500000);
                }
            } 
        }
    }
    return NULL;
}

int main() {
    pthread_t managerThread;
    pthread_create(&managerThread, NULL, BAT_Manager, NULL);

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
                BAT bat = {};
                bat.uuid = getNextId();
                bat.id = i;
                bat.queue = input[i];
                pthread_t newBatThread;
                pthread_create(&newBatThread, NULL, BAT_Processor, (void *)&bat);
            }
        }
    }
}