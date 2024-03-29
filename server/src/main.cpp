#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
using namespace std;

#define PORT 6900


struct player{
	sf::TcpSocket socket;
	sf::Socket::Status status;
	string username;

	float pos = 0;
	float bx = 0, by = 0;
	float bdx = 0, bdy = 0;
	int scores[2] = {0};
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

	unsigned int game_count = 0;
	while(true) {
		player clients[2];
		sf::Packet packet;
		for(int i = 0; i < 2; i++){
			if(listener.accept(clients[i].socket) != sf::Socket::Done){
				cout << "Error while accepting client connection!" << endl;
				return -1;
			}

			cout << "Connection accepted: ";
			clients[i].socket.receive(packet);
			packet >> clients[i].username >> clients[i].pos;
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

		cout << "Starting game #" << game_count << endl;

		thread t1([&]() {
			int score_desync_counter = 0;
			int ball_desync_counter = 0;
			while (clients[0].status == sf::Socket::Done) {
				sf::Packet p;
				clients[0].status = clients[0].socket.receive(p);
				p >> clients[0].pos >> clients[0].bx >> clients[0].by >> clients[0].bdx >> clients[0].bdy >> clients[0].scores[0] >> clients[0].scores[1];

				// only need one thread to warn about desync
				// use counter to only warn about sustained desync
				if(clients[0].scores[0] != clients[1].scores[0] || clients[0].scores[1] != clients[1].scores[1]){
				  score_desync_counter++;
				  if(score_desync_counter >= 10){
				    cout << "Score desync!" << endl;
				    score_desync_counter = 0;
				  }
				}
				else score_desync_counter = 0;

				// this abomination basically returns true if the 
				// ball's position or velocity is different by a relatively
				// significant margin
				if( (clients[0].bx + 5 < clients[1].bx || clients[0].bx - 5 > clients[1].bx ) || 
				    (clients[0].by + 5 < clients[1].by || clients[0].by - 5 > clients[1].by)  ||
				    (clients[0].bdx + 1 < clients[1].bdx || clients[0].bdx - 1 > clients[1].bdx) ||
				    (clients[0].bdy + 1 < clients[1].bdy || clients[0].bdy - 1 > clients[1].bdy) 
				  ){
				  ball_desync_counter++;
				  if(ball_desync_counter >= 50){
				    cout << "Ball desync!" << endl;
				    ball_desync_counter = 0;
				  }
				}
				else ball_desync_counter = 0;

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
				p >> clients[1].pos >> clients[1].bx >> clients[1].by >> clients[1].bdx >> clients[1].bdy >> clients[1].scores[1] >> clients[1].scores[0];

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
		game_count++;
	}
}
