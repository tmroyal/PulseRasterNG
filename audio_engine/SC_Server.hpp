#pragma once

#include <set>
#include <string>

#include "lo/lo_types.h"

using node_id = unsigned int;

class SC_Server;

struct Synth {
    node_id id;
    SC_Server& server;

    Synth(SC_Server& server, node_id id) : id(id), server(server) {}
};

class SC_Server {
public:
    SC_Server(const char *host, const char *port);
    ~SC_Server();
    node_id synth(const std::string& name);
    void set(node_id node, const char* name, int value);
    void set(node_id node, const char* name, double value);

private:
    std::atomic<node_id> current_node{1000};
    std::set<node_id> active_nodes;
    std::mutex m;

    lo_address sc;
};
