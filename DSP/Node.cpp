#include "Node.h"

int Node::node_count = 0;

Node::Node(void) : 
	node_id(get_node_number()), 
	generator(std::chrono::system_clock::now().time_since_epoch().count()+(node_id)),
	broadcasting(false), 
	prob(200)
{
}

Node::Node(int id) : 
	generator(std::chrono::system_clock::now().time_since_epoch().count()+(id)), 
	broadcasting(false),
	prob(200)
{
	node_id = id;
}

Node::~Node(void)
{
}


bool Node::broadcast(void)
{
	std::uniform_int_distribution<int> distribution(1,prob);
	if(neighbours.size()==0)
		return false;
	if(distribution(generator)==1)
	{

		broadcasting=true;
		msg_list.clear();
		//printf("Node %d, broadcasts\n",node_id);
		Message m;
		m.type=Message::CONNECT;
		m.root = root;
		m.sender = shared_from_this();
		receive(m);
		for(auto it:neighbours)
			it->receive(m);
	}
	
	return true;
}


void Node::analyze(void)
{

	if(msg_list.size() == 1)
	{
		Message m = msg_list[0];
		switch(m.type)
		{
		case Message::CONNECT :
			if(m.root)
			{
				if(m.root->get_id() != root->get_id())
				{
					

					change_parent(m.sender);

					change_root(m.root);

					delete_neighbour(m.sender->get_id());
					Message response;
					response.type = Message::ACCEPT;
					response.sender = shared_from_this();
					m.sender->response(response);
				}
				else
				{
					Message response;
					response.type = Message::REJECT;
					response.sender = shared_from_this();
					m.sender->response(response);
					delete_neighbour(m.sender->get_id());
				}
			}
			else
			{
				if(root)
					root->change_root(m.root);
				else
					root=m.root;
				if(parent)
					parent->change_parent(shared_from_this());
				parent = m.sender;
				delete_neighbour(m.sender->get_id());
				Message response;
				response.type = Message::ACCEPT;
				response.sender = shared_from_this();
				m.sender->response(response);
			}

			break;
		default:
			printf("wrong message type\n");
		break;
		}
		


	}
	msg_list.clear();
}

void Node::response(Message m)
{
	switch(m.type)
	{
	case  Message::ACCEPT :
		if(delete_neighbour(m.sender->get_id()))
			childrens.push_back(m.sender);
		root->change_root(root);
		break;

	case  Message::REJECT :
		delete_neighbour(m.sender->get_id());
		root->change_root(root);
		break;

	default:
		printf("wrong message type\n");
	break;
	}
}

void Node::receive(Message m)
{
	if(!broadcasting)
		msg_list.push_back(m);
}

void Node::add_neighbour(std::shared_ptr<Node> n)
{
	neighbours.push_back(n);	
}

int Node::get_id(void)
{
	return node_id;
}


void Node::change_root(std::shared_ptr<Node> root)
{
	for(auto it:childrens)
		it->change_root(root);
	this->root=root;
}

void Node::change_parent(std::shared_ptr<Node> parent)
{
	if(this->parent)
	{
		this->childrens.push_back(this->parent);
		this->parent->change_parent(shared_from_this());
	}
	this->delete_children(parent->get_id());
	this->parent = parent;
}

int Node::get_node_number()
{
	return node_count++;
}

std::string Node::printf_tree()
{
	if(root)
		return root->toString();
	else
		return "No root";
}

std::string Node::toString()
{
	std::stringstream ss;
	if(root)
		ss<<"Node["<<node_id<<","<<root->get_id()<<"]={";
	else
		ss<<"Node["<<node_id<<",]={";
	for(auto it:childrens)
		ss<<it->toString()<<", ";
	ss<<"}";
	return ss.str();
}


bool Node::delete_neighbour(int id)
{
	for(int i=0; i<neighbours.size();i++)
	{
		if(neighbours[i]->get_id()==id)
		{
			neighbours.erase(neighbours.begin()+i);
			return true;
		}
	}
	return false;
}

bool Node::delete_children(int id)
{
	for(int i=0; i<childrens.size();i++)
	{
		if(childrens[i]->get_id()==id)
		{
			childrens.erase(childrens.begin()+i);
			return true;
		}
	}
	return false;
}

std::shared_ptr<Node> Node::create_node(int id)
{
	auto tmp = std::make_shared<Node>(id);
	tmp->root = tmp;
	return tmp;
}

std::shared_ptr<Node> Node::create_node()
{
	auto tmp = std::make_shared<Node>();
	tmp->root = tmp;
	return tmp;
}

int Node::get_size()
{
	int tmp = childrens.size();
	for (auto it:childrens)
		tmp += it->get_size();
	return tmp;
}


std::shared_ptr<Node> Node::get_root()
{
	return root;
}
