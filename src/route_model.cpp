#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    int i = 0;
    for(auto node : this->Nodes()){
        Node* temp = new Node(i++, this, node);
        mNodes.push_back((*temp));
    }
    CreateNodeToRoadHashmap();
}
double RouteModel::Node::distance(RouteModel::Node node){
    return sqrt(pow(node.x - x, 2) + pow(node.y - y, 2));
}

/**
 * What we do here is that we get the vector of roads
 * Go through them and for all of them that are not footways
 * we  map node ids to their respective roads
 */ 
void RouteModel::CreateNodeToRoadHashmap(){
    for(auto road : Roads()){
        if(road.type != Road::Type::Footway){
            for(auto idx : Ways()[road.way].nodes){
                node_to_road[idx].push_back(&road);
            }
        }
    }
}
//Returns a map of node to roads
std::unordered_map<int, std::vector<Model::Road*>> RouteModel::getNodeToRoadHashmap(){
    return node_to_road;
}
std::vector<RouteModel::Node>& RouteModel::SNodes(){
    return mNodes;
}
RouteModel::Node* RouteModel::Node::FindNeighbor(std::vector<int> node_indices){
    Node* closest_node = nullptr;
    for(int node_index : node_indices){
        Node node = parent_model->SNodes()[node_index];
        if(closest_node!= nullptr && !node.visited && distance(node) < distance(*closest_node)){
            closest_node = &node;
        }
    }
    return closest_node;
}
void RouteModel::Node::FindNeighbors(){
    for(auto road : parent_model->node_to_road[this->index]){
        std::vector<int> temp = parent_model->Ways()[road->way].nodes;
        if(findNeighbor(temp) != NULL){
            this->neighbors.push_back(findNeighbor(temp));
        }
    }
}
RouteModel::Node& RouteModel::FindClosestNode(double x, double y){
    Node temp;
    temp.x = x;
    temp.y = y;
    Node closest_idx;
    double min_dist = std::numeric_limits<float>::min();
    for(auto road : Roads()){
        if(road.type != Road::Type::Footway){
            for(auto node : Ways()[road.way].nodes){
                Node close = SNodes()[node];
                if(temp.distance(close) < min_dist){
                    closest_idx = close;
                }
            }
        }
    }
    return closest_idx;
}
