#include <iostream>
#include "Node.h"

using namespace std;

int main()
{
	std::vector<std::shared_ptr<Node> > node_list;
	for (int i = 0; i < 10; i++)
	{
		node_list.push_back(Node::create_node());
	}
	for(int i = 0; i < node_list.size(); i++)
	{
			node_list[i]->add_neighbour(node_list[(i+1)%node_list.size()]);
			node_list[i]->add_neighbour(node_list[i==0?node_list.size()-1:(i-1)%node_list.size()]);
	}

	while (node_list[0]->get_root()->get_size() != node_list.size()-1)
	{

		for(auto it:node_list)
			it->broadcast();

		for(auto it:node_list)
		{	cout<<it->printf_tree()<<endl; 
		it->analyze();}

		
		
	}
	cout<<node_list[0]->printf_tree()<<endl; 
	getchar();
	return 0;
}