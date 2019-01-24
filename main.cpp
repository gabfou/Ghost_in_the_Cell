#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


int maxProd = 0;
bool EnemyBomb = false;

template <typename T> int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

class Factory
{
	class Link
	{
		public:
		
		Factory *factory = NULL;
		int distance;
		float value = -1;
		int nbCyborgNeeded = 0;
		
		Link(Factory *factory, int distance) : factory(factory), distance(distance){};

		void CalculateValue(Factory *source, bool charge = false)
		{
			nbCyborgNeeded = 0;
			value = -1;
			int cibleTroop = factory->nbOfTroopInNTurn(distance);
			int nbCyborgToBeat = (factory->state != 1) ? cibleTroop + factory->nbOfReinforcementInNTurn(distance) : -cibleTroop;
			if (source != NULL
			    && source->troopNb > nbCyborgToBeat
			    && source->nbOfTroopInNTurn(20, -nbCyborgToBeat - 1) > 0
			    && nbCyborgToBeat > -1
			    && factory->production != 0)
			{
				nbCyborgNeeded = nbCyborgToBeat + 1;
				value = (((float)factory->production / (float)nbCyborgNeeded)) * (float)((factory->state == -1) ? 20000 : 1);
			}
			else if (factory->state == -1 && factory->production == maxProd && factory->bombed == 0 && factory->bombApproaching.empty())
			{
			    cout << ";BOMB " << to_string(source->id) << " " << to_string(factory->id);
			    factory->bombApproaching.push_back(distance);
			}
// 			if (source->id == 1)
// 			    cerr << "source " << source->id << " cible " << factory->id << " troop " << cibleTroop << " nbCyborgToBeat "  << nbCyborgToBeat << " value " << value << " " << factory->nbOfReinforcementInNTurn(distance) <<endl;
		}
		
	};
	
	class Troop
	{
		public:

	    int nb;
	    int distance;
	    
	    Troop(int nb, int distance) : nb(nb), distance(distance){};
	};
	
	public:
	
	vector<Link> link;
	int state;                  // player that owns the factory: 1 for you, -1 for your opponent and 0 if neutral
	int troopNb;
	int production;
	int id;
	int bombed = 0;
	
	vector<Troop> troopApproaching;
	vector<int> bombApproaching;
	
	Factory *factoryBestCible = NULL;
	int nbCyborgNeeded = 0;
	float valueOfMove = -1;
	
	
	
	
	void addLink(Factory *factory, float distance)
	{
		link.push_back(Link(factory, distance));
	}

	void addTroopApproaching(int nb, int distance, int state)
	{
		troopApproaching.push_back(Troop((state * this->state > 0) ? nb : -nb, distance));
	}
	
	int distEnemy()
	{
	    int dist = 20000;
	    for (int i = 0; i < link.size(); i++)
	    {
	        if (link[i].factory->state != 1 && link[i].factory->production != 0 && link[i].distance < dist)
	            dist = link[i].distance;
	    }
        return (dist);
	}
	
	void searchBestMove(bool charge = false)
	{
	    
		factoryBestCible = NULL;
		nbCyborgNeeded = 0;
		valueOfMove = -1;
		int distance = -1;
		int nbCyborgUseless;

		if (state < 1)
			return ;
		for (int i = 0; i < link.size(); i++)
		{
		    
			link[i].CalculateValue(this, charge);
			if (link[i].value > valueOfMove)
			{
				valueOfMove = link[i].value;
				factoryBestCible = link[i].factory;
				nbCyborgNeeded = link[i].nbCyborgNeeded;
				distance = link[i].distance;
			}
		}
		if (valueOfMove > 0 && troopNb > 0)
		{
            troopNb -= max(0, nbCyborgNeeded);
            // factoryBestCible->troopNb -= nbCyborgNeeded;
            factoryBestCible->addTroopApproaching(nbCyborgNeeded, distance, state);
            
		    cout << ";MOVE " << to_string(id) << " " << to_string(factoryBestCible->id) << " " << to_string(nbCyborgNeeded);
		    searchBestMove();
		}
// 		else if(EnemyBomb && charge == false)
// 		    searchBestMove(true);
		else if (troopNb >= 10 && production < 3 && nbOfTroopInNTurn(20, -10) - 3 > 0)
	        cout << ";INC " << to_string(id);
	    else if (troopNb > 0 &&  (nbCyborgUseless = nbOfTroopInNTurn(20) - 3) > 0 && (production == 3))
	    {
	        int idCible = -1;
	        int dist = distEnemy();
	        int tmp;
	        
    	    for (int i = 0; i < link.size(); i++)
    	    {
                if (link[i].factory->state == 1 && link[i].factory->production != 0 && ((tmp = link[i].factory->distEnemy()) < dist))
                {
	               idCible = link[i].factory->id;
	               dist = tmp;
	            }
    	    }
    	    if (idCible != -1)
    	    {
    	        cout << ";MOVE " << to_string(id) << " " << to_string(idCible) << " " << to_string(nbCyborgUseless);
    	        troopNb -= nbCyborgUseless;
    	    }
	    }
	}
	
	int nbOfReinforcementInNTurn(int N)
	{
	    int r = 0;
	    for (int i = 0; i < link.size(); i++)
	    {
	        if (link[i].distance < N && link[i].factory->state == -1)
	        {
    	        r += max(0, link[i].factory->nbOfTroopInNTurn(N - link[i].distance));
	        }
	    }
	    return r;
	}

	int nbOfTroopInNTurn(int N, int diff = 0)
	{
	    int turnWithoutProduction = bombed;
		int r = troopNb + diff;


	    for (int j = 0; j <= N; j++)
        {	    
            if (state != 0 && turnWithoutProduction-- < 0)
    			r += production * sgn(r);
    			
    		for (int i = 0; i < troopApproaching.size(); i++)
    		{
    		    if (troopApproaching[i].distance == j)
    			r += troopApproaching[i].nb;
    		}
    		
    		for (int i = 0; i < bombApproaching.size(); i++)
    		{
    			if (bombApproaching[i] == j)
    			{
    				r = (abs(r) < 20) ? (abs(r) < 10) ? 0 : r - sgn(r) * 10 : r / 2;
    				turnWithoutProduction = 5;
    			}
    		}
            
        }
        		if (id == 3)
		    cerr <<  r << " lol " << diff << endl;
		return r;
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
	    
	    for (int i = 0; i < factoryCount; i++)
		{
		    factory[i].troopApproaching.clear();
		    factory[i].bombApproaching.clear();
		}
	    
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
				if (factory[entityId].production < arg3)
				    factory[entityId].production = arg3;
				factory[entityId].bombed = arg4;
				if (arg3 > maxProd)
				    maxProd= arg3;
			}
			if (entityType == "TROOP")
				factory[arg3].addTroopApproaching(arg4, arg5, arg1);
			EnemyBomb = false;
			if (entityType == "BOMB")
			{
			    if (arg3 != -1)
			        factory[arg3].bombApproaching.push_back(arg4);
			    else
			        EnemyBomb = true;
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
