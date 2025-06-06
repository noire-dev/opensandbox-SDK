// 
// OpenSandbox
// 
// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2008-2012 OpenArena Team
// Copyright (C) 2023-2024 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// 
// This file is part of OpenSandbox.
// 
// OpenSandbox is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 2,
// as published by the Free Software Foundation.
// 
// This modified code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this project. If not, see <http://www.gnu.org/licenses/>.
// 
// Contact: opensandboxteam@gmail.com
// 

#include "g_local.h"

#define GUID_SIZE 32

typedef struct {
    char guid[GUID_SIZE+1]; //The guid is 32 chars long
    int age; //int that grows each time a new player is stored. The lowest number is always replaced. Reset to 0 then retrieved.
    int	persistant[MAX_PERSISTANT]; //This is the only information we need to save
    int timePlayed;
} playerstore_t;

static playerstore_t playerstore[MAX_CLIENTS];

static int nextAge;

 /*
 *Resets the player store. Should be called everytime game.qvm is loaded.
 */
void PlayerStoreInit( void ) {
    memset(playerstore,0,sizeof(playerstore));
    nextAge = 1;
}

void PlayerStore_store(char* guid, playerState_t ps) {
    int place2store = -1;
    int lowestAge = 32000;
    int i;
    if(strlen(guid)<32) {
        return;
    }
    for(i=0;i<MAX_CLIENTS;i++) {
        if(!Q_stricmp(guid,playerstore[i].guid)) {
            place2store=i;
        }
    }

    if(place2store<0)
    for(i=0;i<MAX_CLIENTS;i++) {
        if(playerstore[i].age < lowestAge) {
            place2store = i;
            lowestAge = playerstore[i].age;
        }
    }

    //Make sure we don't store in -1
    if(place2store<0)
        place2store = 0;
    //place2store is now the place to store to.
    playerstore[place2store].age = nextAge++;
    Q_strncpyz(playerstore[place2store].guid,guid,GUID_SIZE+1);
    memcpy(playerstore[place2store].persistant,ps.persistant,sizeof(int[MAX_PERSISTANT]));
    playerstore[place2store].timePlayed = level.time - level.clients[ps.clientNum].pers.enterTime;
}

void PlayerStore_restore(char* guid, playerState_t *ps)  {
    int i;
    if(strlen(guid)<32) {
        return;
    }
    for(i=0;i<MAX_CLIENTS;i++) {
        if(!Q_stricmpn(guid,playerstore[i].guid,GUID_SIZE) && playerstore[i].age != -1) {
            memcpy(ps->persistant,playerstore[i].persistant,sizeof(int[MAX_PERSISTANT]));
            level.clients[ps->clientNum].pers.enterTime = level.time - playerstore[i].timePlayed;
            //Never ever restore a player with negative score
            if(ps->persistant[PERS_SCORE]<0)
                ps->persistant[PERS_SCORE]=0;
            playerstore[i].age = -1;
            return;
        }
    }
}
