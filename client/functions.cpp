//
// Created by gunteam on 05/02/2020.
// Source: https://bousk.developpez.com/cours/reseau-c++/UDP/01-introduction-premiers-pas/ - https://bousk.developpez.com/cours/reseau-c++/TCP/04-serveur-premiers-pas
//

// Pourquoi utiliser htons: https://stackoverflow.com/questions/19207745/htons-function-in-socket-programing
//


#include "functions.h"



using namespace std;

functions::functions() {

    /*
     * Création de la socket
     *
     */
     sckt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


    /*
     * Ouverture de la socket
     *
     */
    sockaddr_in addr;

    // Traduction du port en htons port
    addr.sin_port = htons(PORT);

    // Pour IPV4
    addr.sin_family = AF_INET;

    // Binding
    int res = bind(sckt, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    //En cas d'erreur
    if (res != 0)
        throw udpRuntimeException(addr, PORT);



}

functions::~functions() {

}

void functions::sendData(cbor::binary data) {

    sockaddr_in dst;

    /***************************
     *
     *
     *  METTRE L'IP A LA PLACE DE "ADDRESSE"
     *  Et décommenter la ligne
     *
     *
     ***************************/

    inet_pton(AF_INET, "127.0.0.1", &dst.sin_addr);

    cout << data.size() << endl;
    cout << data.capacity() << endl;


    int myId = 5000000;
    int ret = sendto(sckt, (void*) &data, myId, 0, reinterpret_cast<const sockaddr*>(&dst), sizeof(dst));

    //Erreur lors de l'envoi
    if (ret < 0)
        throw udpSendingException();
}

//
// Edit by Anael
//
// Entrées des modifications
// 

cbor::binary functions::entry(){
    double sun_x;
    double sun_y;
    cout << "Entrez la position du soleil sur l'axe X" << endl;
    cin >> sun_x;
    cout << "Entrez la position du soleil sur l'axe Y" << endl;
    cin >> sun_y;

    cbor::map message = {
            { "sun_x", sun_x },
            { "sun_y", sun_y }
    };
    return cbor::encode(message);
	
}


void functions::showModifiableItems(cbor::binary data){
	cbor::map ModifiableItems = cbor::decode(data);
	string choice;
	for(auto it = ModifiableItems.begin(); it != ModifiableItems.end(); ++it){
		cout << "Souhaitez-vous faire cette modification? (y: yes, n: no)" << endl;
		cout << (string) it->first << ": " << (string) it->second << endl;
		cin >> choice;
		if(choice.compare("y")==0){
			//insert function
			break;
		}
	}
	
}
