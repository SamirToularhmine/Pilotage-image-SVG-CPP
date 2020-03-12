//
// Created by gunteam on 05/02/2020.
// Source: https://bousk.developpez.com/cours/reseau-c++/UDP/01-introduction-premiers-pas/ - https://bousk.developpez.com/cours/reseau-c++/TCP/04-serveur-premiers-pas
//
#include "include/functions.hpp"


using namespace std;

functions::functions() {

    //////////////////////////////////////////////
    //                                          //
    //                                          //
    //                  SOCKET                  //
    //                                          //
    //                                          //
    //////////////////////////////////////////////



    //Création de la socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        throw udpRuntimeException(IP_CLIENT, PORT);
    }

    //Initilisation de la socket
    serv_addr.sin_addr.s_addr = inet_addr(IP_CLIENT);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);


    // Convertion des addresses IP en binaire
    if(inet_pton(AF_INET, IP_CLIENT, &serv_addr.sin_addr) <= 0)
    {
        throw udpAdresseConvertionException();
    }

    // Binds
    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        throw udpBindsException();
    }

    // Modification de l'ip pour atteindre le serveur
    serv_addr.sin_addr.s_addr = inet_addr(IP_SERVER);


}

functions::~functions() {

}

void functions::sendData(cbor::map data) {

    cbor::binary encoded = cbor::encode(data);

    // Envoie de message
    if (sendto(sock, encoded.data(), encoded.size(), 0, (const sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        throw udpSendingException();
    }
    cout << "Message envoyé" << endl;

    // Réception des informations modifiables
    if (data.begin()->first == "?")
        getModifiableInformations();
}

void functions::getModifiableInformations() {

    char buffer[1024] = {0};

    if (recv(sock, buffer, sizeof(buffer), 0) < 0) {
        throw udpReceiveException();
    }

    string element = (string) buffer;
    string delimiter = "\n";
    vector<string> modifiableItems;
    int x = 0;
    while (x < 5) {
        modifiableItems.push_back(element.substr(0, element.find_first_of(delimiter)));
        element.erase(0, (int) element.find_first_of(delimiter)+1);
        ++x;
    }

    showModifiableItems(modifiableItems);

}



void functions::showModifiableItems(vector<string> modifiableItems) {

    //Affichage des éléments modifiables
    for (auto it = modifiableItems.begin(); it != modifiableItems.end(); ++it) {
        cout << (*it) << endl;
    }


    string choice;
    do {
        cout << "Entrez le nom de l'item a modifier" << endl;
        getline(cin, choice);
    } while (!isItemExist(modifiableItems, choice));

    cbor::map data;

    for(vector<string>::iterator it = modifiableItems.begin(); it != modifiableItems.end(); ++it) {
        if(choice.compare((*it))==0){
            data[(*it)] = modifyItem((*it));
            break;
        }
    }

    string resp;

    do {
        cout << "Voulez vous modifier d'autres item ? (yes / no)" << endl;
        getline(cin, resp);
        for (int k = 0; k < resp.length(); ++k) {
            tolower(resp[k]);
        }
        if (resp.compare("yes") == 0 || resp.compare("y") == 0 || resp.compare("oui") == 0 || resp.compare("o") == 0) {
            showModifiableItems(modifiableItems);
        }
        else if (resp.compare("no") == 0 || resp.compare("n") == 0 || resp.compare("non") == 0) {
            sendData(data);
        }
    }
    while (resp.compare("yes") != 0 && resp.compare("y") != 0 && resp.compare("oui") != 0 && resp.compare("o") != 0
    && resp.compare("no") != 0 && resp.compare("n") != 0 && resp.compare("non") != 0);
}

bool functions::isItemExist(vector<string> modifiableItems, string item) {
    for (auto it = modifiableItems.begin(); it != modifiableItems.end(); ++it) {
        if (item.compare((*it)) == 0)
            return true;
    }
    return false;
}

string functions::modifyItem(string item){
    string newValue;

    do {
        cout << "Entrez la nouvelle valeur pour l'objet " << item << endl;
        getline(cin, newValue);
    } while (newValue == "");

    
    return newValue;
}


