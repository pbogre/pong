//todo always run and accept max 2 connections
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
using namespace std;

#define PORT 6900


struct player{
	sf::TcpSocket socket;
	sf::Socket::Status status;
	string username;

	float pos;
	bool side;
};

int main(){
	cout << "Setting up TCP listener..." << endl;

	sf::TcpListener listener;
	if(listener.listen(PORT) != sf::Socket::Done){
		cout << "Error while listening to port " << PORT << "!" << endl;
		return -1;
	}

	cout << "Listening on port " << PORT << endl
		 << "Attending client connections..." << endl;

	while(true){
		player clients[2];
		sf::Packet packet;
		float ipos = 0;
		for(int i = 0; i < 2; i++){
			if(listener.accept(clients[i].socket) != sf::Socket::Done){
				cout << "Error while accepting client connection!" << endl;
				return -1;
			}

			cout << "Connection accepted: ";
			clients[i].socket.receive(packet);
			packet >> clients[i].username >> ipos;
			cout << clients[i].username << endl;
			packet.clear();

			clients[i].status = sf::Socket::Done;
			clients[i].side = i;
		}

		// Exchange usernames
		cout << "Both players found. Exchanging information..." << endl;

		packet << clients[1].username << clients[0].side;
		clients[0].socket.send(packet);
		packet.clear();

		packet << clients[0].username << clients[1].side;
		clients[1].socket.send(packet);
		packet.clear();

		cout << "Starting game loop..." << endl;

		float bx, by;
		thread t1([&]() {
			while (clients[0].status == sf::Socket::Done) {
				sf::Packet p;
				clients[0].status = clients[0].socket.receive(p);
				p >> clients[0].pos >> bx >> by;
				p.clear();

				p << clients[0].pos << bx << by;
				clients[1].socket.send(p);
				p.clear();
			}
			cout << "A player has disconnected." << endl;
			packet << -1;
			clients[1].socket.send(packet);
			packet.clear();
		});

		thread t2([&]() {
			while (clients[1].status == sf::Socket::Done) {
				sf::Packet p;
				clients[1].status = clients[1].socket.receive(p);
				p >> clients[1].pos >> bx >> by;
				p.clear();

				p << clients[1].pos << bx << by;
				clients[0].socket.send(p);
				p.clear();
			}
			cout << "A player has disconnected." << endl;
			packet << -1;
			clients[0].socket.send(packet);
			packet.clear();
		});

		t1.join();
		t2.join();
	}
}
