#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Factory
{
	class Link
	{
		public:
		
		Factory *factory = NULL;
		float distance;
		float value = -1;
		int nbCyborgNeeded = 0;
		
		Link(Factory *factory, float distance) : factory(factory), distance(distance){};

		void CalculateValue(Factory *source)
		{
			nbCyborgNeeded = 0;
			value = -1;
			if (source != NULL && source->troopNb > factory->troopNb && factory->state != 1 && factory->troopNb > -1)
			{
				nbCyborgNeeded = factory->troopNb + 1;
				value = (factory->production / (float)nbCyborgNeeded) * (float)((factory->state == -1) ? 2 : 1);
			 //   cerr << value  << endl;
			}
		}
		
	};
	
	public:
	
	vector<Link> link;
	int state;                  // player that owns the factory: 1 for you, -1 for your opponent and 0 if neutral
	int troopNb;
	int production;
	int id;
	
	Factory *factoryBestCible = NULL;
	int nbCyborgNeeded = 0;
	float valueOfMove = -1;
	
	
	
	
	void addLink(Factory *factory, float distance)
	{
		link.push_back(Link(factory, distance));
	}
	
	void searchBestMove()
	{
	    
		factoryBestCible = NULL;
		nbCyborgNeeded = 0;
		valueOfMove = -1;

		if (state < 1)
			return ;
		for (int i = 0; i < link.size(); i++)
		{
		    
			link[i].CalculateValue(this);
			if (link[i].value > valueOfMove)
			{
				valueOfMove = link[i].value;
				factoryBestCible = link[i].factory;
				nbCyborgNeeded = link[i].nbCyborgNeeded;
			}
			if (valueOfMove > 0)
			{
                
			    cout << ";" << "MOVE " << to_string(id) << " " << to_string(factoryBestCible->id) << " " << to_string(nbCyborgNeeded);
			}
		}
	}
	
	
};

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
	int factoryCount; // the number of factories
	cin >> factoryCount; cin.ignore();
	int linkCount; // the number of links between factories
	cin >> linkCount; cin.ignore();
	
	vector<Factory> factory;
	factory.assign(factoryCount, Factory());
	for (int i = 0; i < factoryCount; i++)
		factory[i].id = i;
	
	for (int i = 0; i < linkCount; i++)
	{
		int factory1;
		int factory2;
		int distance;
		cin >> factory1 >> factory2 >> distance; cin.ignore();
		factory[factory1].addLink(&factory[factory2], distance);
		factory[factory2].addLink(&factory[factory1], distance);
	}

	// game loop
	while (1)
	{
		int entityCount; // the number of entities (e.g. factories and troops)
		cin >> entityCount; cin.ignore();
		for (int i = 0; i < entityCount; i++) {
			int entityId;
			string entityType;
			int arg1;
			int arg2;
			int arg3;
			int arg4;
			int arg5;
			cin >> entityId >> entityType >> arg1 >> arg2 >> arg3 >> arg4 >> arg5; cin.ignore();
			
			
			if (entityType == "FACTORY")
			{
				factory[entityId].state = arg1;
				factory[entityId].troopNb = arg2;
				factory[entityId].production = arg3;
			}
			
			if (entityType == "TROOP")
			{
			    factory[arg3].troopNb += arg4 * ((factory[arg3].state) ? arg1 * factory[arg3].state : 1);
			}
		}
		
		cout << "WAIT";


		int factorySource = -1;
		int factoryCible = -1;
		int nbCyborg = 0;
		float valueOfBestMove = 0; 
		
		for (int i = 0; i < factoryCount; i++)
		{
			factory[i].searchBestMove();
// 			cerr << factory[i].valueOfMove  << endl;
// 			if (factory[i].valueOfMove > valueOfBestMove)
// 			{
// 				valueOfBestMove = factory[i].valueOfMove;
// 				factorySource = i;
// 				factoryCible = factory[i].factoryBestCible;
// 				nbCyborg = factory[i].nbCyborgNeeded;
// 			}
		}

        cout << endl;
	}
}
