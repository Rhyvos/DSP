#pragma once
#include <memory>
#include <vector>
#include <random>
#include <chrono>
#include <string>
#include <sstream> 
struct Message;

class Node : public std::enable_shared_from_this<Node>
{
public:
	Node(void);
	Node(int id);
	~Node(void);
private:
	int node_id;
	std::shared_ptr<Node> root;
	std::vector<std::shared_ptr<Node> > neighbours;
	std::vector<std::shared_ptr<Node> > childrens;
	std::shared_ptr<Node> parent;
	size_t tree_id;
	std::vector<Message> msg_list;
	std::default_random_engine generator;
	std::string toString();
	bool broadcasting;
public:
	static int node_count;
	static int get_node_number();
	int prob;
	bool broadcast(void);
	void analyze(void);
	void response(Message m);
	void receive(Message m);
	void add_neighbour(std::shared_ptr<Node> n);
	int get_id(void);
	void change_root(std::shared_ptr<Node> root);
	void change_parent(std::shared_ptr<Node> parent);
	std::string printf_tree();
	bool delete_neighbour(int id);
	bool delete_children(int id);
	static std::shared_ptr<Node> create_node(int id);
	static std::shared_ptr<Node> create_node();
	int get_size();
	std::shared_ptr<Node> get_root();
};

struct Message
{
	enum E_TYPE {
		CONNECT,
		ACCEPT,
		REJECT
	};
	E_TYPE type;
	std::shared_ptr<Node> sender;
	std::shared_ptr<Node> root;

	std::string to_string()
	{
		std::stringstream ss;
		ss<<"------------------"<<std::endl;
		ss<<"TYPE="<<type<<std::endl;
		if(sender)
			ss<<"Sender="<<sender->get_id()<<std::endl;
		if(root)
			ss<<"Root="<<root->get_id()<<std::endl;
		ss<<"------------------"<<std::endl;
		return ss.str();
	}
};
